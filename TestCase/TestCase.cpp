extern "C" {
#include <tracer/util.h>
#include <udis86/udis86.h>
}
#include <string.h>
#include <sys/mman.h>
#include <linux/unistd.h>
#include <stdlib.h>

#include "TestCase.h"
#include "LivenessArray.h"

TestCase::~TestCase() {
	this->reset();
}

void TestCase::reset() {
	for (std::set<TestCase::Instance *>::iterator it = this->instances_.begin(); it != this->instances_.end(); ++it) {
		delete *it;
	}



	for(size_t i = 0; i < this->forks_.size(); i++) {
		fprintf(stdout, "[!] A fork has not been used\n");
		delete this->forks_[i];
	}

	this->forks_.clear();
	this->instances_.clear();
	this->pidToInstance_.clear();
}

void TestCase::generateParameterOrders(int n, std::vector<LocationPtr> parameterLocations, std::vector<LocationPtr> current, std::vector<std::vector<LocationPtr> > *result) {
	if(n == 0) {
	    result->push_back(current);
	    return;
	}
	
	for(size_t i = 0; i < parameterLocations.size(); i++) {
	    std::vector<LocationPtr> parameterLocationsLeft = parameterLocations;
	    std::vector<LocationPtr> newCurrent = current;
	    parameterLocationsLeft.erase(parameterLocationsLeft.begin() + i);
	    newCurrent.push_back(parameterLocations[i]);
	    generateParameterOrders(n - 1, parameterLocationsLeft, newCurrent, result);
	}
}

uint64_t getOffset(ud_operand_t *operand) {
	uint64_t offset;
	switch(operand->offset){
		case 8:
			offset = (uint64_t)operand->lval.sbyte;
			break;
		case 16:
			offset = (uint64_t)operand->lval.sword;
			break;
		case 32:
			offset = (uint64_t)operand->lval.sdword;
			break;
		default:
			offset = 0;
	}
	return offset;
}


void TestCase::applyHeuristic(InstructionPtr instruction) {
	ud_t ud_obj;
	ud_init(&ud_obj);
	ud_set_syntax(&ud_obj, UD_SYN_ATT);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_mode(&ud_obj, 32);

	ud_set_input_buffer(&ud_obj, (uint8_t *)instruction->getBinary(), instruction->getBinarySize());
	if(!ud_disassemble(&ud_obj)) {
		fprintf(stderr, "Error: Failed to disassemble\n");
		exit(1);
	}

	std::vector<LocationPtr> readLocations;

	Tephon::insn_liveliness_t opReadWrite = Tephon::operand_liveliness[ud_obj.mnemonic];
	for(int k = 0; k < 3; k++){
		if(opReadWrite.type[k] == Tephon::LL_READ){
			if(ud_obj.operand[k].type == UD_OP_IMM && ud_obj.mnemonic != UD_Ijmp) {
				if(ud_obj.operand[k].size == 32 && ud_obj.operand[k].lval.sdword >= 0) {
					for(int i = 0; i < instruction->getBinarySize() - 3; i++) {
						if(memcmp(instruction->getBinary() + i, (char *)&ud_obj.operand[k].lval.sdword, 4) == 0) {
							LocationPtr loc(new InstructionLocation(instruction->getAddress() + i, 4));
							readLocations.push_back(loc);
							
							break;
						}
					}
				}
			} else if(ud_obj.operand[k].type == UD_OP_REG) {
				LocationPtr loc(new RegisterLocation(ud_obj.operand[k].base));
				readLocations.push_back(loc);
			}else if(ud_obj.operand[k].type == UD_OP_MEM){
				uint64_t offset = getOffset(&ud_obj.operand[k]);

				if(ud_obj.operand[k].base != UD_NONE || ud_obj.operand[k].index != UD_NONE) {
					RegisterLocationPtr baseLoc(new RegisterLocation(ud_obj.operand[k].base));
					RegisterLocationPtr indexLoc(new RegisterLocation(ud_obj.operand[k].index));
					
					LocationPtr loc(new MemoryLocation(baseLoc, indexLoc, offset, 0));
					readLocations.push_back(loc);
				}
			}
		}
	}

	for(int i = 0; i < forks_.size(); i++) {
		TestCase::Instance *instance = forks_[i]->instance;

		bool found = false;
		for(int j = 0; j < readLocations.size(); j++){ 
			found = false;
			for(int k = 0; k < instance->parameterOrder.size(); k++) {
				if(readLocations[j]->compare(instance->parameterOrder[k].get()) == 0) {
					found = true;
					break;
				}
			}
			if(!found) {
				break;
			}
		}

		if(!found) {
			TestCase::Fork *f = forks_[i];
			forks_.erase(forks_.begin() + i);
			delete f;
			i--;
		}
	}
}

