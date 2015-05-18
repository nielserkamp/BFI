#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <map>
#include <memory>

extern "C" {
#include <udis86/udis86.h>
}

#include "Location/Location.h"
#include "Location/RegisterLocation.h"
#include "Location/MemoryLocation.h"
#include "Location/InstructionLocation.h"
#include "Address.h"
#include "Live.h"

class Analysis {
public:
	std::map<Address, LivePtr> &getLiveLocations();
protected:
	std::map<Address, LivePtr> liveLocations_;
	
	void removeRegister(LivePtr &live, ud_type_t regLocation);	
	void addRegister(LivePtr &live, ud_type_t regLocation);
	void removeMemory(LivePtr &live, ud_type_t regLocation);
	uint64_t getOffset(ud_operand_t *operand);
	void removeMemory(LivePtr &live, uint64_t offset, ud_type_t base, ud_type_t index);
	void addMemory(LivePtr &live, uint64_t offset, ud_type_t baseLocation, ud_type_t indexLocation);
};


#endif
