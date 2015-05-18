#include <stdlib.h>
#include <set>
#include <algorithm>

extern "C" {
#include <udis86/udis86.h>
}

#include "CFG.h"

CFGNode::CFGNode() {
	this->label_ = 0;
}

CFGNode::CFGNode(Address label) {
	this->label_ = label;
}

CFGNode::~CFGNode() {
	
}

void CFGNode::setLabel(Address label) {
	this->label_ = label;
}

Address CFGNode::getLabel() {
	return this->label_;
}

void CFGNode::addPrevNode(std::weak_ptr<CFGNode> &node) {
	this->prevNodes_.push_back(node);
}

void CFGNode::addNextNode(std::weak_ptr<CFGNode> &node) {
	this->nextNodes_.push_back(node);
}

void CFGNode::addInstruction(InstructionPtr &instruction) {
	this->instructions_.push_back(instruction);
}

std::vector<std::weak_ptr<CFGNode> > &CFGNode::getPrevNodes() {
	return this->prevNodes_;
}

std::vector<std::weak_ptr<CFGNode> > &CFGNode::getNextNodes() {
	return this->nextNodes_;
}

std::vector<InstructionPtr> &CFGNode::getInstructions() {
	return this->instructions_;
}

std::vector<Address> &CFG::getLabels() {
	return this->labels_;
}

CFG::CFG() {
	
}

CFG::~CFG() {

}

void CFG::addInstruction(uint32_t address, InstructionPtr &instruction) {
	this->instructions_[address] = instruction;
}

std::map<Address, InstructionPtr> &CFG::getInstructions() {
	return this->instructions_;
}

std::vector<CFGNodePtr> &CFG::getNodes() {
	return this->nodes_;
}

std::map<Address, CFGNodePtr> &CFG::getNodesMap() {
	return this->nodesMap_;
}

std::map<Address, CFGNodePtr> &CFG::getLabelMap() {
	return this->labelMap_;
}