void TestCase::prepareTestCase(trace_t *t, LivePtr &live, Address address) {
	std::vector<std::vector<LocationPtr> > parameterOrders;
	std::vector<LocationPtr> parameterLocations;
	std::vector<LocationPtr> trackLocations;
	
	for(size_t i = 0; i < live->getLocations().size(); i++) {
		parameterLocations.push_back(live->getLocations()[i]);
	}

	generateParameterOrders(this->numOfParameters_, parameterLocations, trackLocations, &parameterOrders);

	for(size_t i = 0; i < parameterOrders.size(); i++) {
		TestCase::Instance *instance = new TestCase::Instance();
		instance->parameterOrder = parameterOrders[i];
		instance->address = address;
		instance->stopped = 0;
		instance->number = this->instanceNumber_++;
		this->instances_.insert(instance);
		
		for(int j = 0; j < this->numOfTestCases_; j++) {
			TestCase::Fork *fork = new TestCase::Fork();
			fork->instance = instance;
			fork->testID = j;
			this->forks_.push_back(fork);
		}
	}
}

void TestCase::activateTestCase(trace_t *t) {
	TestCase::Fork *fork = *this->forks_.begin();

	TestCase::Instance *instance = fork->instance;
	instance->pidToTestID[t->pid] = fork->testID;
	this->pidToInstance_[t->pid] = instance;
	
	printf("Activate: %d | ", t->pid);
	int atParameter = 0;
	for(size_t i = 0; i < instance->parameterOrder.size(); i++) {
		instance->parameterOrder[i]->print();
		printf(" %d %d %d %d", fork->testID, atParameter, instance->parameterOrder.size(), testCaseParameters_.size());
		uint32_t value = (uint32_t)testCaseParameters_[fork->testID][atParameter];
		instance->parameterOrder[i]->setValue(t, value);
		atParameter++;
	}
	printf("\n");
	
	this->forks_.erase(this->forks_.begin());
	delete fork;
}

void TestCase::checkResult(FILE *reportFile, TestCase::Instance *instance) {
	if(instance->results.size() < (size_t)this->numOfTestCases_) {
		return;
	}
	
	std::vector<TestCase::Result *> *results = &instance->results[0];
	
	for(size_t j = 0; j < results->size(); j++) {
		if(foundResult((*results)[j], &instance->results)) {
			printf("RESULTS FOUND ==================\n");
			if((*results)[j]->reg) {
				fprintf(reportFile, "%llx, %x, %x, %s, ", instance->number, instance->address, (*results)[j]->address, Tephon::reg_names[(*results)[j]->reg->getRegister()]);
			} else {
				fprintf(reportFile, "%llx, %x, %x, %s, ", instance->number, instance->address, (*results)[j]->address, "NONE");
			}
			/*fprintf(reportFile, "FOUND RESULT\n");
			fprintf(reportFile, "Number  : %llx\n", instance->number);
			fprintf(reportFile, "Start   : %llx\n", instance->address);
			fprintf(reportFile, "End     : %llx\n", (*results)[j]->address);
			fprintf(reportFile, "Reg     : %s\n", reg_names[(*results)[j]->reg]);*/
			fprintf(reportFile, "\"");
			for(size_t k = 0; k < instance->parameterOrder.size(); k++) {
				instance->parameterOrder[k]->print(reportFile);
				fprintf(reportFile, " ");
			}
			fprintf(reportFile, "\"\n");
			fflush(reportFile);
		}
	}
}

