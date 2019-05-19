/***************************************************************************
*                      ZBOSS ZigBee Pro 2007 stack                         *
*                                                                          *
*          Copyright (c) 2012 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*          Copyright (c) 2011 ClarIDy Solutions, Inc., Taipei, Taiwan.     *
*                       http://www.claridy.com/                            *
*                                                                          *
*          Copyright (c) 2011 Uniband Electronic Corporation (UBEC),       *
*                             Hsinchu, Taiwan.                             *
*                       http://www.ubec.com.tw/                            *
*                                                                          *
*          Copyright (c) 2011 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*                                                                          *
*                                                                          *
* ZigBee Pro 2007 stack, also known as ZBOSS (R) ZB stack is available     *
* under either the terms of the Commercial License or the GNU General      *
* Public License version 2.0.  As a recipient of ZigBee Pro 2007 stack, you*
* may choose which license to receive this code under (except as noted in  *
* per-module LICENSE files).                                               *
*                                                                          *
* ZBOSS is a registered trademark of DSR Corporation AKA Data Storage      *
* Research LLC.                                                            *
*                                                                          *
* GNU General Public License Usage                                         *
* This file may be used under the terms of the GNU General Public License  *
* version 2.0 as published by the Free Software Foundation and appearing   *
* in the file LICENSE.GPL included in the packaging of this file.  Please  *
* review the following information to ensure the GNU General Public        *
* License version 2.0 requirements will be met:                            *
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.                   *
*                                                                          *
* Commercial Usage                                                         *
* Licensees holding valid ClarIDy/UBEC/DSR Commercial licenses may use     *
* this file in accordance with the ClarIDy/UBEC/DSR Commercial License     *
* Agreement provided with the Software or, alternatively, in accordance    *
* with the terms contained in a written agreement between you and          *
* ClarIDy/UBEC/DSR.                                                        *
*                                                                          *
****************************************************************************
PURPOSE: Test for ZC application written using ZDO.
*/


#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"

#include "command_system.h"

#ifndef ZB_ED_ROLE
#error define ZB_ED_ROLE to compile ze tests
#endif
/*! \addtogroup ZB_TESTS */
/*! @{ */
#ifndef ZB_SECURITY
#error Define ZB_SECURITY
#endif

#define ZB_TEST_DATA_SIZE 6


static void send_data(zb_buf_t *buf);
void data_indication(zb_uint8_t param) ZB_CALLBACK;
zb_ret_t dummy_d(zb_uint8_t param) ZB_CALLBACK;


static void send_set_orange_color(zb_uint8_t param) ZB_CALLBACK 
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  user_info *user_data;
  user_data = ZB_GET_BUF_TAIL(buf, sizeof(user_info));
  user_data->device_address = 0;
  user_data->choosen_parameter = ORANGE;
  ZB_SCHEDULE_ALARM(zb_send_switch_color, param, 1 * ZB_TIME_ONE_SECOND);
}

static void send_set_brightness(zb_uint8_t param) ZB_CALLBACK 
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  user_info *user_data;
  user_data = ZB_GET_BUF_TAIL(buf, sizeof(user_info));
  user_data->device_address = 0;
  user_data->choosen_parameter = 123;
  ZB_SCHEDULE_ALARM(zb_send_set_brightness, param, 1 * ZB_TIME_ONE_SECOND);
}

static void send_turn_on(zb_uint8_t param) ZB_CALLBACK 
{
  zb_buf_t *buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
  user_info *user_data;
  user_data = ZB_GET_BUF_TAIL(buf, sizeof(user_info));
  user_data->device_address = 0;
  ZB_SCHEDULE_ALARM(zb_send_turn_on, param, 1 * ZB_TIME_ONE_SECOND);
}


/*
  ZE joins to ZC(ZR), then sends APS packet.
*/

zb_ieee_addr_t g_ieee_addr = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

MAIN()
{
  ARGV_UNUSED;

#ifndef KEIL
  if ( argc < 3 )
  {
    printf("%s <read pipe path> <write pipe path>\n", argv[0]);
    return 0;
  }
#endif

  /* Init device, load IB values from nvram or set it to default */
#ifndef ZB8051
  ZB_INIT("zdo_ze", argv[1], argv[2]);
#else
  ZB_INIT("zdo_ze", "3", "3");
#endif

  ZB_IEEE_ADDR_COPY(ZB_PIB_EXTENDED_ADDRESS(), g_ieee_addr);

  if (zdo_dev_start() != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "zdo_dev_start failed", (FMT__0));
  }
  else
  {
    zdo_main_loop();
  }

  TRACE_DEINIT();
  MAIN_RETURN(0);
}


void zb_zdo_startup_complete(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  if (buf->u.hdr.status == 0)
  {
    TRACE_MSG(TRACE_APS1, "Device STARTED OK", (FMT__0));
    ZB_GET_OUT_BUF_DELAYED(send_turn_on);
    ZB_GET_OUT_BUF_DELAYED(send_set_brightness);
    ZB_GET_OUT_BUF_DELAYED(send_set_orange_color);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Device started FAILED status %d", (FMT__D, (int)buf->u.hdr.status));
    zb_free_buf(buf);
  }
}


/*! @} */
