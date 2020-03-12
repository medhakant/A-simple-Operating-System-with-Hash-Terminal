#pragma once

#include "labs/shell.h"
#include "util/fiber.h"
#include "labs/preempt.h"
#include "x86/except.h"


void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack,preempt_t timer,addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t lapic);
void fnprime(addr_t* pmain_stack, addr_t* pf_stack, int* i, int* count);