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

// task ID to receive events
extern uint8_t app_ev_task_id;
// events
#define EV_BTN0_PRESSED (1)
#define EV_BTN0_RELEASED (2)
#define EV_BTN1_PRESSED (3)
#define EV_BTN1_RELEASED (4)
#define EV_BTN2_PRESSED (5)
#define EV_BTN2_RELEASED (6)
#define EV_ENC_LEFT (7)
#define EV_ENC_RIGHT (8)

void app_init(void);

#ifdef __cplusplus
}
#endif

#endif // INC_APP_H_
