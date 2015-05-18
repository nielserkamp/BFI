#ifndef QSORT_TEST_CASE_H
#define QSORT_TEST_CASE_H

#include <stdint.h>

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class qsortTestCase : public TestCase {
public:
	struct SortElement {
		int a;
		int b;
		char c;
	};

	qsortTestCase();
	~qsortTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
	uint32_t arrays_[2];
};

#endif
