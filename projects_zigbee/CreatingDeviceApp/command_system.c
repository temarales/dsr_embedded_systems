#include "command_system.h"

static void zb_init_buffer(zb_uint8_t param, zb_uint8_t address, char* data, int data_size) 
{
  zb_buf_t *buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);

  zb_uint8_t *ptr = NULL;
  ZB_BUF_INITIAL_ALLOC(buf, data_size, ptr);
  memcpy(ptr, data, data_size);

  zb_apsde_data_req_t *req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
  req->dst_addr.addr_short = address; /* send to ZC */
  req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
  req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
  req->radius = 1;
  req->profileid = 2;
  req->src_endpoint = 10;
  req->dst_endpoint = 10;
  buf->u.hdr.handle = 0x11;
}

static void zb_send_command(zb_uint8_t param, zb_uint8_t command, int with_params) 
{
  zb_buf_t *buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
  user_info *info = ZB_GET_BUF_TAIL(buf, sizeof(user_info));
  
  if (with_params)
  { 
	//but now there is no such commands
  }
  else
  {
    simple_cp command_data;
    command_data.command_code = command;

    zb_init_buffer(param, info->device_address, &command_data, sizeof(simple_cp));
  }
  ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, ZB_REF_FROM_BUF(buf));
  
}

void zb_send_switch_mode(zb_uint8_t param) ZB_CALLBACK
{
   zb_send_command(param, SWITCH_MODE, 0);
}

void zb_send_brightness_up(zb_uint8_t param) ZB_CALLBACK
{
   zb_send_command(param, CHANGE_BRIGHTNESS_FOR_STEP_UP, 0);
}

void zb_send_switch_color(zb_uint8_t param) ZB_CALLBACK
{
   zb_send_command(param, SWITCH_COLOR, 0);
}

