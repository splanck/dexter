#include "idt.h"
#include "config.h"
#include "memory.h"

struct idt_desc idt_descriptors[DEXTER_TOTAL_INTERUPTS];
struct idtr_desc idtr_descriptor;

void idt_init() {

}