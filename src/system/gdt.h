#ifndef __SYSTEM_GDT__
#define __SYSTEM_GDT__

#define KERNEL_CODE_SEGMENT 0x08
#define KERNEL_DATA_SEGMENT 0x10
#define USER_CODE_SEGMENT (0x18 | 0x3)
#define USER_DATA_SEGMENT (0x20 | 0x3)
#define TASK_STATE_SEGMENT 0x28

void setupGDT(void);

unsigned int gdt_page_address(void);

#endif
