#ifndef CFG_H
#define CFG_H

#include <stdint.h>

#include <vector>
#include <map>
#include <memory>

#include "Instruction.h"
#include "Address.h"

class CFGNode {
public:
	CFGNode();
	CFGNode(uint32_t label);
	~CFGNode();
	
	void setLabel(uint32_t label);
	uint32_t getLabel();
	
	void addPrevNode(std::weak_ptr<CFGNode> &node);
	void addNextNode(std::weak_ptr<CFGNode> &node);
	void addInstruction(InstructionPtr &instruction);
	
	std::vector<std::weak_ptr<CFGNode> > &getPrevNodes();
	std::vector<std::weak_ptr<CFGNode> > &getNextNodes();
	std::vector<InstructionPtr> &getInstructions();
private:
	uint32_t label_;
	
	std::vector<std::weak_ptr<CFGNode> > prevNodes_;
	std::vector<InstructionPtr> instructions_;
	std::vector<std::weak_ptr<CFGNode> > nextNodes_;
};

typedef std::shared_ptr<CFGNode> CFGNodePtr;

class CFG {
public:
	CFG();
	~CFG();
	
	void addInstruction(uint32_t address, InstructionPtr &instruction);

	std::map<Address, InstructionPtr> &getInstructions();
	std::vector<CFGNodePtr> &getNodes();
	std::map<Address, CFGNodePtr> &getNodesMap();
	std::map<Address, CFGNodePtr> &getLabelMap();
	std::vector<Address> &getLabels();

	void generate();
	void generate(int32_t address, const char *binary, int size);
private:
	std::vector<CFGNodePtr> nodes_;
	std::map<Address, InstructionPtr> instructions_;
	std::map<Address, CFGNodePtr> nodesMap_;
	std::map<Address, CFGNodePtr> labelMap_;
	std::vector<Address> labels_;
};

#endif
