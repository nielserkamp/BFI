#include <algorithm>
#include <set>
#include <string.h>

#include "BackwardReadAnalysis.h" 
#include "LivenessArray.h"

BackwardReadAnalysis::BackwardReadAnalysis() {
	this->window_ = 5;
	this->cfg_ = 0;
	ud_init(&this->ud_obj_);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
	ud_set_mode(&this->ud_obj_, 32);
}

BackwardReadAnalysis::BackwardReadAnalysis(int window) {
	this->window_ = window;
	this->cfg_ = 0;
	ud_init(&this->ud_obj_);
	ud_set_syntax(&this->ud_obj_, UD_SYN_ATT);
	ud_set_vendor(&this->ud_obj_, UD_VENDOR_INTEL);
	ud_set_mode(&this->ud_obj_, 32);
}

BackwardReadAnalysis::~BackwardReadAnalysis() {
	if(this->cfg_) {
		delete this->cfg_;
	}
	
	liveLocations_.clear();
}

void BackwardReadAnalysis::setCFG(CFG *cfg) {
	if(this->cfg_) {
		delete this->cfg_;
	}
	
	this->cfg_ = cfg;
}

CFG *BackwardReadAnalysis::getCFG() {
	return this->cfg_;
}

void BackwardReadAnalysis::analyiseInstruction(LivePtr &live, InstructionPtr &instruction) {
	ud_set_input_buffer(&this->ud_obj_, (uint8_t *)instruction->getBinary(), instruction->getBinarySize());
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
	}

	if(this->ud_obj_.mnemonic == UD_Ipush) {
		this->removeRegister(live, UD_R_ESP);
		this->removeMemory(live, UD_R_ESP);
	}

	if(this->ud_obj_.mnemonic == UD_Ipop) {
		this->removeRegister(live, UD_R_ESP);
		this->removeMemory(live, UD_R_ESP);
	}

	Tephon::insn_liveliness_t opReadWrite = Tephon::operand_liveliness[this->ud_obj_.mnemonic];
	for(int k = 0; k < 3; k++) {
	
		if(opReadWrite.type[k] == Tephon::LL_WRITE){
			if(this->ud_obj_.operand[k].type == UD_OP_REG) {
				this->removeRegister(live, this->ud_obj_.operand[k].base);
				this->removeMemory(live, this->ud_obj_.operand[k].base);
			} else if(this->ud_obj_.operand[k].type == UD_OP_MEM) {
				uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
				if(this->ud_obj_.operand[k].base != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].base);
				}
				if(this->ud_obj_.operand[k].index != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].index);
				}
				if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
					this->removeMemory(live, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
				}
			}
		}
	}

	if(this->ud_obj_.mnemonic == UD_Icmpsb 
		|| this->ud_obj_.mnemonic == UD_Icmpsw 
		|| this->ud_obj_.mnemonic == UD_Icmpsd
	) {
		this->addRegister(live, UD_R_EDI);
		this->addRegister(live, UD_R_ESI);
		this->addRegister(live, UD_R_ECX);
	}

	for(int k = 0; k < 3; k++) {
		if(opReadWrite.type[k] == Tephon::LL_READ_WRITE){
			if(this->ud_obj_.operand[k].type == UD_OP_REG){
				this->addRegister(live, this->ud_obj_.operand[k].base);
				this->removeMemory(live, this->ud_obj_.operand[k].base);
			}else if(this->ud_obj_.operand[k].type == UD_OP_MEM){
				uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
				if(this->ud_obj_.operand[k].base != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].base);
				}
				if(this->ud_obj_.operand[k].index != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].index);
				}
				if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
					this->addMemory(live, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
				}
			}
		}
	}

	for(int k = 0; k < 3; k++){
		if(opReadWrite.type[k] == Tephon::LL_READ){
			if(this->ud_obj_.operand[k].type == UD_OP_IMM && this->ud_obj_.mnemonic != UD_Ijmp) {
				if(this->ud_obj_.operand[k].size == 32 && this->ud_obj_.operand[k].lval.sdword >= 0) {
					for(int i = 0; i < instruction->getBinarySize() - 3; i++) {
						if(memcmp(instruction->getBinary() + i, (char *)&this->ud_obj_.operand[k].lval.sdword, 4) == 0) {
							LocationPtr loc(new InstructionLocation(instruction->getAddress() + i, 4));
							
							for(size_t i = 0; i < live->getLocations().size(); i++) {
								if(live->getLocations()[i]->compare(loc.get()) == 0) {
									break;
								}
							}
							live->addLocation(loc);
							
							break;
						}
					}
				}
			} else if(this->ud_obj_.operand[k].type == UD_OP_REG) {
				this->addRegister(live, this->ud_obj_.operand[k].base);
			}else if(this->ud_obj_.operand[k].type == UD_OP_MEM){
				uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
				if(this->ud_obj_.operand[k].base != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].base);
				}
				if(this->ud_obj_.operand[k].index != UD_NONE) {
					this->addRegister(live, this->ud_obj_.operand[k].index);
				}
				if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
					this->addMemory(live, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
				}
			}
		}
	}
}

