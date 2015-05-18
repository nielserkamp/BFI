#ifndef SPRINTF_TEST_CASE_H
#define SPRINTF_TEST_CASE_H

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class sprintfTestCase : public TestCase {
public:
	sprintfTestCase();
	~sprintfTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
	char *resultStrings_[3];
};

#endif
