#ifndef STRFTIME_TEST_CASE_H
#define STRFTIME_TEST_CASE_H

#include <stdint.h>

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class strftimeTestCase : public TestCase {
public:
	strftimeTestCase();
	~strftimeTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
};

#endif