void BackwardReadAnalysis::analyseNode(LivePtr &live, CFGNodePtr &node, std::set<CFGNodePtr> seen, int left) {
	std::vector<InstructionPtr> instructions = node->getInstructions();
	std::vector<std::weak_ptr<CFGNode> > nodes = node->getPrevNodes();
	this->count++;
	if(left <= 0) {
		return;
	}
	
	size_t numTodo = (size_t)left < instructions.size() ? left : instructions.size();
	if(seen.find(node) != seen.end()) {
		return;
	}
	seen.insert(node);
	
	for(size_t i = 0; i < nodes.size(); i++) {
		LivePtr prevLive(new Live());
		
		CFGNodePtr n = nodes[i].lock();
		analyseNode(prevLive, n, seen, left - numTodo);
		
		std::vector<LocationPtr> locations = prevLive->getLocations();
		for(size_t j = 0; j < locations.size(); j++) {
			if(!live->containsLocation(locations[j])) {
				live->addLocation(locations[j]);
			}
		}
	}
	
	int stopAt = instructions.size() - 1 - numTodo;
	stopAt = stopAt > 0 ? stopAt : 0;
	for(int i = ((int)instructions.size()) - 1; i >= stopAt; i--) {
		analyiseInstruction(live, instructions[i]);
	}
}

void BackwardReadAnalysis::analyse() {
	this->instructionsDone_ = 0;
	const std::vector<CFGNodePtr> nodes = this->cfg_->getNodes();
	
	for(size_t i = 0; i < nodes.size(); i++) {
		CFGNodePtr node = nodes[i];
		std::vector<InstructionPtr> instructions = nodes[i]->getInstructions();
		
		std::map<Address, InstructionPtr>::iterator itInstructions;
		for(size_t j = 0; j < instructions.size(); j++) {
			InstructionPtr instruction = instructions[j];
			LivePtr live(new Live(instruction->getAddress()));
			
			for(size_t k = 0; k < node->getPrevNodes().size(); k++) {
				LivePtr prevLive(new Live());
		
				std::set<CFGNodePtr> seen;
				seen.insert(node);
				
				CFGNodePtr n = node->getPrevNodes()[k].lock();
				analyseNode(prevLive, n, seen, this->window_);
				
				std::vector<LocationPtr> locations = prevLive->getLocations();
				for(int h = 0; h < locations.size(); h++) {
					if(!live->containsLocation(locations[h])) {
						live->addLocation(locations[h]);
					}
				}
			}

			for(int k = instructions.size(); k > j; k--) {
				analyiseInstruction(live, instructions[k - 1]);
			}
			this->liveLocations_[instruction->getAddress()] = live;
			this->instructionsDone_++;
		}
		
	}
}

