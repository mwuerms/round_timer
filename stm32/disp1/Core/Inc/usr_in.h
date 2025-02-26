/**
 * module usr_in "User Inputs"
 * Martin, 2025-02-26
 */

#ifndef _USR_IN_H_
#define _USR_IN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

// - events
/*
#define EV_USR_IN_BTN0
#define EV_USR_IN_BTN1
#define EV_USR_IN_BTN2
#define EV_USR_IN_ENC_LEFT
#define EV_USR_IN_ENC_RIGHT
*/
#define EV_BTN0_PRESSED (1)
#define EV_BTN0_RELEASED (2)

// - public functions ---------
void usr_in_init(void);

#define USR_IN_BTN0_MASK (1 << 0)
#define USR_IN_BTN1_MASK (1 << 1)
#define USR_IN_BTN2_MASK (1 << 2)
#define USR_IN_ENC_MASK (1 << 3)
void usr_in_enable(uint32_t mask);
void usr_in_disable(uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif // _USR_IN_H_
