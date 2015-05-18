#include "memcmpTestCase.h"

memcmpTestCase::memcmpTestCase() {
	this->numOfTestCases_ = 4;
	this->numOfParameters_ = 3;
	this->instanceNumber_ = 0;
}

memcmpTestCase::~memcmpTestCase() {
	
}

void memcmpTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09", 10));
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x0A", 10));
	parameters.push_back((void *)10);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "\xFF\xFF\x10\xFF\xAB", 5));
	parameters.push_back((void *)this->storeData(t, "\xFF\xFF\x00\xFF\xAB", 5));
	parameters.push_back((void *)5);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x00\x05\x06\x07\x08\x09", 10));
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x01\x05\x06\x07\x08\x09", 10));
	parameters.push_back((void *)4);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x00\x05\x06\x07\x08\x09", 10));
	parameters.push_back((void *)this->storeData(t, "\x00\x01\x02\x03\x00\x05\x06\x08\x09\x07", 10));
	parameters.push_back((void *)7);
	this->testCaseParameters_.push_back(parameters);
}

void memcmpTestCase::getPossibleResults(trace_t *t,  LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	
	for(int i = 0; i < live->getLocations().size(); i++) {
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
		} else if(value == 0 && testID == 3) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 3;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		}
	}
}
