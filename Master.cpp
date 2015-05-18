#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <map>
#include <vector>
#include <set>

#include <map>
#include <utility>
#include <fstream>
#include <vector>
#include <exception>

extern "C" {
	#include <tracer/trace.h>
	#include <tracer/process.h>
	#include <tracer/util.h>
	#include <tracer/debug.h>
	#include <udis86/udis86.h>
}

#include "TestCase/strcmpTestCase.h"
#include "TestCase/memcmpTestCase.h"
#include "TestCase/memcpyTestCase.h"
#include "TestCase/inet_ntoaTestCase.h"
#include "TestCase/mktimeTestCase.h"
#include "TestCase/strtolTestCase.h"
#include "TestCase/qsortTestCase.h"
#include "TestCase/fgetsTestCase.h"
#include "TestCase/sprintfTestCase.h"
#include "TestCase/strftimeTestCase.h"

#include "Exceptions.h"
#include "Address.h"
#include "AddressRangeList.h"
#include "Info.h"

#include "Analysis/BackwardReadAnalysis.h"
#include "Analysis/ForwardWriteAnalysis.h"

#define CHILD_INSTRUCTION_LIMIT 1000
#define CHILD_TIME_LIMIT 60
#define ANALYSIS_BUFFER_SIZE 256

ud_t ud_obj;
pid_t clientPID;
pid_t nextPID = -1;
AddressRangeList *addressRangeList = NULL;

TestCase *testCase;

Address elfloaderAddress;
Address stopClientAtAddress = 0x8048414;
Address currentAddress = 0;
Address activeAddress = 0;

bool elfloaderLoaded = false;
bool slaveLoaded = false;
bool clientActive = false;
bool slaveActive = false;

bool isBWRA = true;

registers_t clientRegisters;
registers_t slaveRegisters;

Info info;

FILE *output;

struct ActiveChild {
	pid_t pid;
	int count;
	int masterCount;
	bool started;
	struct timeval time;
	std::set<uint32_t> skipAddress;
};

std::map<pid_t, ActiveChild *> activeChildren;

pid_t tempFixedPID(void *_){
	pid_t ret = nextPID;
	nextPID = -1;
	return ret;
}

void loadSlave(trace_t *t) {
	if((Address)t->regs.eax != (Address)0xDEADBEAF) {
		return;
	}

	printf("[+] Slave Done Loading\n");	

	info.fileReadA = 0;
	info.fileReadB = 0;
	info.fileReadC = 0;

	fgetsTestCase *fgetsTC;
	fgetsTC = dynamic_cast<fgetsTestCase *>(testCase);
	if(fgetsTC) {
		info.fileReadA = (FILE *)1;
		info.fileReadB = (FILE *)1;
		info.fileReadC = (FILE *)1;
	}

	if(memstore(t->pid, (void *)&info, (void *)(t->regs.esi), sizeof(Info)) == -1) {
		fprintf(stderr, "Could not store info from client at location %lx\n", t->regs.eip);
		exit(1);
	}

	slaveRegisters = t->regs;
	t->regs = clientRegisters;
	set_registers(t);
	slaveLoaded = true;
	clientActive = true;
	
	printf("[+] Slave Loaded\n");

	return;
}