void TestCase::processStopped(FILE *reportFile, pid_t pid) {
	TestCase::Instance *instance = this->pidToInstance_[pid];
	instance->stopped += 1;
	
	if(instance->stopped == this->numOfTestCases_) {
		checkResult(reportFile, instance);
		
		std::map<int, std::vector<Result *> >::iterator it;
		for(it = instance->results.begin(); it != instance->results.end(); it++) {
			std::vector<TestCase::Result *> results = it->second;
			for(size_t i = 0; i < results.size(); i++) {
				delete results[i];
			}
		}
		
		this->instances_.erase(instance);
		this->pidToInstance_.erase(pid);
		delete instance;
	}
}

void TestCase::reportResults(FILE *reportFile) {
	for (std::set<TestCase::Instance *>::iterator it = this->instances_.begin(); it != this->instances_.end(); ++it) {
		checkResult(reportFile, *it);
	}
}

uint64_t TestCase::storeData(trace_t *t, const char *buffer, size_t size) {
	uint64_t address;

	long args[] = {0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0};
	address = (uint64_t)inject_syscall(t, __NR_mmap2, args, 6, NULL);
	if(!address) {
		fprintf(stderr, "Could not mmap\n");
		exit(1);
	}

	if(memstore(t->pid, (void *)buffer, (void *)(address), size) == -1){
		fprintf(stderr, "Error: Failed to store file on client\n");
		exit(1);
	}

	return address;
}

uint32_t TestCase::getRegister(registers_t *registers, ud_type_t reg) {
	switch(reg){
	    case UD_R_EAX:
	        return registers->eax;
	    case UD_R_EBX:
	        return registers->ebx;
	    case UD_R_ECX:
	        return registers->ecx;
	    case UD_R_EDX:
	        return registers->edx;
	    case UD_R_ESI:
	        return registers->esi;
	    case UD_R_EDI:
	        return registers->edi;
	    case UD_R_EBP:
	        return registers->ebp;
	    case UD_R_ESP:
	        return registers->esp;
	    case UD_R_RIP:
	        return registers->eip;
	    default:
	    	return registers->eax;
	}
}

void TestCase::setRegister(registers_t *registers, ud_type_t reg, uint32_t value) {
	if(reg == UD_R_EAX || reg == UD_R_AL || reg == UD_R_AH || reg == UD_R_AX) {
		registers->eax = value;
	} else if(reg == UD_R_EBX || reg == UD_R_BL || reg == UD_R_BH || reg == UD_R_BX) {
		registers->ebx = value;
	} else if(reg == UD_R_ECX || reg == UD_R_CL || reg == UD_R_CH || reg == UD_R_CX) {
		registers->ecx = value;
	} else if(reg == UD_R_EDX || reg == UD_R_DL || reg == UD_R_DH || reg == UD_R_DX) {
		registers->edx = value;
	} else if(reg == UD_R_ESI || reg == UD_R_SI) {
		registers->esi = value;
	} else if(reg == UD_R_EDI || reg == UD_R_DI) {
		registers->edi = value;
	} else if(reg == UD_R_EBP || reg == UD_R_BP) {
		registers->ebp = value;
	} else if(reg == UD_R_ESP || reg == UD_R_SP) {
		registers->esp = value;
	} else if(reg == UD_R_RIP) {
		registers->eip = value;
	}
}

bool TestCase::findResult(TestCase::Result *result, std::vector<TestCase::Result  *> results) {
	for(size_t i = 0; i < results.size(); i++) {

		if(result->address == results[i]->address 
			&& (result->reg == 0 
				|| result->reg->compare(results[i]->reg.get()) == 0)
		) {
			return true;
		}
	}

	return false;
}

bool TestCase::foundResult(TestCase::Result *result, std::map<int, std::vector<TestCase::Result *> > *tests) {
	for(int i = 0; i < numOfTestCases_; i++) {
		if(!findResult(result, (*tests)[i])) {
			return false;
		}
	}

	return true;
}

int TestCase::getNumForks() {
	return this->forks_.size();
}

int TestCase::getNumInstances() {
	return this->instances_.size();
}

int TestCase::getNumResults() {
	return 0;
}

int TestCase::getNumParameters() {
	return this->numOfParameters_;
}

int TestCase::getNumTestCases() {
	return this->numOfTestCases_;
}
