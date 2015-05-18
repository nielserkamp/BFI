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

#include "Address.h"

#define CHILD_INSTRUCTION_LIMIT 1000
#define CHILD_TIME_LIMIT 60
#define ANALYSIS_BUFFER_SIZE 256

ud_t ud_obj;
pid_t clientPID;
pid_t nextPID = -1;

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

	slaveRegisters = t->regs;
	t->regs = clientRegisters;
	set_registers(t);
	slaveLoaded = true;
	clientActive = true;
	
	printf("[+] Slave Loaded\n");

	return;
}

static void tracer_step(trace_t *t, void *data) {
	char buffer[32];
	
	if(memload(t->pid, (void *)buffer, (void *)(t->regs.eip), 32) == -1) {
		fprintf(stderr, "Could not load info from client at location %lx\n", t->regs.eip);
		exit(1);
	}

    ud_set_input_buffer(&ud_obj, (uint8_t *)buffer, 32);
    if (ud_disassemble(&ud_obj)) {
        printf("%p:\t%s\n", (void *)t->regs.eip, ud_insn_asm(&ud_obj));
    }
	
	if(!elfloaderLoaded) {
		return;
	}

	if(!slaveLoaded) {
		loadSlave(t);
		return;
	}
}

static void tracer_end(trace_t *t, void *data) {

}

static void tracer_start(trace_t *t, trace_t *parent, void *data) {
	steptrace_process(t, 1);
	
	printf("[+] Process %d spawned\n", t->pid);
	if(parent) {
		printf("[+] Process %d child of %d\n", t->pid, parent->pid);
	}
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

	long args[] = {0, elfloaderStat.st_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0};
	elfloaderAddress = (Address)inject_syscall(t, __NR_mmap2, args, 6, NULL);
	int n = 0;
    if((n = memstore(t->pid, (void *)buffer, (void *)(elfloaderAddress), elfloaderStat.st_size)) == -1){
		fprintf(stderr, "Error: Failed to store file on client\n");
		exit(1);
	}

    delete[] buffer;
	printf("[+] %x %x\n", elfloaderAddress, buffer);
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
}

static void print_signal(trace_t *t, void *data){
    fprintf(stdout, "[+] signal: pid %d, signal %d at %lx\n", t->pid, t->signal, t->regs.eip);
}

int main(int argc, char **argv) {
	tracer_plugin_t plug;

    ud_init(&ud_obj);
    ud_set_mode(&ud_obj, 32);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
    ud_set_syntax(&ud_obj, UD_SYN_ATT);

	// Start trace
	clientPID = run_traceable(argv[1], &argv[1], 1, 0);
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
