#include <stdio.h>
#include <string.h>

#include "RegisterLocation.h"
#include "LivenessArray.h"

RegisterLocation::RegisterLocation() {
	this->register_ = UD_NONE;
}

RegisterLocation::RegisterLocation(ud_type_t reg) {
	this->register_ = reg;
}

RegisterLocation::~RegisterLocation() {

}

void RegisterLocation::print() const {
	this->print(stdout);
}

void RegisterLocation::print(FILE *output) const {
	fprintf(output, "%s", Tephon::reg_names[this->register_]);
}

int RegisterLocation::compare(const Location *location) const {
	const RegisterLocation *mLocation;
	mLocation = dynamic_cast<const RegisterLocation *>(location);
  	if(!mLocation) {
  		return strcmp(typeid(this).name(), typeid(*location).name());
  	}
  	
	if(this->register_ != mLocation->register_) {
		if(this->register_ < mLocation->register_) {
			return -2;
		}
		
		return 3;
	}
	
	return 0;
}

void RegisterLocation::setValue(trace_t *client, uint32_t value) const {
	this->setRegister(&client->regs, value);
}

ud_type_t RegisterLocation::getRegister() const {
	return this->register_;
}

void RegisterLocation::setRegister(ud_type_t reg) {
	this->register_ = reg;
}

void RegisterLocation::setRegister(registers_t *registers, uint32_t value) const {
	ud_type_t reg = this->register_;
	if(reg == UD_R_EAX || reg == UD_R_AL || reg == UD_R_AH || reg == UD_R_AX) {
		registers->eax = value;
	} else if(reg == UD_R_EBX || reg == UD_R_BL || reg == UD_R_BH || reg == UD_R_BX) {
		registers->ebx = value;
	} else if(reg == UD_R_ECX || reg == UD_R_CL || reg == UD_R_CH || reg == UD_R_CX) {
		registers->ecx = value;
	} else if(reg == UD_R_EDX || reg == UD_R_DL || reg == UD_R_DH || reg == UD_R_DX) {
		registers->edx = value;
	} else if(reg == UD_R_ESI || reg == UD_R_SI) {
		registers->esi = value;
	} else if(reg == UD_R_EDI || reg == UD_R_DI) {
		registers->edi = value;
	} else if(reg == UD_R_EBP || reg == UD_R_BP) {
		registers->ebp = value;
	} else if(reg == UD_R_ESP || reg == UD_R_SP) {
		registers->esp = value;
	} else if(reg == UD_R_RIP) {
		registers->eip = value;
	}
}