static void tracer_step(trace_t *t, void *data) {
	if(!elfloaderLoaded) {
		return;
	}

	if(!slaveLoaded) {
		loadSlave(t);
		return;
	}
	/*!
	// Child / Slave active
	if(t->pid != clientPID) {
		ActiveChild *activeChild = activeChildren[t->pid];
		struct timeval time;
		gettimeofday(&time, 0);

		// Child has not been activated for a test
		if(!activeChild->started) {

			// Need to be able to perform some function in the slave before activating
			if((Address)t->regs.eax != (Address)0xDEADBEAF) {
				return;
			}

			activeChild->started = true;
			printf("[+] Activate process %d to EIP %x\n", t->pid, activeAddress);

			fgetsTestCase *fgetsTC;
			fgetsTC = dynamic_cast<fgetsTestCase *>(testCase);
		  	if(fgetsTC) {
		  		Info info;
		  		if(memload(t->pid, (void *)&info, (void *)(t->regs.esi), sizeof(Info)) == -1) {
					fprintf(stderr, "Could not load info from client at location %lx\n", t->regs.eip);
					exit(1);
				}

				t->regs = clientRegisters;	
				t->regs.eip = activeAddress;	
				fgetsTC->activateTestCase(t, &info);
				set_registers(t);
				
				return;
		  	}

			t->regs = clientRegisters;
			t->regs.eip = activeAddress;		
			testCase->activateTestCase(t);
			set_registers(t);
			

			return;
		}
		
		// Limit on number of inrange instructions
		if(activeChild->count > CHILD_INSTRUCTION_LIMIT) {
			t->regs.eip = 0;
			set_registers(t);
			fprintf(stdout, "[+] Child %d executed too many instructions\n", t->pid);
			fflush(stdout);
			return;
		}
		
		// Time limit
		if((time.tv_sec - activeChild->time.tv_sec) > CHILD_TIME_LIMIT) {
			t->regs.eip = 0;
			set_registers(t);
			fprintf(stdout, "[+] Child %d timed out\n", t->pid);
			fflush(stdout);
			return;
		}
		

		// Get results
		if(!addressRangeList->isInRange(t->regs.eip)) {
			return;
		}
		
		if(activeChild->skipAddress.find((Address)t->regs.eip) != activeChild->skipAddress.end()) {
			return;
		}
		activeChild->skipAddress.insert((Address)t->regs.eip);
		
		printf("[+] Slave EIP: %x\n", t->regs.eip);
		
		char buffer[ANALYSIS_BUFFER_SIZE];
		if(memload(t->pid, (void *)buffer, (void *)(t->regs.eip), sizeof(buffer)) == -1) {
			fprintf(stderr, "Could not load from 32 bytes from client at location %lx\n", t->regs.eip);
			exit(1);
		}
	
		ud_set_input_buffer(&ud_obj, (uint8_t *)buffer, sizeof(buffer));
		if(!ud_disassemble(&ud_obj)) {
			fprintf(stderr, "Could not disassemble assembly code from location %lx\n", t->regs.eip);
			exit(1);
		}


		BackwardReadAnalysis analysis;
		analysis.analyse((Address)t->regs.eip, (const char *)buffer, ANALYSIS_BUFFER_SIZE);
		
		std::map<Address, LivePtr>::iterator found = analysis.getLiveLocations().find((Address)t->regs.eip);
		if(found != analysis.getLiveLocations().end()) {
			LivePtr live = found->second;
			testCase->getPossibleResults(t, live, (uint32_t)t->regs.eip);
		}

		activeChild->count++;
		
		return;
	}

	if(clientActive) {
		if(addressRangeList->isInRange(t->regs.eip)) {
			printf("[+] Client EIP: %lx\n", t->regs.eip);
		}
		if((Address)t->regs.eip != (Address)stopClientAtAddress) {
			return;
		}

		slaveActive = true;
		clientActive = false;
		clientRegisters = t->regs;
		t->regs = slaveRegisters;
		t->regs.eax = 0;
		set_registers(t);

		return;
	}

	if(slaveActive) {
		if((Address)t->regs.eax == (Address)0xDEADBEAF) {
			bool invalidInstruction = true;

			testCase->reset();
			testCase->reportResults(output);
			
			std::map<pid_t, ActiveChild *>::iterator it;
			for(it = activeChildren.begin(); it != activeChildren.end(); it++) {
				delete it->second;
			}
			activeChildren.clear();

			while(invalidInstruction) {
				if(isBWRA) {
					BackwardReadAnalysis analysis;

					unsigned char binaryBuffer[ANALYSIS_BUFFER_SIZE];
					binaryBuffer[0] = 0;
					if(memload(t->pid, (void *)binaryBuffer, (void *)(currentAddress), ANALYSIS_BUFFER_SIZE) == -1) {
						fprintf(stdout, "[+] Could not load memory %d\n", t->pid);
						fflush(stdout);
					}

					analysis.analyse((Address)currentAddress, (const char *)binaryBuffer, ANALYSIS_BUFFER_SIZE);
					if(analysis.getLiveLocations().size() != 0) {
						std::map<Address, LivePtr>::iterator liveLocationIt = analysis.getLiveLocations().find((Address)currentAddress);

						testCase->prepareTestCase(t, liveLocationIt->second, (Address)currentAddress);
						InstructionPtr instruction(new Instruction(currentAddress, "", (char *)binaryBuffer, ANALYSIS_BUFFER_SIZE));
						int s = testCase->getNumForks();
						activeAddress = currentAddress;
						testCase->applyHeuristic(instruction);
						t->regs.eax = testCase->getNumForks();
						set_registers(t);
						invalidInstruction = false;
						printf("%x: # %d -> %d\n", currentAddress, s, testCase->getNumForks());
					}

					if(!addressRangeList->nextAddress(&currentAddress)) {
						fclose(output);
						exit(1);
					}
				} else {
					ForwardWriteAnalysis analysis(10);

					unsigned char binaryBuffer[ANALYSIS_BUFFER_SIZE];
					binaryBuffer[0] = 0;
					if(memload(t->pid, (void *)binaryBuffer, (void *)(currentAddress), ANALYSIS_BUFFER_SIZE) == -1) {
						fprintf(stdout, "[+] Could not load memory %d\n", t->pid);
						fflush(stdout);
					}

					if(analysis.analyse((Address)currentAddress, (char *)binaryBuffer, ANALYSIS_BUFFER_SIZE) && analysis.getInstructions().size() != 0) {
						InstructionPtr iPtr = analysis.getInstructions()[analysis.getInstructions().size() - 1];
						std::map<Address, LivePtr>::iterator liveLocationIt = analysis.getLiveLocations().find((Address)iPtr->getAddress());

						testCase->prepareTestCase(t, liveLocationIt->second, (Address)iPtr->getAddress());
						InstructionPtr instruction(new Instruction(iPtr->getAddress(), "", (char *)iPtr->getBinary(), iPtr->getBinarySize()));
						int s = testCase->getNumForks();
						activeAddress = iPtr->getAddress();
						//testCase->applyHeuristic(instruction);
						t->regs.eax = testCase->getNumForks();
						set_registers(t);
						invalidInstruction = false;
						printf("%x: # %d -> %d\n", activeAddress, s, testCase->getNumForks());	
					}
					
					if(!addressRangeList->nextAddress(&currentAddress)) {
						fclose(output);
						exit(1);
					}
				}
			};

		}	
	}
	!*/
}

