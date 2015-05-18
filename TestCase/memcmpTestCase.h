#ifndef MEMCMP_TEST_CASE_H
#define MEMCMP_TEST_CASE_H

#include <stdint.h>

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class memcmpTestCase : public TestCase {
public:
	memcmpTestCase();
	~memcmpTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
};

#endif