void CFG::generate() {
	ud_t ud_obj;
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_syntax(&ud_obj, UD_SYN_ATT);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	
	// Labels
	std::map<Address, InstructionPtr>::iterator itInstruction;
	
	std::set<Address> labels;
	
	labels.insert(this->instructions_.begin()->first);
	for(itInstruction = this->instructions_.begin(); itInstruction != this->instructions_.end(); itInstruction++) {
		ud_set_pc(&ud_obj, itInstruction->first);
		ud_set_input_buffer(&ud_obj, (uint8_t *)itInstruction->second->getBinary(), itInstruction->second->getBinarySize());
		if(!ud_disassemble(&ud_obj)) {
			fprintf(stderr, "Error: Failed to disassemble\n");
			exit(1);
		}
		
		if(ud_obj.operand[0].type == UD_OP_JIMM){
			int offset = 0;
			switch(ud_obj.operand[0].size) {
				case 8:
					offset = (int)ud_obj.operand[0].lval.sbyte;
					break;
				case 16:
					offset = (int)ud_obj.operand[0].lval.sword;
					break;
				case 32:
					offset = (int)ud_obj.operand[0].lval.sdword;
					break;

			};
			
			labels.insert(ud_obj.pc + offset);
			labels.insert(ud_obj.pc);
		}
		
		if(ud_obj.mnemonic == UD_Iret || ud_obj.mnemonic == UD_Ihlt){
			labels.insert(ud_obj.pc);
		}
		
		ud_obj.operand[0].lval.uqword = 0;
	}
	
	std::set<uint32_t>::iterator itL;
	for(itL = labels.begin(); itL != labels.end(); itL++) {
		this->labels_.push_back(*itL);
	}
	
	// Nodes
	std::vector<Address>::iterator itLabels;
	for(itLabels = this->labels_.begin(); itLabels != this->labels_.end(); itLabels++) {
		uint32_t address = *itLabels;
		
		if(this->instructions_.find(address) == this->instructions_.end()) {
			continue;
		}
		
		CFGNodePtr node(new CFGNode(address));
		node->addInstruction(this->instructions_[address]);
		address += this->instructions_[address]->getBinarySize();
		labelMap_[address] = node;
		
		while(std::find(labels_.begin(), labels_.end(), address) == labels_.end() 
			&& instructions_.find(address) != instructions_.end()) {
			node->addInstruction(this->instructions_[address]);
			address += this->instructions_[address]->getBinarySize();
			this->labelMap_[address] = node;
		}
		
		this->nodes_.push_back(node);
		this->nodesMap_[node->getLabel()] = node;
	}
	
	// Links
	std::vector<CFGNodePtr>::iterator itNodes;
	for(itNodes = this->nodes_.begin(); itNodes != this->nodes_.end(); itNodes++) {
		CFGNodePtr node = *itNodes;
		std::weak_ptr<CFGNode> wNode(node);
		InstructionPtr instruction = node->getInstructions()[node->getInstructions().size() - 1];
		
		ud_set_pc(&ud_obj, instruction->getAddress());
		ud_set_input_buffer(&ud_obj, (uint8_t *)instruction->getBinary(), instruction->getBinarySize());
		if(!ud_disassemble(&ud_obj)) {
			fprintf(stderr, "Error: Failed to disassemble\n");
			exit(1);
		}
		
		uint32_t rel = 0;
		switch(ud_obj.operand[0].size) {
			case 8:
				rel = (uint32_t)ud_obj.operand[0].lval.sbyte;
				break;
			case 16:
				rel = (uint32_t)ud_obj.operand[0].lval.sword;
				break;
			case 32:
				rel = (uint32_t)ud_obj.operand[0].lval.sdword;
				break;

		};
		
		if(ud_obj.mnemonic == UD_Icall) {
			if(this->nodesMap_.find((uint32_t)ud_obj.pc) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)ud_obj.pc];
				std::weak_ptr<CFGNode> wNextNode(nextNode);

				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}

			if(this->nodesMap_.find((uint32_t)ud_obj.pc + rel) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)(ud_obj.pc + rel)];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
		} else if(ud_obj.mnemonic == UD_Ijmp) {
			if(this->nodesMap_.find((uint32_t)ud_obj.pc) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)ud_obj.pc];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
			
			if(this->nodesMap_.find((uint32_t)(ud_obj.pc + rel)) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)(ud_obj.pc + rel)];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
			
		} else if(ud_obj.mnemonic == UD_Iret) {
			// DO NOTHING
		} else if(ud_obj.operand[0].type == UD_OP_JIMM) {
			if(this->nodesMap_.find((uint32_t)(ud_obj.pc + rel)) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)(ud_obj.pc + rel)];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
			
			if(this->nodesMap_.find((uint32_t)ud_obj.pc) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)ud_obj.pc];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
		} else {
			if(this->nodesMap_.find((uint32_t)ud_obj.pc) != this->nodesMap_.end()) {
				CFGNodePtr nextNode = this->nodesMap_[(uint32_t)ud_obj.pc];
				std::weak_ptr<CFGNode> wNextNode(nextNode);
				
				nextNode->addPrevNode(wNode);
				node->addNextNode(wNextNode);
			}
		}
	}

}
	
void CFG::generate(int32_t address, const char *buffer, int size) {
	ud_t ud_obj;
	ud_init(&ud_obj);
	ud_set_pc(&ud_obj, address);
	ud_set_syntax(&ud_obj, UD_SYN_ATT);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_mode(&ud_obj, 32);
	ud_set_input_buffer(&ud_obj, (uint8_t *)buffer, size);

	while(ud_disassemble(&ud_obj) && ud_obj.mnemonic != UD_Iinvalid) {
		InstructionPtr instruction(new Instruction(
			(uint64_t)(ud_obj.pc - ud_insn_len(&ud_obj)), 
			ud_insn_asm(&ud_obj), 
			(const char *)ud_insn_ptr(&ud_obj), 
			(int)ud_insn_len(&ud_obj)
		));
	
		this->instructions_[instruction->getAddress()] = instruction;
	}

	this->generate();
}

