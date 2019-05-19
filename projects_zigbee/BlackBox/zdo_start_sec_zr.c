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
PURPOSE:
*/

#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"

#ifndef ZB_ROUTER_ROLE
#error Router role is not compiled!
#endif

#ifndef ZB_SECURITY
#error Define ZB_SECURITY
#endif


/*! \addtogroup ZB_TESTS */
/*! @{ */

static void send_data(zb_uint8_t param);
static void ieee_address_request(zb_uint8_t param);
static void power_mode_description_request(zb_uint8_t param);
static void active_end_points_request(zb_uint8_t param);
static void simple_desciption_of_endpoint_request(zb_uint8_t param);

void node_power_desc_callback(zb_uint8_t param);
void ieee_addr_callback(zb_uint8_t param);
void active_ep_callback(zb_uint8_t param);
void simple_desc_callback(zb_uint8_t param);
#ifndef APS_RETRANSMIT_TEST
void data_indication(zb_uint8_t param) ZB_CALLBACK;
#endif

/*
  ZR joins to ZC, then sends APS packet.
 */

zb_ieee_addr_t g_ieee_addr = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};


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
  ZB_INIT("zdo_zr", argv[1], argv[2]);
#else
  ZB_INIT("zdo_zr", "2", "2");
#endif
#ifdef ZB_SECURITY
  ZG->nwk.nib.security_level = 0;
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
#ifndef APS_RETRANSMIT_TEST
    zb_af_set_data_indication(data_indication);
#endif
    
    ZB_SCHEDULE_CALLBACK(ieee_address_request, 0);
    /*ZB_SCHEDULE_CALLBACK(active_end_points_request, 0);
    ZB_SCHEDULE_CALLBACK(simple_desciption_of_endpoint_request, 0);
    ZB_SCHEDULE_CALLBACK(power_mode_description_request, 0);
    ZB_SCHEDULE_CALLBACK(send_data, 0);*/
    ZB_SCHEDULE_ALARM(active_end_points_request, 0, 2*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(simple_desciption_of_endpoint_request, 0, 4*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(power_mode_description_request, 0, 6*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(send_data, 0, 8*ZB_TIME_ONE_SECOND);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Device started FAILED status %d", (FMT__D, (int)buf->u.hdr.status));
    zb_free_buf(buf);
  }
}

static void ieee_address_request(zb_uint8_t param) ZB_CALLBACK
{
  if (param == 0)
  {
      ZB_GET_OUT_BUF_DELAYED(ieee_address_request);
      return;
  }  
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_ieee_addr_req_t *req = NULL;
  ZB_BUF_INITIAL_ALLOC(buf, sizeof(zb_zdo_ieee_addr_req_t), req);

  req->nwk_addr = 0; //Отсылаем координатору
  req->request_type = ZB_ZDO_SINGLE_DEV_RESPONSE;
  req->start_index = 0;
  zb_zdo_ieee_addr_req(ZB_REF_FROM_BUF(buf), ieee_addr_callback);

}

static void active_end_points_request(zb_uint8_t param) ZB_CALLBACK
{
 if (param == 0)
  {
      ZB_GET_OUT_BUF_DELAYED(active_end_points_request);
      return;
  }  
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_active_ep_req_t *req;
  ZB_BUF_INITIAL_ALLOC(buf, sizeof(zb_zdo_active_ep_req_t), req);
  req->nwk_addr = 0; //coord addr
  zb_zdo_active_ep_req(ZB_REF_FROM_BUF(buf), active_ep_callback);
}

static void power_mode_description_request(zb_uint8_t param) ZB_CALLBACK
{
  if (param == 0)
  {
      ZB_GET_OUT_BUF_DELAYED(power_mode_description_request);
      return;
  }  
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_power_desc_req_t *req;
  ZB_BUF_INITIAL_ALLOC(buf, sizeof(zb_zdo_power_desc_req_t), req);
  req->nwk_addr = 0;
  zb_zdo_power_desc_req(ZB_REF_FROM_BUF(buf), node_power_desc_callback);
}

static void simple_desciption_of_endpoint_request(zb_uint8_t param) ZB_CALLBACK
{
  if (param == 0)
  {
      ZB_GET_OUT_BUF_DELAYED(simple_desciption_of_endpoint_request);
      return;
  }  
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_simple_desc_req_t *req;
  ZB_BUF_INITIAL_ALLOC(buf, sizeof(zb_zdo_simple_desc_req_t), req);
  req->nwk_addr = 0; //send to coordinator
  req->endpoint = 11;
  zb_zdo_simple_desc_req(ZB_REF_FROM_BUF(buf), simple_desc_callback);
}

void simple_desc_callback(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t *zdp_cmd = ZB_BUF_BEGIN(buf);
  zb_zdo_simple_desc_resp_t *resp = (zb_zdo_simple_desc_resp_t*)(zdp_cmd);
  zb_uint_t i;

  TRACE_MSG(TRACE_APS1, "RESPONSE: simple description of endpoint received from 0x%x",
            (FMT__H, resp->hdr.nwk_addr));
  if (resp->hdr.status != ZB_ZDP_STATUS_SUCCESS || resp->hdr.nwk_addr != 0x0)
  {
    TRACE_MSG(TRACE_APS1, "Error incorrect status/addr", (FMT__0));
  }
  
  zb_uint_t input_clusters_count = resp->simple_desc.app_input_cluster_count;
  zb_uint_t output_clusters_count = *(&resp->simple_desc.app_output_cluster_count + input_clusters_count * 2);

  TRACE_MSG(TRACE_APS1, "ep %hd, app prof %d, dev id %d, dev ver %hd, input count 0x%hx, output count 0x%hx",
            (FMT__H_D_D_H_H_H, resp->simple_desc.endpoint, resp->simple_desc.app_profile_id,
            resp->simple_desc.app_device_id, resp->simple_desc.app_device_version,
           input_clusters_count, output_clusters_count));

  TRACE_MSG(TRACE_APS1, "input clusters:", (FMT__0));
  for(i = 0; i < input_clusters_count; i++)
  {
    TRACE_MSG(TRACE_APS1, "0x%hx%hx", (FMT__D, *(&resp->simple_desc.app_output_cluster_count + 1 + i), *(&resp->simple_desc.app_output_cluster_count + i) ));
  }
  
  TRACE_MSG(TRACE_APS1, "output clusters:", (FMT__0));
  
  for(i = 0; i < output_clusters_count * 2; i += 2)
  {
    TRACE_MSG(TRACE_APS1, "0x%hx%hx", (FMT__D, *(&resp->simple_desc.app_output_cluster_count + input_clusters_count * 2 + 2 + i), *(&resp->simple_desc.app_output_cluster_count + input_clusters_count * 2 + 1 + i)));
  }

  zb_free_buf(buf);
}

void active_ep_callback(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t *zdp_cmd = ZB_BUF_BEGIN(buf);
  zb_zdo_ep_resp_t *resp = (zb_zdo_ep_resp_t*)zdp_cmd;
  zb_uint8_t *ep_list = zdp_cmd + sizeof(zb_zdo_ep_resp_t);

  TRACE_MSG(TRACE_APS1, "RESPONSE: active end points from  0x%x received",
            (FMT__H, resp->nwk_addr));

  if (resp->status != ZB_ZDP_STATUS_SUCCESS || resp->nwk_addr != 0x0)
  {
    TRACE_MSG(TRACE_APS1, "Error incorrect status/addr", (FMT__0));
  }
  TRACE_MSG(TRACE_APS1, " ep count %hd, ep %hd", (FMT__H_H, resp->ep_count, *ep_list));

  zb_free_buf(buf);
}

void node_power_desc_callback(zb_uint8_t param) ZB_CALLBACK //Напечатает power_mode
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t *zdp_cmd = ZB_BUF_BEGIN(buf);
  zb_zdo_power_desc_resp_t *resp = (zb_zdo_power_desc_resp_t*)(zdp_cmd);

  TRACE_MSG(TRACE_APS1, "RESPONSE: power mode received", (FMT__H));

  TRACE_MSG(TRACE_APS1, "power mode %hd, avail power src %hd, cur power src %hd, cur power level %hd",
            (FMT__H_H_H_H, ZB_GET_POWER_DESC_CUR_POWER_MODE(&resp->power_desc),
             ZB_GET_POWER_DESC_AVAIL_POWER_SOURCES(&resp->power_desc),
             ZB_GET_POWER_DESC_CUR_POWER_SOURCE(&resp->power_desc),
             ZB_GET_POWER_DESC_CUR_POWER_SOURCE_LEVEL(&resp->power_desc)));
  zb_free_buf(buf);
}

