#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H 

#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"

typedef struct simple_command_package{
 zb_int8_t command_code;
} simple_cp;

typedef struct info_from_user {
 zb_int8_t device_address;
} simple_user_info;


enum commands {
 SWITCH_MODE,
 CHANGE_BRIGHTNESS_FOR_STEP_UP,
 SWITCH_COLOR,
};


void zb_send_switch_mode(zb_uint8_t param) ZB_CALLBACK;
void zb_send_brightness_up(zb_uint8_t param) ZB_CALLBACK;
void zb_send_switch_color(zb_uint8_t param) ZB_CALLBACK;

#endif
