#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Info.h"

Info info;

uint32_t breakpoint(uint32_t infoAddr) {
	uint32_t addr;
	asm  (
		"mov %1, %%esi;"
		"mov $0xDEADBEAF, %%eax;"
		"movl %%eax, %0;"
		:"=r"(addr)
		:"r"(infoAddr)
	);
	return addr;
}

int main() {
	
	while(true){
		uint32_t numForks = breakpoint((uint32_t)&info);

		int n = 0;
		for(size_t i = 0; i < numForks; i++) {
			pid_t pid;
			
			// Child
			if((pid = fork()) == 0) {
				if(info.fileReadA) {
					info.fileReadA = fopen("testFiles/fileA", "r");
					if(info.fileReadA == 0) {
						printf("[+] Failed to open file\n");
					}
				}
				if(info.fileReadB) {
					info.fileReadB = fopen("testFiles/fileB", "r");
					if(info.fileReadB == 0) {
						printf("[+] Failed to open file\n");
					}
				}
				if(info.fileReadC) {
					info.fileReadC = fopen("testFiles/fileC", "r");
					if(info.fileReadC == 0) {
						printf("[+] Failed to open file\n");
					}
				}
				
				while(1) {
					 breakpoint((uint32_t)&info);
				}
			}
			
			// An error while forking, just try again
			if(pid < 0) {
				printf("[+] Error while forking");
				i--;
				fprintf(stdout, "[+] Failed to fork\n");
				fflush(stdout);
				//sleep(1);
			} 
			
			// Child has forked, so keep track of how many
			if(pid > 0) {
				n++;
			}
			
			// If we have 5 or more forks are spawned wait for them to stop
			if(n >= 5) {
				for(int j = 0; j < n; j++) {
					int status;
					wait(&status);
				}
				n = 0;
			}
		}
		
		// Clean up the zombies
		for(int i = 0; i < n; i++) {
			int status;
			wait(&status);
		}
	}
}
