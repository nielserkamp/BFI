#ifndef FGETS_TEST_CASE_H
#define FGETS_TEST_CASE_H

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"
#include "Info.h"

class fgetsTestCase : public TestCase {
public:
	fgetsTestCase();
	~fgetsTestCase();

	void storeParameters(trace_t *t);
	void activateTestCase(trace_t *t, Info *info);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
	uint32_t strings_;
};

#endif
