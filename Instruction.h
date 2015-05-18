#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include <stdlib.h>

#include <memory>
#include <vector>

class Instruction {
public:
	typedef std::vector<Instruction *> List;
	
	Instruction();
	Instruction(uint32_t address, const char *assembly, const char *binary, int binarySize);
	~Instruction();
	
	void setAddress(uint32_t address);
	void setAssembly(const char *assembly);
	void setBinary(const char *binary, int binarySize);
	
	uint32_t getAddress() const;
	const char *getAssembly() const;
	const char *getBinary() const;
	int getBinarySize() const;
	
private:
	uint32_t address_;
	char *assembly_;
	char *binary_;
	int binarySize_;
};

typedef std::shared_ptr<Instruction> InstructionPtr;

#endif
