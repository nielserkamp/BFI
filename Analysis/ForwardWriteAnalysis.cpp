#include "LivenessArray.h"
#include "ForwardWriteAnalysis.h"

ForwardWriteAnalysis::ForwardWriteAnalysis() {
	this->window_ = 20;
	ud_init(&this->ud_obj_);
	ud_set_mode(&this->ud_obj_, 32);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
}

ForwardWriteAnalysis::ForwardWriteAnalysis(int window) {
	this->window_ = window;
	ud_init(&this->ud_obj_);
	ud_set_mode(&this->ud_obj_, 32);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
}

ForwardWriteAnalysis::~ForwardWriteAnalysis() {
	
}


const std::vector<InstructionPtr> &ForwardWriteAnalysis::getInstructions() const {
	return this->instructions_;
}

bool ForwardWriteAnalysis::analyse(uint32_t address, const char *binary, int size) {
	ud_t ud_obj;
	ud_init(&ud_obj);
	ud_set_syntax(&ud_obj, UD_SYN_ATT);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_mode(&ud_obj, 32);

	ud_set_pc(&ud_obj, address);
	ud_set_input_buffer(&ud_obj, (uint8_t *)binary, size);
	for(int i = 0; i < window_; i++) {
		if(!ud_disassemble(&ud_obj)) {
			return false;
		}
		
		InstructionPtr instruction(new Instruction(
			(uint64_t)(ud_obj.pc - ud_insn_len(&ud_obj)), 
			ud_insn_asm(&ud_obj), 
			(const char *)ud_insn_ptr(&ud_obj), 
			(int)ud_insn_len(&ud_obj)
		));

		addInstruction(instruction);
	}

	return true;
}

void ForwardWriteAnalysis::addInstruction(InstructionPtr &instruction) {
	// We do the analysis on the previous instructions, because else we would get 
	// new locations that are only going to be written and might not have changed yet
	// (pre instruction value has not yet been written, and is written post instruction)
	// we delay the analysis by 1 instruction
	InstructionPtr iPtr(new Instruction(
		instruction->getAddress(),
		instruction->getAssembly(),
		instruction->getBinary(),
		instruction->getBinarySize()
	));
	
	this->instructions_.push_back(iPtr);

	LivePtr live;
	if(this->liveLocations_.find(instruction->getAddress()) == this->liveLocations_.end()) {
		LivePtr lPtr(new Live(instruction->getAddress()));
		this->liveLocations_[instruction->getAddress()] = lPtr;
	}
	live = this->liveLocations_[instruction->getAddress()];
	
	int start = this->instructions_.size() - this->window_;
	if(start < 0) {
		start = 0;
	}
	for(int i = start; i < this->instructions_.size() - 1; i++) {
		ud_set_input_buffer(&this->ud_obj_, (uint8_t *)this->instructions_[i]->getBinary(), this->instructions_[i]->getBinarySize());
		if(!ud_disassemble(&this->ud_obj_)) {
			fprintf(stderr, "Error: Failed to disassemble\n");
			exit(1);
		}
	
		if(this->ud_obj_.mnemonic == UD_Iret) {
			this->addRegister(live, UD_R_ESP);
			this->removeMemory(live, UD_R_ESP);
			this->removeMemory(live, UD_R_SP);
		}			

		if(this->ud_obj_.mnemonic == UD_Icall) {
			this->addRegister(live, UD_R_ESP);
			//this->removeMemory(UD_R_ESP);
		}
	
		// Causes all previous ESP values to be invalidated
		if(this->ud_obj_.mnemonic == UD_Ipush) {
			this->addRegister(live, UD_R_ESP);
			this->removeMemory(live, UD_R_ESP);
		}

		// Causes all previous ESP values to be invalidated
		if(this->ud_obj_.mnemonic == UD_Ipop) {
			this->addRegister(live, UD_R_ESP);
			this->removeMemory(live, UD_R_ESP);
		}
	
		Tephon::insn_liveliness_t opReadWrite = Tephon::operand_liveliness[this->ud_obj_.mnemonic];
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_READ){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					// Do nothing
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					// Do nothing
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
				}
			}
		}
	
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_WRITE){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					this->addRegister(live, this->ud_obj_.operand[i].base);
					this->removeMemory(live, this->ud_obj_.operand[i].base);
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
					if(this->ud_obj_.operand[i].base != UD_NONE || this->ud_obj_.operand[i].index != UD_NONE) {
						this->addMemory(live, offset, this->ud_obj_.operand[i].base, this->ud_obj_.operand[i].index);
					}
				}
			}
		}
	
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_READ_WRITE){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					this->addRegister(live, this->ud_obj_.operand[i].base);
					this->removeMemory(live, this->ud_obj_.operand[i].base);
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
					if(this->ud_obj_.operand[i].base != UD_NONE || this->ud_obj_.operand[i].index != UD_NONE) {
						this->addMemory(live, offset, this->ud_obj_.operand[i].base, this->ud_obj_.operand[i].index);
					}
				}
			}
		}
	}
}
