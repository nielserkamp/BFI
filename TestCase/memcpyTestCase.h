#ifndef TEPHON_MEMCPY_TEST_CASE_H
#define TEPHON_MEMCPY_TEST_CASE_H

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class memcpyTestCase : public TestCase {
public:
	memcpyTestCase();
	~memcpyTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
	char *dstStrings_[3];
};

#endif
