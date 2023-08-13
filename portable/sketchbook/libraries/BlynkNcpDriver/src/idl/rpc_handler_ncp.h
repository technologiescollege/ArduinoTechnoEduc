
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_HANDLER_NCP_H
#define RPC_HANDLER_NCP_H

#ifdef __cplusplus
extern "C" {
#endif


void rpc_ncp_ping_impl(void);

static
void rpc_ncp_ping_handler(MessageBuffer* _rpc_buff) {
  (void)_rpc_buff;


  MessageWriter_writeUInt8(RPC_STATUS_OK);
}


bool rpc_ncp_reboot_impl(void);

static
void rpc_ncp_reboot_handler(MessageBuffer* _rpc_buff) {
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_ncp_reboot_impl();

  MessageWriter_writeUInt8(RPC_STATUS_OK);
  /* Serialize outputs */
  MessageWriter_writeBool(_rpc_ret_val);
}


bool rpc_ncp_hasUID_impl(uint16_t uid);

static
void rpc_ncp_hasUID_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint16_t uid; MessageBuffer_readUInt16(_rpc_buff, &uid);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_writeUInt8(RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_ncp_hasUID_impl(uid);

  MessageWriter_writeUInt8(RPC_STATUS_OK);
  /* Serialize outputs */
  MessageWriter_writeBool(_rpc_ret_val);
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_HANDLER_NCP_H */
