#include <string.h>

extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "sprintfTestCase.h"

sprintfTestCase::sprintfTestCase() {
	this->numOfTestCases_ = 3;
	this->numOfParameters_ = 3;
}

sprintfTestCase::~sprintfTestCase() {
	
}

void sprintfTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	uint64_t dst = this->storeData(t, "NOOOOO THE FILLER BUFFER IN WHICH EVERYTHING WILL BE COPIED", 60);
	
	this->resultStrings_[0] = (char *)"First TestCase for sprintf\x0a";
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, "%s\n", 3));
	parameters.push_back((void *)this->storeData(t, "First TestCase for sprintf", 27));
	this->testCaseParameters_.push_back(parameters);
	
	this->resultStrings_[1] = (char *)"An integer: 31337\x0a";
	parameters.clear();
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, "An integer: %d\n", 16));
	parameters.push_back((void *)31337);
	this->testCaseParameters_.push_back(parameters);
	
	this->resultStrings_[2] = (char *)"A hex: 0xdeadbeaf\x0a";
	parameters.clear();
	uint32_t h = 0xDEADBEAF;
	parameters.push_back((void *)dst);
	parameters.push_back((void *)this->storeData(t, "A hex: 0x%x\n", 14));
	parameters.push_back((void *)*((int *)&h));
	this->testCaseParameters_.push_back(parameters);
}

void sprintfTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	if(memload(t->pid, (void *)buffer, (void *)(this->testCaseParameters_[testID][0]), strlen(this->resultStrings_[testID])) == -1) {
		return;
	}

	if(memcmp((void *)buffer, (void *)this->resultStrings_[testID], strlen(this->resultStrings_[testID])) == 0) {
		TestCase::Result *testResult = new TestCase::Result();
	
		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
	
		instance->results[testID].push_back(testResult);
	}
}
