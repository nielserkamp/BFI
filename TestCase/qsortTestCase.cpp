extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}

#include "qsortTestCase.h"

qsortTestCase::qsortTestCase() {
	this->numOfTestCases_ = 1;
	this->numOfParameters_ = 4;
}

qsortTestCase::~qsortTestCase() {
	
}

void qsortTestCase::storeParameters(trace_t *t) {
	std::vector<void *> parameters;
	
	int intArray[] = {4, 1, 111, 2, 5};
	SortElement structArray[] = {
		{1, 2, 'c'},
		{2, 3, 'd'},
		{2, 3, 'e'},
		{1, 3, 'a'},
		{3, 3, 'b'}
	};
	
	char intCompare[] = "\x55\x89\xe5\x8b\x45\x08\x8b\x10\x8b\x45\x0c\x8b\x00\x89\xd1\x29\xc1\x89\xc8\x5d\xc3";
	char structCompare[] = "\x55\x89\xe5\x83\xec\x10\x8b\x45\x08\x89\x45\xf8\x8b\x45\x0c\x89\x45\xfc\x8b\x45\xf8\x8b\x10\x8b\x45\xfc\x8b\x00\x39\xc2\x75\x58\x8b\x45\xf8\x8b\x50\x04\x8b\x45\xfc\x8b\x40\x04\x39\xc2\x75\x35\x8b\x45\xf8\x0f\xb6\x50\x08\x8b\x45\xfc\x0f\xb6\x40\x08\x38\xc2\x75\x07\xb8\x00\x00\x00\x00\xeb\x3f\x8b\x45\xf8\x0f\xb6\x40\x08\x0f\xbe\xd0\x8b\x45\xfc\x0f\xb6\x40\x08\x0f\xbe\xc0\x89\xd1\x29\xc1\x89\xc8\xeb\x23\x8b\x45\xfc\x8b\x50\x04\x8b\x45\xf8\x8b\x00\x89\xd1\x29\xc1\x89\xc8\xeb\x10\x8b\x45\xf8\x8b\x10\x8b\x45\xfc\x8b\x00\x89\xd1\x29\xc1\x89\xc8\xc9\xc3";
	
	this->arrays_[0] = (uint32_t)this->storeData(t, (char *)intArray, sizeof(intArray));
	parameters.push_back((void *)this->arrays_[0]);
	parameters.push_back((void *)5);
	parameters.push_back((void *)sizeof(int));
	parameters.push_back((void *)this->storeData(t, (char *)intCompare, sizeof(intCompare)));
	this->testCaseParameters_.push_back(parameters);
	
	parameters.clear();
	this->arrays_[1] = (uint32_t)this->storeData(t, (char *)structArray, sizeof(structArray));
	parameters.push_back((void *)this->arrays_[1]);
	parameters.push_back((void *)5);
	parameters.push_back((void *)sizeof(SortElement));
	parameters.push_back((void *)this->storeData(t, (char *)structCompare, sizeof(structCompare)));
	this->testCaseParameters_.push_back(parameters);
}

void qsortTestCase::getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) {
	Instance *instance = this->pidToInstance_[t->pid];
	int testID = instance->pidToTestID[t->pid];
	char buffer[1024];
	
	if(memload(t->pid, (void *)buffer, (void *)this->arrays_[testID], (uint32_t)this->testCaseParameters_[testID][1] * (uint32_t)this->testCaseParameters_[testID][2]) == -1) {
		return;
	}
	
	if(testID == 0) {
		int *arr = (int *)buffer;
		if(arr[0] != 1) {
			return;
		}
		if(arr[1] != 2) {
			return;
		}
		if(arr[2] != 4) {
			return;
		}
		if(arr[3] != 5) {
			return;
		}
		if(arr[4] != 111) {
			return;
		}
		TestCase::Result *testResult = new TestCase::Result();
	
		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
	
		instance->results[testID].push_back(testResult);
	} else if(testID == 1) {
		SortElement *arr = (SortElement *)buffer;
		
		if(arr[0].a != 1 ||
			arr[0].b != 3 ||
			arr[0].c != 'a'
		) {
			return;
		}
		if(arr[1].a != 1 ||
			arr[1].b != 2 ||
			arr[1].c != 'c'
		) {
			return;
		}
		if(arr[2].a != 2 ||
			arr[2].b != 3 ||
			arr[2].c != 'd'
		) {
			return;
		}
		if(arr[3].a != 2 ||
			arr[3].b != 3 ||
			arr[3].c != 'e'
		) {
			return;
		}
		if(arr[4].a != 3 ||
			arr[4].b != 3 ||
			arr[4].c != 'b'
		) {
			return;
		}
		TestCase::Result *testResult = new TestCase::Result();
	
		testResult->testID = testID;
		testResult->address = address;
		testResult->reg = RegisterLocationPtr(new RegisterLocation(UD_NONE));
	
		instance->results[testID].push_back(testResult);
	}
}