static void tracer_end(trace_t *t, void *data) {
	/*!if(t->pid != clientPID) {
		printf("[+] Child %d stopped\n", t->pid);
		testCase->processStopped(output, t->pid);
	}!*/
}

static void tracer_start(trace_t *t, trace_t *parent, void *data) {
	steptrace_process(t, 1);
	
	printf("[+] Process %d spawned\n", t->pid);
	if(parent) {
		printf("[+] Process %d child of %d\n", t->pid, parent->pid);
	}
	/*!
	// Add new fork to active children
	if(clientPID != t->pid) {
		ActiveChild *activeChild = new ActiveChild();
		activeChild->pid = t->pid;
		activeChild->started = false;
		activeChild->count = 0;
		activeChild->masterCount = 0;
		gettimeofday(&activeChild->time, 0);
		activeChildren[t->pid] = activeChild;
	}!*/
}

void loadElfloader(trace_t *t) {
	struct stat elfloaderStat;
    if(stat("SlaveLoader", &elfloaderStat) == -1){
        fprintf(stderr, "Error: Failed to stat elfloader file\n");
        exit(1);
    }

   	char *buffer = new char[elfloaderStat.st_size];
    FILE *f = fopen("SlaveLoader", "r");
    if(f == NULL) {
        fprintf(stderr, "Error: Failed to open elfloader file\n");
        exit(1);
    }
    fread(buffer, 1, elfloaderStat.st_size, f);
    fclose(f);
	
    printf("[+] Elfloader size: %d\n", elfloaderStat.st_size);

	long args[] = {0, 0x16000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0};
	elfloaderAddress = (Address)inject_syscall(t, __NR_mmap2, args, 6, NULL);
	
    if(memstore(t->pid, (void *)buffer, (void *)(elfloaderAddress), elfloaderStat.st_size) == -1){
		fprintf(stderr, "Error: Failed to store file on client\n");
		exit(1);
	}

    delete[] buffer;
	
	//!testCase->storeParameters(t);
	printf("[+] Elfloader Loaded\n");

	// Run Elfloader
	clientRegisters = t->regs;
    t->regs.eip = elfloaderAddress;
    set_registers(t);

	elfloaderLoaded = true;
}

