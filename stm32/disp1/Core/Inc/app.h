/**
 * app, main application
 * Martin, 2025-02-26
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

extern uint8_t app_ev_task_id;

void app_init(void);

#ifdef __cplusplus
}
#endif

#endif // INC_APP_H_
