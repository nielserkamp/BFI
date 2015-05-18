#ifndef REGISTER_LOCATION_H
#define REGISTER_LOCATION_H

#include <vector>

extern "C" {
#include <udis86/udis86.h>
#include <tracer/trace.h>
}

#include "Location.h"


class RegisterLocation : public Location {
public:
	RegisterLocation();
	RegisterLocation(ud_type_t reg);
	~RegisterLocation();
	
	virtual int compare(const Location *location) const;
	virtual void setValue(trace_t *client, uint32_t value) const;
	virtual void print() const;
	virtual void print(FILE *output) const;
	
	void setRegister(ud_type_t reg);
	
	ud_type_t getRegister() const;

private:
	void setRegister(registers_t *registers, uint32_t value) const;

private:
	ud_type_t register_;
};

typedef std::shared_ptr<RegisterLocation> RegisterLocationPtr;

#endif
