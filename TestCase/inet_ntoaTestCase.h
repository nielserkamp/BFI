#ifndef INET_NTOA_TEST_CASE_H
#define INET_NTOA_TEST_CASE_H

extern "C" {
#include <tracer/trace.h>
}

#include "TestCase.h"

class inet_ntoaTestCase : public TestCase {
public:
	inet_ntoaTestCase();
	~inet_ntoaTestCase();

	void storeParameters(trace_t *t);
	void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address);
private:
};

#endif
