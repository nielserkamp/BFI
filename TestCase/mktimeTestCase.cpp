#include <time.h>

extern "C" {
#include <tracer/util.h>
}

#include "mktimeTestCase.h"

mktimeTestCase::mktimeTestCase() {
	this->numOfTestCases_ = 1;
	this->numOfParameters_ = 1;
	this->instanceNumber_ = 0;
}

mktimeTestCase::~mktimeTestCase() {
	
}

void mktimeTestCase::storeParameters(trace_t *t) {
	std::vector<void *> param1;
	
	struct tm tmInfo;
	tmInfo.tm_year = 100;
	tmInfo.tm_mon = 1;
	tmInfo.tm_mday = 02;
	tmInfo.tm_hour = 3;
	tmInfo.tm_min = 4;
	tmInfo.tm_sec = 5;
	tmInfo.tm_isdst = 1;
	mktime(&tmInfo);
	
	param1.push_back((void *)this->storeData(t, (char *)&tmInfo, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param1);
	
	std::vector<void *> param2;
	struct tm tmInfo2;
	tmInfo2.tm_year = 101;
	tmInfo2.tm_mon = 1;
	tmInfo2.tm_mday = 23;
	tmInfo2.tm_hour = 21;
	tmInfo2.tm_min = 43;
	tmInfo2.tm_sec = 55;
	tmInfo2.tm_isdst = 0;
	mktime(&tmInfo2);
	param2.clear();
	param2.push_back((void *)this->storeData(t, (char *)&tmInfo2, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param2);
	
	std::vector<void *> param3;
	struct tm tmInfo3;
	tmInfo3.tm_year = 32;
	tmInfo3.tm_mon = 11;
	tmInfo3.tm_mday = 30;
	tmInfo3.tm_hour = 11;
	tmInfo3.tm_min = 11;
	tmInfo3.tm_sec = 11;
	tmInfo3.tm_isdst = 0;
	mktime(&tmInfo3);
	param3.clear();
	param3.push_back((void *)this->storeData(t, (char *)&tmInfo3, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param3);
	
	std::vector<void *> param4;
	struct tm tmInfo4;
	tmInfo4.tm_year = 4;
	tmInfo4.tm_mon = 4;
	tmInfo4.tm_mday = 4;
	tmInfo4.tm_hour = 4;
	tmInfo4.tm_min = 4;
	tmInfo4.tm_sec = 4;
	tmInfo4.tm_isdst = 4;
	mktime(&tmInfo4);
	param4.clear();
	param4.push_back((void *)this->storeData(t, (char *)&tmInfo4, sizeof(struct tm)));
	this->testCaseParameters_.push_back(param4);
}

void mktimeTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	
	printf("Check: %d %x %d\n", t->pid, address, live->getLocations().size());

	for(int i = 0; i < live->getLocations().size(); i++) {
		RegisterLocation *rLocation;
		rLocation = dynamic_cast<RegisterLocation *>(live->getLocations()[i].get());
	  	if(!rLocation) {
	  		continue;
	  	}
	
		uint32_t value = (uint32_t)this->getRegister(&t->regs, rLocation->getRegister());
		
		rLocation->print();
		printf(" %x\n", value);

		if(value == 0x38978285 && testID == 0) {
			TestCase::Result *testResult = new TestCase::Result ();

			testResult->testID = 0;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 0x3a96cb8b && testID == 1) {
			TestCase::Result *testResult = new TestCase::Result ();

			testResult->testID = 1;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 0xba65a6bb && testID == 2) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 2;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		} else if(value == 0x847dfe20 && testID == 3) {
			TestCase::Result *testResult = new TestCase::Result ();
			
			testResult->testID = 3;
			testResult->address = address;
			testResult->reg = std::static_pointer_cast<RegisterLocation>(live->getLocations()[i]);
			
			instance->results[testID].push_back(testResult);
		}
	}
}
