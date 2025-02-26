/**
 * Martin Wuerms
 * 2024-10-05
 * use STM32L4
 */

/* - this file contains architecture specific definitions etc ... ----------- */

#ifndef _ARCH_H_
#define _ARCH_H_

#define ARCH_STM32L4

#ifdef ARCH_STM32L4
// - includes ------------------------------------------------------------------
#include <cmsis_gcc.h>
#include <stdint.h>

/**
 * STM32L4
 * #include <cmsis_gcc.h>
 * these functions are available
 * en/disable interrupts
 * __enable_irq() // enable interrupts
 * __disable_irq() // disable interrupts
 * __get_PRIMASK() // get PRIM
 * __set_PRIMASK(prim) // set PRIM
 *
 * go to sleep
 * __WFE() // go to sleep, Wait for Event
 * __WFI() // go to sleep, Wait for Interrupt
 * __NOP() // no operation
 */

// - define --------------------------------------------------------------------

/**
 * STM32L4 lock and restore global interrupts
 * see https://stackoverflow.com/questions/71626597/what-are-the-various-ways-to-disable-and-re-enable-interrupts-in-stm32-microcont
 * use if(__get_PRIMASK() & 0x01) //Global interrupts disabled

	uint32_t prim;
	prim = __get_PRIMASK(); // get PRIMASK
	__disable_irq(); // disable irqs

	// some uninterrupted code

	if (!prim) {
        __enable_irq();
    }
 */

// declare previously uint32_t prim;
#define lock_interrupt(x)		x = __get_PRIMASK(); \
								__disable_irq()

#define restore_interrupt(x)	if (!x) {__enable_irq();}
#else

// no specific arcitecture defined
#define lock_interrupt(x)
#define restore_interrupt(x)

#endif

#endif /* _ARCH_H_ */
