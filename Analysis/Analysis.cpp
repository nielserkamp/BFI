#include "Analysis.h"


std::map<Address, LivePtr> &Analysis::getLiveLocations() {
	return this->liveLocations_;
}

void Analysis::removeRegister(LivePtr &live, ud_type_t regLocation) {
	if(regLocation == UD_R_EAX) {
		removeRegister(live, UD_R_AH);
		removeRegister(live, UD_R_AL);
		removeRegister(live, UD_R_AX);
	} else if(regLocation == UD_R_EBX) {
		removeRegister(live, UD_R_BH);
		removeRegister(live, UD_R_BL);
		removeRegister(live, UD_R_BX);
	} else if(regLocation == UD_R_ECX) {
		removeRegister(live, UD_R_CH);
		removeRegister(live, UD_R_CL);
		removeRegister(live, UD_R_CX);
	} else if(regLocation == UD_R_EDX) {
		removeRegister(live, UD_R_DH);
		removeRegister(live, UD_R_DL);
		removeRegister(live, UD_R_DX);
	} else if(regLocation == UD_R_ESI) {
		removeRegister(live, UD_R_SI);
	} else if(regLocation == UD_R_EDI) {
		removeRegister(live, UD_R_DI);
	} else if(regLocation == UD_R_ESP) {
		removeRegister(live, UD_R_SP);
	} else if(regLocation == UD_R_ESP) {
		removeRegister(live, UD_R_SP);
	} else if(regLocation == UD_R_EBP) {
		removeRegister(live, UD_R_BP);
	}
	
	LocationPtr loc(new RegisterLocation(regLocation));
	live->removeLocation(loc);
}

void Analysis::addRegister(LivePtr &live, ud_type_t regLocation) {
	if(regLocation == UD_R_EAX) {
		removeRegister(live, UD_R_AH);
		removeRegister(live, UD_R_AL);
		removeRegister(live, UD_R_AX);
	} else if(regLocation == UD_R_EBX) {
		removeRegister(live, UD_R_BH);
		removeRegister(live, UD_R_BL);
		removeRegister(live, UD_R_BX);
	} else if(regLocation == UD_R_ECX) {
		removeRegister(live, UD_R_CH);
		removeRegister(live, UD_R_CL);
		removeRegister(live, UD_R_CX);
	} else if(regLocation == UD_R_EDX) {
		removeRegister(live, UD_R_DH);
		removeRegister(live, UD_R_DL);
		removeRegister(live, UD_R_DX);
	} else if(regLocation == UD_R_ESI) {
		removeRegister(live, UD_R_SI);
	} else if(regLocation == UD_R_EDI) {
		removeRegister(live, UD_R_DI);
	} else if(regLocation == UD_R_ESP) {
		removeRegister(live, UD_R_SP);
	} else if(regLocation == UD_R_ESP) {
		removeRegister(live, UD_R_SP);
	} else if(regLocation == UD_R_EBP) {
		removeRegister(live, UD_R_BP);
	}
	
	LocationPtr loc(new RegisterLocation(regLocation));
	for(size_t i = 0; i < live->getLocations().size(); i++) {
		if(live->getLocations()[i]->compare(loc.get()) == 0) {
			return;
		}
	}
	live->addLocation(loc);
}


void Analysis::addMemory(LivePtr &live, uint64_t offset, ud_type_t baseLocation, ud_type_t indexLocation) {
	RegisterLocationPtr baseLoc(new RegisterLocation(baseLocation));
	RegisterLocationPtr indexLoc(new RegisterLocation(indexLocation));
	
	LocationPtr loc(new MemoryLocation(baseLoc, indexLoc, offset, 0));
	for(size_t i = 0; i < live->getLocations().size(); i++) {
		if(live->getLocations()[i]->compare(loc.get()) == 0) {
			return;
		}
	}
	live->addLocation(loc);
}

void Analysis::removeMemory(LivePtr &live, ud_type_t regLocation) {
	LocationPtr loc(new RegisterLocation(regLocation));
	MemoryLocation *mLocation;
	for(int i = live->getLocations().size() - 1; i >= 0; i--) {
		Location *l = live->getLocations()[i].get();
		mLocation = dynamic_cast<MemoryLocation *>(l);
	  	if(!mLocation) {
	  		continue;
	  	}
	  	
	  	if(mLocation->getBase()->compare(loc.get()) == 0 ||
	  		mLocation->getIndex()->compare(loc.get()) == 0
	  	){ 
	  		LocationPtr mLoc(live->getLocations()[i]);
	  		live->removeLocation(mLoc);
	  	}
	}
}

void Analysis::removeMemory(LivePtr &live, uint64_t offset, ud_type_t base, ud_type_t index) {
	RegisterLocationPtr baseLoc(new RegisterLocation(base));
	RegisterLocationPtr indexLoc(new RegisterLocation(index));
	LocationPtr loc(new MemoryLocation(baseLoc, indexLoc, offset, 0));
	

	live->removeLocation(loc);
}

uint64_t Analysis::getOffset(ud_operand_t *operand) {
	uint64_t offset;
	switch(operand->offset){
		case 8:
			offset = (uint64_t)operand->lval.sbyte;
			break;
		case 16:
			offset = (uint64_t)operand->lval.sword;
			break;
		case 32:
			offset = (uint64_t)operand->lval.sdword;
			break;
		default:
			offset = 0;
	}
	return offset;
}
