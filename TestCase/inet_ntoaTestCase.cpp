#include <string.h>

extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "inet_ntoaTestCase.h"

inet_ntoaTestCase::inet_ntoaTestCase() {
	this->numOfTestCases_ = 4;
	this->numOfParameters_ = 1;
}

inet_ntoaTestCase::~inet_ntoaTestCase() {
	
}

void inet_ntoaTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	parameters.push_back((void *)0xFFFFFFFF);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)0xFF00FF00);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)0xAB21AABB);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)0xCCBBAA99);
	this->testCaseParameters_.push_back(parameters);
}

void inet_ntoaTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	for(int i = 0; i < live->getLocations().size(); i++) {
		RegisterLocation *rLocation;
		rLocation = dynamic_cast<RegisterLocation *>(live->getLocations()[i].get());
	  	if(!rLocation) {
	  		continue;
	  	}
	
		uint32_t addr = this->getRegister(&t->regs, rLocation->getRegister());
		
		if(memload(t->pid, (void *)buffer, (void *)addr, 16) == -1) {
			return;
		}
		
		if(testID == 0) {
			if(strncmp(buffer, "255.255.255.255", 16) == 0) {
				TestCase::Result *testResult = new TestCase::Result();
	
				testResult->testID = testID;
				testResult->address = address;
				testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
	
				instance->results[testID].push_back(testResult);
			}
		} else if(testID == 1) {
			if(strncmp(buffer, "0.255.0.255", 16) == 0) {
				TestCase::Result *testResult = new TestCase::Result();
	
				testResult->testID = testID;
				testResult->address = address;
				testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
	
				instance->results[testID].push_back(testResult);
			}
		} else if(testID == 2) {
			if(strncmp(buffer, "187.170.33.171", 16) == 0) {
				TestCase::Result *testResult = new TestCase::Result();
	
				testResult->testID = testID;
				testResult->address = address;
				testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
	
				instance->results[testID].push_back(testResult);
			}
		} else if(testID == 3) {
			if(strncmp(buffer, "153.170.187.204", 16) == 0) {
				TestCase::Result *testResult = new TestCase::Result();
	
				testResult->testID = testID;
				testResult->address = address;
				testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
	
				instance->results[testID].push_back(testResult);
			}
		}	
	}
}
