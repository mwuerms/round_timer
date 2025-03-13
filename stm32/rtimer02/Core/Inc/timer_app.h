/**
 * timer_app, timer application
 * Martin, 2025-03-05
 */

#ifndef _TIMER_APP_H_
#define _TIMER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "cmsis_os2.h"

/**
 * init timer_app
 */
void timer_app_init(void);

/**
 * draw timer_app background, basic structure
 */
void timer_app_draw(void);

#ifdef __cplusplus
}
#endif


#endif // _TIMER_APP_H_
