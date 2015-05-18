#include <string.h>

extern "C" {
#include <tracer/util.h>
}

#include "InstructionLocation.h"

InstructionLocation::InstructionLocation() {
	this->address_ = 0;
	this->size_ = 4;
}

InstructionLocation::InstructionLocation(uint32_t address) {
	this->address_ = address;
	this->size_ = 4;
}

InstructionLocation::InstructionLocation(uint32_t address, int size) {
	this->address_ = address;
	this->size_ = size;
}

InstructionLocation::~InstructionLocation() {

}

void InstructionLocation::print() const {
	this->print(stdout);
}

void InstructionLocation::print(FILE *output) const {
	fprintf(output, "%x", this->address_);
}

int InstructionLocation::compare(const Location *location) const {
	const InstructionLocation *mLocation;
	mLocation = dynamic_cast<const InstructionLocation *>(location);
  	if(!mLocation) {
  		return strcmp(typeid(this).name(), typeid(*location).name());
  	}
  	
	if(this->address_ != mLocation->address_) {
		if(this->address_ < mLocation->address_) {
			return -1;
		}
		
		return 1;
	}
	
	return 0;
}

void InstructionLocation::setValue(trace_t *client, uint32_t value) const {
	if(memstore(client->pid, (void *)&value, (void *)(this->address_), this->size_) == -1){
		fprintf(stderr, "[+] Failed to store value %x memory parameter\n", value);
	}
}
