#include <stdio.h>
#include <string.h>

extern "C" {
#include <tracer/util.h>
}

#include "MemoryLocation.h"
#include "LivenessArray.h"

MemoryLocation::MemoryLocation() {
	this->base_ = RegisterLocationPtr(new RegisterLocation(UD_NONE));
	this->index_ = RegisterLocationPtr(new RegisterLocation(UD_NONE));
	this->scale_ = 0;
	this->offset_ = 0;
}

MemoryLocation::MemoryLocation(RegisterLocationPtr &base, RegisterLocationPtr &index, int32_t offset) {
	this->base_ = base;
	this->index_ = index;
	this->scale_ = 0;
	this->offset_ = offset;
}

MemoryLocation::MemoryLocation(RegisterLocationPtr &base, RegisterLocationPtr &index, int32_t offset, uint32_t scale) {
	this->base_ = base;
	this->index_ = index;
	this->scale_ = scale;
	this->offset_ = offset;
}

MemoryLocation::~MemoryLocation() {

}

void MemoryLocation::print() const {
	this->print(stdout);
}

void MemoryLocation::print(FILE *output) const {
	if(((int)this->offset_) < 0) {
		fprintf(output, "-%x(%s, %s, %x)",
			this->offset_ * -1, 
			Tephon::reg_names[this->base_->getRegister()], 
			Tephon::reg_names[this->index_->getRegister()], 
			this->scale_
		);
	} else {
		fprintf(output, "%x(%s, %s, %x)",
			this->offset_, 
			Tephon::reg_names[this->base_->getRegister()], 
			Tephon::reg_names[this->index_->getRegister()], 
			this->scale_
		);
	}
}

void MemoryLocation::setBase(RegisterLocationPtr &base) {
	this->base_ = base;
}

void MemoryLocation::setIndex(RegisterLocationPtr &index) {
	this->index_ = index;
}

void MemoryLocation::setOffset(int32_t offset) {
	this->offset_ = offset;
}

void MemoryLocation::setScale(uint32_t scale) { 
	this->scale_ = scale;
}

RegisterLocationPtr &MemoryLocation::getBase() {
	return this->base_;
}

RegisterLocationPtr &MemoryLocation::getIndex() { 
	return this->index_;
}

int32_t MemoryLocation::getOffset() const {
	return this->offset_;
}

uint32_t MemoryLocation::getScale() const {
	return this->scale_;
}

int MemoryLocation::compare(const Location *location) const {
	const MemoryLocation *mLocation;
	mLocation = dynamic_cast<const MemoryLocation *>(location);
  	if(!mLocation) {
  		return strcmp(typeid(this).name(), typeid(*location).name());
  	}
  	
	if(this->base_->compare(mLocation->base_.get()) != 0) {
		return this->base_->compare(mLocation->base_.get());
	}
	
	if(this->index_->compare(mLocation->index_.get())) {
		return this->index_->compare(mLocation->index_.get());
	}
	
	if(this->offset_ != mLocation->offset_) {
		if(this->offset_ < mLocation->offset_) {
			return -4;
		}
		
		return 5;
	}
	
	return 0;
}

void MemoryLocation::setValue(trace_t *client, uint32_t value) const {
	uint32_t base = this->getRegisterUnsignedValue(&client->regs, this->base_.get());
	uint32_t memoryAddress = base + this->offset_;
	
	if(memstore(client->pid, (void *)&value, (void *)(memoryAddress), sizeof(uint32_t)) == -1){
		fprintf(stderr, "[+] Failed to store value %x memory parameter at %x for eip %lx\n", value, memoryAddress, client->regs.eip);
	}
}

int32_t MemoryLocation::getRegisterSignedValue(registers_t *registers, const RegisterLocation *reg) const {
	switch(reg->getRegister()){
	    case UD_R_EAX:
	        return (int64_t)registers->eax;
	    case UD_R_EBX:
	        return (int64_t)registers->ebx;
	    case UD_R_ECX:
	        return (int64_t)registers->ecx;
	    case UD_R_EDX:
	        return (int64_t)registers->edx;
	    case UD_R_ESI:
	        return (int64_t)registers->esi;
	    case UD_R_EDI:
	        return (int64_t)registers->edi;
	    case UD_R_EBP:
	        return (int64_t)registers->ebp;
	    case UD_R_ESP:
	        return (int64_t)registers->esp;
	    case UD_R_RIP:
	        return (int64_t)registers->eip;
	    default:
	    	return 0;
	}
}

uint32_t MemoryLocation::getRegisterUnsignedValue(registers_t *registers, const RegisterLocation *reg) const {
	switch(reg->getRegister()){
	    case UD_R_EAX:
	        return (uint64_t)registers->eax;
	    case UD_R_EBX:
	        return (uint64_t)registers->ebx;
	    case UD_R_ECX:
	        return (uint64_t)registers->ecx;
	    case UD_R_EDX:
	        return (uint64_t)registers->edx;
	    case UD_R_ESI:
	        return (uint64_t)registers->esi;
	    case UD_R_EDI:
	        return (uint64_t)registers->edi;
	    case UD_R_EBP:
	        return (uint64_t)registers->ebp;
	    case UD_R_ESP:
	        return (uint64_t)registers->esp;
	    case UD_R_RIP:
	        return (uint64_t)registers->eip;
	    default:
	    	return 0;
	}
}