void ieee_addr_callback(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_nwk_addr_resp_head_t *resp;

  resp = (zb_zdo_nwk_addr_resp_head_t*)ZB_BUF_BEGIN(buf);
  TRACE_MSG(TRACE_ZDO2, "RESPONSE: IEEE address received", (FMT__H_D));
  ZB_DUMP_IEEE_ADDR(resp->ieee_addr);
  zb_free_buf(buf);
}

static void send_data(zb_uint8_t param) ZB_CALLBACK
{
  if (param == 0)
  {
      ZB_GET_OUT_BUF_DELAYED(send_data);
      return;
  }  
  zb_apsde_data_req_t *req;
  zb_buf_t *buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);

  req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
  req->dst_addr.addr_short = 0;
  req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
  req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
  req->radius = 5;
  req->profileid = 2;
  req->clusterid = 0xaaaa;
  req->src_endpoint = 11;
  req->dst_endpoint = 11;
  buf->u.hdr.handle = 0x11;

  TRACE_MSG(TRACE_APS2, "Sending apsde_data.request", (FMT__0));

  ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, ZB_REF_FROM_BUF(buf));
}

#ifndef APS_RETRANSMIT_TEST
void data_indication(zb_uint8_t param)
{
  zb_ushort_t i;
  zb_uint8_t *ptr;
  zb_buf_t *asdu = (zb_buf_t *)ZB_BUF_FROM_REF(param);

  /* Remove APS header from the packet */
  ZB_APS_HDR_CUT_P(asdu, ptr);

  TRACE_MSG(TRACE_APS2, "data_indication: packet %p len %d handle 0x%x", (FMT__P_D_D,
                         asdu, (int)ZB_BUF_LEN(asdu), asdu->u.hdr.status));
  TRACE_MSG(TRACE_APS2, "RESPONSE: some data has returned", (FMT__P_D_D,
                         asdu, (int)ZB_BUF_LEN(asdu), asdu->u.hdr.status));

  for (i = 0 ; i < ZB_BUF_LEN(asdu) ; ++i)
  {
    TRACE_MSG(TRACE_APS2, "%x %c", (FMT__D_C, (int)ptr[i], ptr[i]));
    if (ptr[i] != i % 32 + '0')
    {
      TRACE_MSG(TRACE_ERROR, "Bad data %hx %c wants %x %c", (FMT__H_C_D_C, ptr[i], ptr[i],
                              (int)(i % 32 + '0'), (char)i % 32 + '0'));
    }
  }

}
#endif

/*! @} */