static void tracer_pre_call(trace_t *t, void *data){
	if(!elfloaderLoaded) {
		loadElfloader(t);
		return;
	}
	/*!
	if (t->syscall == __NR_clone){
		nextPID = t->pid;
		return;
	}

	if(t->pid != clientPID) {
		ActiveChild *activeChild = activeChildren[t->pid];
		if(!activeChild->started) {
			return;
		}
		
		fgetsTestCase *fgetsTC;
		fgetsTC = dynamic_cast<fgetsTestCase *>(testCase);
		if(fgetsTC) {
			return;
		}
		
		mktimeTestCase *mktimeTC;
		mktimeTC = dynamic_cast<mktimeTestCase *>(testCase);
		if(mktimeTC) {
			return;
		}
		
		t->regs.eip = 0;
		set_registers(t);
		undo_syscall(t);
	}!*/
}

static void print_signal(trace_t *t, void *data){
    fprintf(stdout, "[+] signal: pid %d, signal %d at %lx\n", t->pid, t->signal, t->regs.eip);
}

int main(int argc, char **argv) {
	tracer_plugin_t plug;

	if(strcmp(argv[1], "strcmp") == 0) {
		testCase = new strcmpTestCase();
	} else if(strcmp(argv[1], "memcmp") == 0) {
		testCase = new memcmpTestCase();
	} else if(strcmp(argv[1], "memcpy") == 0) {
		testCase = new memcpyTestCase();
	} else if(strcmp(argv[1], "inet_ntoa") == 0) {
		testCase = new inet_ntoaTestCase();
	} else if(strcmp(argv[1], "sprintf") == 0) {
		testCase = new sprintfTestCase();
	} else if(strcmp(argv[1], "strftime") == 0) {
		testCase = new strftimeTestCase();
	} else if(strcmp(argv[1], "fgets") == 0) {
		testCase = new fgetsTestCase();
	} else if(strcmp(argv[1], "mktime") == 0) {
		testCase = new mktimeTestCase();
	} else if(strcmp(argv[1], "strtol") == 0) {
		testCase = new strtolTestCase();
	} else if(strcmp(argv[1], "qsort") == 0) {
		testCase = new qsortTestCase();
	} else {
		fprintf(stderr, "Invalid Testcase\n");
		exit(1);
	}

	output = fopen(argv[2], "w");

	// Default Range
	addressRangeList = AddressRangeList::createFromFile(argv[3]);

	stopClientAtAddress = strtol(argv[4], 0, 16);

	//addressRangeList->addRange(0x8050dc4, 0x09000000);
	currentAddress = addressRangeList->startAddress();

	// Setup Disassembler
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_syntax(&ud_obj, UD_SYN_ATT);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);

	// Start trace
	clientPID = run_traceable(argv[5], &argv[5], 1, 0);
    memset(&plug, 0, sizeof(plug));
    plug.start = tracer_start;
    plug.step = tracer_step;
   	plug.stop = tracer_end;
    plug.pre_call = tracer_pre_call;
    plug.pid_selector = tempFixedPID;
    plug.signal = print_signal;
    debug_init(stdout);
	
	trace(clientPID, &plug);

	return 0;
}