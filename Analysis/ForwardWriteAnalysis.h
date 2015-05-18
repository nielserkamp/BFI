#ifndef FORWARD_WRITE_ANALYSIS_H
#define FORWARD_WRITE_ANALYSIS_H

#include <vector>

extern "C" {
#include <udis86/udis86.h>
}

#include "Analysis.h"
#include "Instruction.h"

class ForwardWriteAnalysis : public Analysis {
public:
	ForwardWriteAnalysis();
	ForwardWriteAnalysis(int window);
	~ForwardWriteAnalysis();
	
	bool analyse(uint32_t address, const char *binary, int size);
	void addInstruction(InstructionPtr &instruction);
	const std::vector<InstructionPtr> &getInstructions() const;
private:
	int window_;
	ud_t ud_obj_;
	std::vector<InstructionPtr> instructions_;
	
};

#endif
