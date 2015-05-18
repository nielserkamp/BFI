#ifndef MKTIME_TEST_CASE_H
#define MKTIME_TEST_CASE_H

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class mktimeTestCase : public TestCase {
public:
	mktimeTestCase();
	~mktimeTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
	
private:
	uint32_t tmStructs_[4];
};

#endif
