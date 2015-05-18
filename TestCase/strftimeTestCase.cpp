#include <time.h>
#include <string.h>

extern "C" {
#include <tracer/util.h>
}

#include "strftimeTestCase.h"

strftimeTestCase::strftimeTestCase() {
	this->numOfTestCases_ = 2;
	this->numOfParameters_ = 4;
	this->instanceNumber_ = 0;
}

strftimeTestCase::~strftimeTestCase() {
	
}

void strftimeTestCase::storeParameters(trace_t *t) {
	uint32_t buffer = this->storeData(t, "12345678901234567890123456789012345678901234567890123456789", 61);
	struct tm tmInfo;
	tmInfo.tm_year = 100;
	tmInfo.tm_mon = 0;
	tmInfo.tm_mday = 1;
	tmInfo.tm_hour = 13;
	tmInfo.tm_min = 0;
	tmInfo.tm_sec = 0;
	tmInfo.tm_isdst = 0;
	mktime(&tmInfo);
	
	std::vector<void *> param1;
	param1.push_back((void *)buffer);
	param1.push_back((void *)61);
	param1.push_back((void *)this->storeData(t, "%c", 3));
	param1.push_back((void *)this->storeData(t, (char *)&tmInfo, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param1);
	
	struct tm tmInfo1;
	tmInfo1.tm_year = 110;
	tmInfo1.tm_mon = 4;
	tmInfo1.tm_mday = 3;
	tmInfo1.tm_hour = 6;
	tmInfo1.tm_min = 30;
	tmInfo1.tm_sec = 30;
	tmInfo1.tm_isdst = 0;
	mktime(&tmInfo1);
	
	std::vector<void *> param2;
	param2.push_back((void *)buffer);
	param2.push_back((void *)61);
	param2.push_back((void *)this->storeData(t, "%c %A %D", 9));
	param2.push_back((void *)this->storeData(t, (char *)&tmInfo1, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param2);
}

void strftimeTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	if(memload(t->pid, (void *)buffer, (void *)(this->testCaseParameters_[testID][0]), 61) == -1) {
		return;
	}
	
	if(testID == 0) {
		if(memcmp((void *)buffer, (void *)"Sat Jan  1 13:00:00 2000", strlen("Sat Jan  1 13:00:00 2000")) == 0) {
			TestCase::Result *testResult = new TestCase::Result();
		
			testResult->testID = testID;
			testResult->address = address;
			testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
		
			instance->results[testID].push_back(testResult);
		}
	} else if(testID == 1) {
		if(memcmp((void *)buffer, (void *)"Mon May  3 07:30:30 2010 Monday 05/03/10", strlen("Mon May  3 07:30:30 2010 Monday 05/03/10")) == 0) {
			TestCase::Result *testResult = new TestCase::Result();
		
			testResult->testID = testID;
			testResult->address = address;
			testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
		
			instance->results[testID].push_back(testResult);
		}
	}
	
}
