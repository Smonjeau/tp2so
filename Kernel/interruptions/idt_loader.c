// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   IDT_LOADER.C    |                                                                               |
|--------------------                                                                               |
| This file has the functions that set up the IDT table, as well as the descriptor structure.   	|
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <defs.h>
#include <interrupts.h>
#include <syscall_handler.h>

#pragma pack(push)
#pragma pack (1)

typedef struct {
 uint16_t offset_l;
 uint16_t selector;
 uint8_t cero;
 uint8_t access;
 uint16_t offset_m;
 uint32_t offset_h;
 uint32_t other_cero;
} DESCR_INT;

#pragma pack(pop)

DESCR_INT * idt = (DESCR_INT *) 0;


void setup_IDT_entry (int index, uint64_t offset) {

    idt[index].offset_l = offset & 0xFFFF;
    idt[index].selector = 0x08;
    idt[index].cero = 0;
    idt[index].access = ACS_INT;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].other_cero = 0;

}


void load_idt() {

    _cli();
    
    setup_IDT_entry(0x20, (uint64_t) &_irq00Handler);
    setup_IDT_entry(0x21, (uint64_t) &_irq01Handler);

    setup_IDT_entry(0x80, (uint64_t) &_syscallDispatcher);

    setup_IDT_entry(0, (uint64_t) &_exception0Handler);
    setup_IDT_entry(1, (uint64_t) &_exception1Handler);
    setup_IDT_entry(2, (uint64_t) &_exception2Handler);
    setup_IDT_entry(3, (uint64_t) &_exception3Handler);
    setup_IDT_entry(4, (uint64_t) &_exception4Handler);
    setup_IDT_entry(5, (uint64_t) &_exception5Handler);
    setup_IDT_entry(6, (uint64_t) &_exception6Handler);
    setup_IDT_entry(7, (uint64_t) &_exception7Handler);
    setup_IDT_entry(8, (uint64_t) &_exception8Handler);
    setup_IDT_entry(9, (uint64_t) &_exception9Handler);
    setup_IDT_entry(10, (uint64_t) &_exception10Handler);
    setup_IDT_entry(11, (uint64_t) &_exception11Handler);
    setup_IDT_entry(12, (uint64_t) &_exception12Handler);
    setup_IDT_entry(13, (uint64_t) &_exception13Handler);
    setup_IDT_entry(14, (uint64_t) &_exception14Handler);
    setup_IDT_entry(15, (uint64_t) &_exception15Handler);
    setup_IDT_entry(16, (uint64_t) &_exception16Handler);
    setup_IDT_entry(17, (uint64_t) &_exception17Handler);
    setup_IDT_entry(18, (uint64_t) &_exception18Handler);
    setup_IDT_entry(19, (uint64_t) &_exception19Handler);
    setup_IDT_entry(20, (uint64_t) &_exception20Handler);
    setup_IDT_entry(21, (uint64_t) &_exception21Handler);
    setup_IDT_entry(22, (uint64_t) &_exception22Handler);
    setup_IDT_entry(23, (uint64_t) &_exception23Handler);
    setup_IDT_entry(24, (uint64_t) &_exception24Handler);
    setup_IDT_entry(25, (uint64_t) &_exception25Handler);
    setup_IDT_entry(26, (uint64_t) &_exception26Handler);
    setup_IDT_entry(27, (uint64_t) &_exception27Handler);
    setup_IDT_entry(28, (uint64_t) &_exception28Handler);
    setup_IDT_entry(29, (uint64_t) &_exception29Handler);
    setup_IDT_entry(30, (uint64_t) &_exception30Handler);
    setup_IDT_entry(31, (uint64_t) &_exception31Handler);
    
    picMasterMask(0xFC);
    picSlaveMask(0xFF);

    // _sti(); Luego se activa en el main del kernel

} 