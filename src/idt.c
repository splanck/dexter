#include "idt.h"
#include "config.h"

struct idt_desc idt_descriptors[DEXTER_TOTAL_INTERUPTS];
//4:42