#ifndef LOCATION_H
#define LOCATION_H

#include <stdio.h>
#include <stdint.h>

#include <typeinfo>
#include <exception>
#include <vector>
#include <memory>

extern "C" {
#include <tracer/trace.h>
}

class Location {
public:
	typedef std::vector<Location *> List;

	struct Compare {
		inline bool operator()(const Location *left, const Location *right) {
			if(typeid(*left) == typeid(*right)) {
				return left->compare(right) < 0;
			}
			
			return false;
		}
	};
	
	~Location() {}
	
	virtual int compare(const Location *location) const = 0;
	virtual void setValue(trace_t *client, uint32_t value) const = 0;
	virtual void print() const = 0;
	virtual void print(FILE *output) const = 0;
	
private:
	
};

typedef std::shared_ptr<Location> LocationPtr;


#endif
