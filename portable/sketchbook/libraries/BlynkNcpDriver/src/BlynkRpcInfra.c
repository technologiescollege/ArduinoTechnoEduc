
#include "BlynkRpc.h"

RpcStatus _rpc_status;

uint32_t _rpc_timeout_override = 0;

RpcStatus rpc_get_status(void) {
  return _rpc_status;
}

void rpc_set_timeout(uint32_t ms) {
  _rpc_timeout_override = ms;
}

void rpc_set_status(RpcStatus status) {
  _rpc_status = status;
}

const char* rpc_get_status_str(RpcStatus status) {
  switch (status) {
    case RPC_STATUS_OK             : return "OK";
    case RPC_STATUS_ERROR_GENERIC  : return "Generic error";
    case RPC_STATUS_ERROR_TIMEOUT  : return "Timeout";
    case RPC_STATUS_ERROR_MEMORY   : return "Memory failed";
    case RPC_STATUS_ERROR_UID      : return "Function not found";
    case RPC_STATUS_ERROR_ARGS_W   : return "Args writing failed";
    case RPC_STATUS_ERROR_ARGS_R   : return "Args reading failed";
    case RPC_STATUS_ERROR_RETS_W   : return "Rets writing failed";
    case RPC_STATUS_ERROR_RETS_R   : return "Rets reading failed";
  }
  return "unknown";
}

static
bool rpc_handle_msg(MessageBuffer* buff)
{
  uint16_t op;
  MessageBuffer_readUInt16(buff, &op);
  if (op == RPC_OP_INVOKE || op == RPC_OP_ONEWAY) {
      uint16_t id; uint16_t seq;
      MessageBuffer_readUInt16(buff, &id);
      if (op == RPC_OP_INVOKE) {
        MessageBuffer_readUInt16(buff, &seq);

        MessageWriter_begin();
        MessageWriter_writeUInt16(RPC_OP_RESULT);
        MessageWriter_writeUInt16(seq);
        rpc_invoke_handler(id, buff);
        MessageWriter_end();
      } else {
        rpc_invoke_handler(id, buff);
      }
  } else {
      return false;
  }

  return true;
}

RpcStatus rpc_wait_result(uint16_t expected_seq, MessageBuffer* buff, uint32_t timeout)
{
  if (_rpc_timeout_override) {
    timeout = _rpc_timeout_override;
  }
  uint8_t status = RPC_STATUS_ERROR_TIMEOUT;
  // TODO: timeout should decrease with time
  while (rpc_recv_msg(buff, timeout)) {
    uint16_t op = 0;
    MessageBuffer_readUInt16(buff, &op);
    if (op == RPC_OP_RESULT) {
      uint16_t seq = 0;
      MessageBuffer_readUInt16(buff, &seq);
      if (seq == expected_seq) {
        MessageBuffer_readUInt8(buff, &status);
        break;
      } else {
        // not our reply => skip it
        continue;
      }
    }
    // process unexpected messages
    MessageBuffer_rewind(buff);
    rpc_handle_msg(buff);
  }
  return (RpcStatus)status;
}

void rpc_run(void) {
  MessageBuffer buff;
  MessageBuffer_init(&buff, NULL, 0);
  while (rpc_recv_msg(&buff, 0)) {
    rpc_handle_msg(&buff);
  }
}
