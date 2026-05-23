#include "idt.h"

struct IDTEntry idt[256];
struct IDTPointer idt_ptr;

void idt_init()
{
    idt_ptr.limit=
    sizeof(idt)-1;

    idt_ptr.base=
    (uint32_t)&idt;

    __asm__ volatile(
        "lidt %0"
        :
        : "m"(idt_ptr)
    );
}