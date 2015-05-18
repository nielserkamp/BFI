extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "strtolTestCase.h"

strtolTestCase::strtolTestCase() {
	this->numOfTestCases_ = 3;
	this->numOfParameters_ = 3;
	this->instanceNumber_ = 0;
}

strtolTestCase::~strtolTestCase() {
	
}

void strtolTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	parameters.push_back((void *)this->storeData(t, "1337", 5));
	parameters.push_back((void *)0);
	parameters.push_back((void *)10);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "0xdeadbeaf", 11));
	parameters.push_back((void *)0);
	parameters.push_back((void *)16);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)((uint32_t)this->storeData(t, "111 1000", 9)));
	parameters.push_back((void *)(((uint32_t)parameters[0]) + 9));
	parameters.push_back((void *)2);
	this->testCaseParameters_.push_back(parameters);
}

void strtolTestCase::getPossibleResults(trace_t *t,  LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	
	for(int i = 0; i < live->getLocations().size(); i++) {
		RegisterLocation *rLocation;
		rLocation = dynamic_cast<RegisterLocation *>(live->getLocations()[i].get());
	  	if(!rLocation) {
	  		continue;
	  	}
	
		int value = (int)this->getRegister(&t->regs, rLocation->getRegister());
		
		if(value == 1337 && testID == 0) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 0;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 0x7fffffff && testID == 1) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 1;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 7 && testID == 2) {
			uint32_t buffer;
			if(memload(t->pid, (void *)&buffer, (void *)(this->testCaseParameters_[testID][1]), (size_t)4) == -1) {
				continue;
			}
			if(buffer != (uint32_t)this->testCaseParameters_[testID][0] + 3) {
				continue;
			}
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 2;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		}
	}
}
