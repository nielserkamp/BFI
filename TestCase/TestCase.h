#ifndef TESTCASE_H
#define TESTCASE_H

#include <vector>
#include <map>
#include <set>

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include <tracer/trace.h>
}

#include "Analysis/BackwardReadAnalysis.h"
#include "Location/Location.h"
#include "Location/RegisterLocation.h"
#include "Instruction.h"

class TestCase {
public:
	struct Result {
		int testID;
		uint32_t address;
		RegisterLocationPtr reg;
	};

	struct Instance {
		uint64_t number;
		uint32_t address;
		std::vector<LocationPtr> parameterOrder;
		std::map<pid_t, int> pidToTestID;
		std::map<int, std::vector<Result *> > results;
		int stopped;
	};
	
	struct Fork {
		int testID;
		Instance *instance;
	};
	
	~TestCase();

	virtual void storeParameters(trace_t *t) = 0;
	void prepareTestCase(trace_t *t, LivePtr &live, uint32_t address);
	void applyHeuristic(InstructionPtr instruction);
	void activateTestCase(trace_t *t);
	virtual void getPossibleResults(trace_t *t, LivePtr &live, uint32_t address) = 0;
	void reset();
	void processStopped(FILE *reportFile, pid_t pid);
	void reportResults(FILE *reportFile);
	
	int getNumForks();
	int getNumInstances();
	int getNumResults();
	int getNumParameters();
	int getNumTestCases();
protected:
	void generateParameterOrders(int n, std::vector<LocationPtr> left, std::vector<LocationPtr> current, std::vector<std::vector<LocationPtr> > *result);

	uint64_t storeData(trace_t *t, const char *buffer, size_t size);
	uint32_t getRegister(registers_t *registers, ud_type_t reg);
	void setRegister(registers_t *registers, ud_type_t reg, uint32_t value);
	bool foundResult(TestCase::Result *result, std::map<int, std::vector<TestCase::Result *> > *tests);
	bool findResult(TestCase::Result *result, std::vector<TestCase::Result  *> results);
	void checkResult(FILE *reportFile, TestCase::Instance *instance);
	
protected:
	std::vector< std::vector<void *> > testCaseParameters_;
	
	std::set<Instance *> instances_;
	std::map<pid_t, Instance *> pidToInstance_;
	
	int numOfParameters_;
	int numOfTestCases_;
	uint64_t instanceNumber_;
	std::vector<Fork *> forks_;
};

#endif
