#ifndef INSTRUCTION_LOCATION_H
#define INSTRUCTION_LOCATION_H

#include <stdint.h>

extern "C" {
#include <tracer/trace.h>
}

#include "Location.h"


class InstructionLocation : public Location {
public:
	InstructionLocation();
	InstructionLocation(uint32_t address);
	InstructionLocation(uint32_t address, int size);
	~InstructionLocation();
	
	int compare(const Location *location) const;
	void setValue(trace_t *client, uint32_t value) const;
	void print() const;
	void print(FILE *output) const;

private:
	uint32_t address_;
	int size_;
};

#endif
