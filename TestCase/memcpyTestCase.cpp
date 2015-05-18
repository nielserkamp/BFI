#include <string.h>

extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "memcpyTestCase.h"

memcpyTestCase::memcpyTestCase() {
	this->numOfTestCases_ = 3;
	this->numOfParameters_ = 3;
}

memcpyTestCase::~memcpyTestCase() {
	
}

void memcpyTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	uint64_t dst = this->storeData(t, "NOOOOO THE FILLER BUFFER IN WHICH EVERYTHING WILL BE COPIED", 60);
	this->dstStrings_[0] = (char *)"AAAA\0AAAA";
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, dstStrings_[0], 9));
	parameters.push_back((void *)9);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	this->dstStrings_[1] = (char *)"THIS IS A LONG SENTENCE IT WILL BE COPIED I HOPE";
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, dstStrings_[1], 49));
	parameters.push_back((void *)49);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	this->dstStrings_[2] = (char *)"\x98\x01\x02\x03\x04\x05\xFF";
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, dstStrings_[2] , 7));
	parameters.push_back((void *)7);
	this->testCaseParameters_.push_back(parameters);
}

void memcpyTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	if(memload(t->pid, (void *)buffer, (void *)(this->testCaseParameters_[testID][0]), (size_t)this->testCaseParameters_[testID][2]) == -1) {
		return;
	}
	
	if(memcmp((void *)buffer, (void *)this->dstStrings_[testID], (size_t)this->testCaseParameters_[testID][2]) == 0) {
		TestCase::Result *testResult = new TestCase::Result();
		
		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
		
		instance->results[testID].push_back(testResult);
	}
}
