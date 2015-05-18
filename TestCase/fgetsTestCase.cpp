#include <string.h>

extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "fgetsTestCase.h"

fgetsTestCase::fgetsTestCase() {
	this->numOfTestCases_ = 3;
	this->numOfParameters_ = 3;
}

fgetsTestCase::~fgetsTestCase() {
	
}

void fgetsTestCase::activateTestCase(trace_t *t, Info *info) {
	this->testCaseParameters_[0][2] = info->fileReadA;
	this->testCaseParameters_[1][2] = info->fileReadB;
	this->testCaseParameters_[2][2] = info->fileReadC;
	
	TestCase::activateTestCase(t);
}

void fgetsTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	uint32_t dst = this->storeData(t, "xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx", 58);
	
	parameters.push_back((void *)dst);
	parameters.push_back((void *)30);
	parameters.push_back((void *)0);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)dst);
	parameters.push_back((void *)30);
	parameters.push_back((void *)0);
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	parameters.push_back((void *)dst);
	parameters.push_back((void *)46);
	parameters.push_back((void *)0);
	this->testCaseParameters_.push_back(parameters);
	
}

void fgetsTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	buffer[0] = 0;
	
	if(memload(t->pid, (void *)buffer, (void *)this->testCaseParameters_[testID][0], 46) == -1) {
		return;
	}
	buffer[45] = 0;
	
	printf("%s\n", buffer);

	if(strncmp(buffer, "A string the size of 24", 23) == 0) {
		TestCase::Result *testResult = new TestCase::Result();

		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));

		instance->results[testID].push_back(testResult);
	} else if(strncmp(buffer, "File C contains some info and", 29) == 0) {
		TestCase::Result *testResult = new TestCase::Result();

		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));

		instance->results[testID].push_back(testResult);
	} else if(strncmp(buffer, "123456789012345678901234567890123456789012345", 45) == 0) {
		TestCase::Result *testResult = new TestCase::Result();

		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));

		instance->results[testID].push_back(testResult);
	}
}
