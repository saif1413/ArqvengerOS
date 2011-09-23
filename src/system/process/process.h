#ifndef __SYSTEM_PROCESS_PROCESS__
#define __SYSTEM_PROCESS_PROCESS__

#include "system/mm.h"

typedef void (*EntryPoint)(char*);

struct ProcessMemory {
    void* esp;
    void* stackStart;
    int pagesInStack;
};

struct ProcessSchedule {
    unsigned int inWait:1;
    unsigned int done:1;
};

typedef struct Process;
struct Process {
    int pid;

    int ppid;
    struct Process* parent;
    EntryPoint entryPoint;
    char args[512];
    int children;

    struct ProcessSchedule schedule;
    struct ProcessMemory mm;
};

void createProcess(struct Process* process, EntryPoint entryPoint, struct Process* parent, char* args);

void destroyProcess(struct Process* process);

void exitProcess(struct Process* process);

#endif