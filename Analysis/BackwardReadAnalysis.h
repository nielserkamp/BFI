#ifndef BACKWARD_READ_ANALYSIS_H
#define BACKWARD_READ_ANALYSIS_H

#include <stdint.h>

#include <map>
#include <vector>
#include <set>
#include <memory>

#include "Location/Location.h"
#include "Location/MemoryLocation.h"
#include "Location/RegisterLocation.h"
#include "Location/InstructionLocation.h"
#include "CFG.h"
#include "Analysis.h"

class BackwardReadAnalysis : public Analysis {
public:
	BackwardReadAnalysis();
	BackwardReadAnalysis(int window);
	~BackwardReadAnalysis();
	
	void setCFG(CFG *cfg);
	CFG *getCFG();
	
	void analyse();
	void analyse(uint32_t address, const char *binary, int size);
	void analyse(CFGNodePtr &node, std::vector<InstructionPtr> &prevInstructions);
	
	int count;
protected:
	void analyiseInstruction(LivePtr &live, InstructionPtr &instruction);
	void analyseNode(LivePtr &live, CFGNodePtr &node, std::set<CFGNodePtr> seen, int left);
	
private:
	ud_t ud_obj_;
	CFG *cfg_;
	long instructionsDone_;
	int window_;
};

#endif
