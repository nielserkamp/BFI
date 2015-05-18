#include "strcmpTestCase.h"

strcmpTestCase::strcmpTestCase() {
	this->numOfTestCases_ = 3;
	this->numOfParameters_ = 2;
	this->instanceNumber_ = 0;
}

strcmpTestCase::~strcmpTestCase() {
	
}

void strcmpTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	parameters.push_back((void *)this->storeData(t, "AAAAAAAAAAAA", 13));
	parameters.push_back((void *)this->storeData(t, "AAAAAAAAAAAB", 13));
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "BBBB", 5));
	parameters.push_back((void *)this->storeData(t, "AAAA", 5));
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "0123456789", 11));
	parameters.push_back((void *)this->storeData(t, "0123456789", 11));
	this->testCaseParameters_.push_back(parameters);
}

void strcmpTestCase::getPossibleResults(trace_t *t,  LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	
	for(size_t i = 0; i < live->getLocations().size(); i++) {
		RegisterLocation *rLocation;
		rLocation = dynamic_cast<RegisterLocation *>(live->getLocations()[i].get());
	  	if(!rLocation) {
	  		continue;
	  	}
	
		int value = (int)this->getRegister(&t->regs, rLocation->getRegister());
		
		if(value < 0 && testID == 0) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 0;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value > 0 && testID == 1) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 1;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 0 && testID == 2) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 2;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		}
	}
}
