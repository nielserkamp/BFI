#ifndef FORWARD_READ_ANALYSIS_H
#define FORWARD_READ_ANALYSIS_H

extern "C" {
#include <udis86/udis86.h>
}

#include "Analysis.h"
#include "Instruction.h"

class ForwardReadAnalysis : public Analysis {
public:
	ForwardReadAnalysis();
	ForwardReadAnalysis(int window);
	~ForwardReadAnalysis();
	
	void addInstruction(Instruction *instruction);
	const Instruction::List &getInstructions() const;
private:
	int window_;
	ud_t ud_obj_;
	Instruction::List instructions_;
	
};

#endif
