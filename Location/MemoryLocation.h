#ifndef MEMORY_LOCATION_H
#define MEMORY_LOCATION_H

#include <stdint.h>
#include <memory>
#include <vector>

extern "C" {
#include <tracer/trace.h>
}

#include "Location.h"
#include "RegisterLocation.h"

class MemoryLocation : public Location {
public:
	MemoryLocation();
	MemoryLocation(RegisterLocationPtr &base, RegisterLocationPtr &index, int32_t offset);
	MemoryLocation(RegisterLocationPtr &base, RegisterLocationPtr &index, int32_t offset, uint32_t scale);
	~MemoryLocation();
	
	virtual int compare(const Location *location) const;
	virtual void setValue(trace_t *client, uint32_t value) const;
	virtual void print() const;
	virtual void print(FILE *output) const;
	
	void setBase(RegisterLocationPtr &base);
	void setIndex(RegisterLocationPtr &index);
	void setOffset(int32_t offset);
	void setScale(uint32_t scale);
	
	RegisterLocationPtr &getBase();
	RegisterLocationPtr &getIndex();
	int32_t getOffset() const;
	uint32_t getScale() const;
private:
	int32_t getRegisterSignedValue(registers_t *registers, const RegisterLocation *reg) const;
	uint32_t getRegisterUnsignedValue(registers_t *registers, const RegisterLocation *reg) const;
	
private:
	RegisterLocationPtr base_;
	RegisterLocationPtr index_;
	int32_t offset_;
	uint32_t scale_;
	char buffer[1024];
};

typedef std::shared_ptr<MemoryLocation> MemoryLocationPtr;

#endif
