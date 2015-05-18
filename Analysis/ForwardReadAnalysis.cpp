
#include "LivenessArray.h"
#include "ForwardReadAnalysis.h"

ForwardReadAnalysis::ForwardReadAnalysis() {
	this->window_ = 100;
	ud_init(&this->ud_obj_);
	ud_set_mode(&this->ud_obj_, 32);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
}

ForwardReadAnalysis::ForwardReadAnalysis(int window) {
	this->window_ = window;
	ud_init(&this->ud_obj_);
	ud_set_mode(&this->ud_obj_, 32);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
}

ForwardReadAnalysis::~ForwardReadAnalysis() {
	for(int i = 0; i < this->instructions_.size(); i++) {
		delete this->instructions_[i];
	}
	
	LiveMap::iterator itLiveMap;
	for(itLiveMap = this->liveLocations_.begin(); itLiveMap != this->liveLocations_.end(); itLiveMap++) {
		itLiveMap->second->removeAllLocations();
	}
}

void ForwardReadAnalysis::addInstruction(Instruction *instruction) {
	this->instructions_.push_back(new Instruction(
		instruction->getAddress(),
		instruction->getAssembly(),
		instruction->getBinary(),
		instruction->getBinarySize()
	));
	
	Live *live;
	if(this->liveLocations_.find(instruction->getAddress()) == this->liveLocations_.end()) {
		this->liveLocations_[instruction->getAddress()] = new Live(instruction->getAddress());
	}
	live = this->liveLocations_[instruction->getAddress()];
	
	int start = this->instructions_.size() - this->window_;
	if(start < 0) {
		start = 0;
	}
	for(int i = start; i < this->instructions_.size(); i++) {
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
		
		}
	
		// Causes all previous ESP values to be invalidated
		if(this->ud_obj_.mnemonic == UD_Ipush) {
			this->removeRegister(live, UD_R_ESP);
			this->removeMemory(live, UD_R_ESP);
		}

		// Causes all previous ESP values to be invalidated
		if(this->ud_obj_.mnemonic == UD_Ipop) {
			this->removeRegister(live, UD_R_ESP);
			this->removeMemory(live, UD_R_ESP);
		}
	
		Tephon::insn_liveliness_t opReadWrite = Tephon::operand_liveliness[this->ud_obj_.mnemonic];
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_WRITE){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					this->removeRegister(live, this->ud_obj_.operand[i].base);
					this->removeMemory(live, this->ud_obj_.operand[i].base);
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
					this->removeMemory(live, offset, this->ud_obj_.operand[i].base, this->ud_obj_.operand[i].index);
					this->addRegister(live, this->ud_obj_.operand[i].base);
					this->addRegister(live, this->ud_obj_.operand[i].index);
				}
			}
		}
	
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_READ_WRITE){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					this->removeRegister(live, this->ud_obj_.operand[i].base);
					this->removeMemory(live, this->ud_obj_.operand[i].base);
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
					this->removeMemory(live, offset, this->ud_obj_.operand[i].base, this->ud_obj_.operand[i].index);
					this->addRegister(live, this->ud_obj_.operand[i].base);
					this->addRegister(live, this->ud_obj_.operand[i].index);
				}
			}
		}
	
		for(int i = 0; i < 3; i++){
			if(opReadWrite.type[i] == Tephon::LL_READ){
				if(this->ud_obj_.operand[i].type == UD_OP_REG){
					this->addRegister(live, this->ud_obj_.operand[i].base);
				}else if(this->ud_obj_.operand[i].type == UD_OP_MEM){
					uint64_t offset = getOffset(&this->ud_obj_.operand[i]);
					if(this->ud_obj_.operand[i].base != UD_NONE || this->ud_obj_.operand[i].index != UD_NONE) {
						this->addMemory(live, offset, this->ud_obj_.operand[i].base, this->ud_obj_.operand[i].index);
					}
					this->addRegister(live, this->ud_obj_.operand[i].base);
					this->addRegister(live, this->ud_obj_.operand[i].index);
				}
			}
		}
	}
}

const Instruction::List &ForwardReadAnalysis::getInstructions() const {
	return this->instructions_;
}