/*void BackwardReadAnalysis::analyse() {
	const CFG::Node::List nodes = this->cfg_->getNodes();
	
	std::vector<Refract> refracts;
	
	for(int i = 0; i < nodes.size(); i++) {
		Refract r;
		r.node = nodes[i];
		refracts.push_back(r);
	}
	
	while(refracts.begin() != refracts.end()) {
		count++;
	
		CFG::Node *node = (*refracts.begin()).node;
		Instruction::List *instructions = &(*refracts.begin()).instructions;
		std::set<CFG::Node *> *nodesSeen = &(*refracts.begin()).nodesSeen;

		bool finished = false;
		if(nodesSeen->find(node) != nodesSeen->end()) {
			refracts.erase(refracts.begin());
			continue;
		}
		bool different = false;
		for(int i = node->getInstructions().size() - 1; i >= 0; i--) {
			instructions->push_back(node->getInstructions()[i]);
			
			// Get live values
			Live *liveLocation;
			if(this->liveLocations_.find(node->getInstructions()[i]->getAddress()) == this->liveLocations_.end()) {
				liveLocation = new Live(node->getInstructions()[i]->getAddress());
				this->liveLocations_[node->getInstructions()[i]->getAddress()] = liveLocation;
			} else {
				liveLocation = this->liveLocations_[node->getInstructions()[i]->getAddress()];
			}
			
			// Get Copy
			Live copyLiveLocation;
			for(int j = 0; j < liveLocation->getLocations().size(); j++) {
				copyLiveLocation.addLocation(liveLocation->getLocations()[j]);
			}
			
			// Analysis
			int diff = instructions->size() - 10;
			if(diff < 0) {
				diff = 0;
			}
			//Live *liveLocation = new Live();
			for(int j = diff; j < instructions->size(); j++) {
				Instruction *instruction = (*instructions)[j];
				ud_set_input_buffer(&this->ud_obj_, (uint8_t *)instruction->getBinary(), instruction->getBinarySize());
				if(!ud_disassemble(&this->ud_obj_)) {
					fprintf(stderr, "Error: Failed to disassemble\n");
					exit(1);
				}
				
				if(this->ud_obj_.mnemonic == UD_Iret) {
					this->addRegister(liveLocation, UD_R_ESP);
					this->removeMemory(liveLocation, UD_R_ESP);
					this->removeMemory(liveLocation, UD_R_SP);
				}			

				if(this->ud_obj_.mnemonic == UD_Icall) {
					this->addRegister(liveLocation, UD_R_ESP);
				}
	
				if(this->ud_obj_.mnemonic == UD_Ipush) {
					this->removeRegister(liveLocation, UD_R_ESP);
					this->removeMemory(liveLocation, UD_R_ESP);
				}

				if(this->ud_obj_.mnemonic == UD_Ipop) {
					this->removeRegister(liveLocation, UD_R_ESP);
					this->removeMemory(liveLocation, UD_R_ESP);
				}
				
				Tephon::insn_liveliness_t opReadWrite = Tephon::operand_liveliness[this->ud_obj_.mnemonic];
				for(int k = 0; k < 3; k++){
					if(opReadWrite.type[k] == Tephon::LL_WRITE){
						if(this->ud_obj_.operand[k].type == UD_OP_REG){
							this->removeRegister(liveLocation, this->ud_obj_.operand[k].base);
							this->removeMemory(liveLocation, this->ud_obj_.operand[k].base);
						}else if(this->ud_obj_.operand[k].type == UD_OP_MEM){
							uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
							if(this->ud_obj_.operand[k].base != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].base);
							}
							if(this->ud_obj_.operand[k].index != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].index);
							}
							if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
								this->removeMemory(liveLocation, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
							}
						}
					}
				}
		
				if(this->ud_obj_.mnemonic == UD_Icmpsb 
					|| this->ud_obj_.mnemonic == UD_Icmpsw 
					|| this->ud_obj_.mnemonic == UD_Icmpsd
				) {
					this->addRegister(liveLocation, UD_R_EDI);
					this->addRegister(liveLocation, UD_R_ESI);
					this->addRegister(liveLocation, UD_R_ECX);
				}
		
				for(int k = 0; k < 3; k++){
					if(opReadWrite.type[k] == Tephon::LL_READ_WRITE){
						if(this->ud_obj_.operand[k].type == UD_OP_REG){
							this->addRegister(liveLocation, this->ud_obj_.operand[k].base);
							this->removeMemory(liveLocation, this->ud_obj_.operand[k].base);
						}else if(this->ud_obj_.operand[k].type == UD_OP_MEM){
							uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
							if(this->ud_obj_.operand[k].base != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].base);
							}
							if(this->ud_obj_.operand[k].index != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].index);
							}
							if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
								this->addMemory(liveLocation, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
							}
						}
					}
				}
	
				for(int k = 0; k < 3; k++){
					if(opReadWrite.type[k] == Tephon::LL_READ){
						if(this->ud_obj_.operand[k].type == UD_OP_REG){
							this->addRegister(liveLocation, this->ud_obj_.operand[k].base);
						}else if(this->ud_obj_.operand[k].type == UD_OP_MEM){
							uint64_t offset = getOffset(&this->ud_obj_.operand[k]);
							if(this->ud_obj_.operand[k].base != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].base);
							}
							if(this->ud_obj_.operand[k].index != UD_NONE) {
								this->addRegister(liveLocation, this->ud_obj_.operand[k].index);
							}
							if(this->ud_obj_.operand[k].base != UD_NONE || this->ud_obj_.operand[k].index != UD_NONE) {
								this->addMemory(liveLocation, offset, this->ud_obj_.operand[k].base, this->ud_obj_.operand[k].index);
							}
						}
					}
				}
			}
			
			for(int i = 0; i < liveLocation->getLocations().size(); i++) {
				if(!liveLocation->containsLocation(liveLocation->getLocations()[i])) {
					liveLocation->addLocation(liveLocation->getLocations()[i]);
				} else {
					delete liveLocation->getLocations()[i];
				}
			}
			delete liveLocation;
			
			
			// If no change occured
			if(nodesSeen->find(node) == nodesSeen->end()) {
				different = true;
			}
			if(liveLocation->getLocations().size() == copyLiveLocation.getLocations().size()) {
				for(int j = 0; j < liveLocation->getLocations().size(); j++) {
					if(liveLocation->getLocations()[j]->compare(copyLiveLocation.getLocations()[j]) != 0) {
						different = true;
						break;
					}
				}
			}
		}
		
		if(!different) {
			refracts.erase(refracts.begin());
			continue;
		}
		
		nodesSeen->insert(node);
		
		for(int i = 0; i < node->getPrevNodes().size(); i++) {
			Refract r;
			r.node = node->getPrevNodes()[i];
			
			for(int j = 0; j < instructions->size(); j++) {
				r.instructions.push_back((*instructions)[j]);
			}
			
			std::set<CFG::Node *>::iterator itNodesSeen;
			for(itNodesSeen = nodesSeen->begin(); itNodesSeen != nodesSeen->end(); itNodesSeen++) {
				r.nodesSeen.insert(*itNodesSeen);
			}
			
			refracts.push_back(r);
		}
		refracts.erase(refracts.begin());
	}
}*/

void BackwardReadAnalysis::analyse(Address address, const char *binary, int size) {
	if(this->cfg_) {
		delete this->cfg_;
	}
	liveLocations_.clear();
	
	this->cfg_ = new CFG();
	this->cfg_->generate(address, binary, size);
	this->analyse();
}
