#include <string.h>

#include "Instruction.h"

Instruction::Instruction() {
	this->address_ = 0;
	this->assembly_ = 0;
	this->binary_ = 0;
	this->binarySize_ = 0;
}

Instruction::Instruction(uint32_t address, const char *assembly, const char *binary, int binarySize) {
	this->address_ = 0;
	this->assembly_ = 0;
	this->binary_ = 0;
	this->binarySize_ = 0;
	this->address_ = address;
	this->setAssembly(assembly);
	this->setBinary(binary, binarySize);
}

Instruction::~Instruction() {
	if(this->assembly_) {
		delete[] this->assembly_;
	}
	
	if(this->binary_) {
		delete[] this->binary_;
	}
}

void Instruction::setAddress(uint32_t address) {
	this->address_ = address;
}

uint32_t Instruction::getAddress() const {
	return this->address_;
}

void Instruction::setAssembly(const char *assembly) {
	if(this->assembly_) {
		delete[] this->assembly_;
	}
	
	this->assembly_ = new char[strlen(assembly) + 1];
	memcpy(this->assembly_, assembly, strlen(assembly));
	this->assembly_[strlen(assembly)] = '\0';
}

void Instruction::setBinary(const char *binary, int binarySize) {
	if(this->binary_) {
		delete[] this->binary_;
	}
	
	this->binary_ = new char[binarySize];
	this->binarySize_ = binarySize;
	memcpy(this->binary_, binary, binarySize);
}

const char *Instruction::getAssembly() const {
	return this->assembly_;
}

const char *Instruction::getBinary() const {
	return this->binary_;
}
int Instruction::getBinarySize() const {
	return this->binarySize_;
}
