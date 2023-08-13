#include "MessageBuffer.h"
#include "MessageWriter.h"
#include "BlynkRpcUartFraming.h"

size_t MessageWriter_writeString(const char* value) {
    if (!value) {
        return MessageWriter_write("", 1);
    }
    size_t length = strlen(value) + 1;
    return MessageWriter_write(value, length);
}

size_t MessageWriter_writeBinary(buffer_t value) {
    size_t len = sizeof(uint16_t) + value.length;
    /*if (MessageBuffer_availableToWrite(self) < len) {
        self->_error = true;
        return 0;
    }*/
    MessageWriter_writeUInt16(value.length);
    MessageWriter_write(value.data, value.length);
    return len;
}

size_t MessageWriter_writeBool(bool value) {
    return MessageWriter_writeUInt8(value ? 1 : 0);
}

size_t MessageWriter_writeInt8(const int8_t value) {
    return MessageWriter_write(&value, sizeof(int8_t));
}

size_t MessageWriter_writeInt16(const int16_t value) {
    return MessageWriter_write(&value, sizeof(int16_t));
}

size_t MessageWriter_writeInt32(const int32_t value) {
    return MessageWriter_write(&value, sizeof(int32_t));
}

size_t MessageWriter_writeInt64(const int64_t value) {
    return MessageWriter_write(&value, sizeof(int64_t));
}

size_t MessageWriter_writeUInt8(const uint8_t value) {
    return MessageWriter_write(&value, sizeof(uint8_t));
}

size_t MessageWriter_writeUInt16(const uint16_t value) {
    return MessageWriter_write(&value, sizeof(uint16_t));
}

size_t MessageWriter_writeUInt32(const uint32_t value) {
    return MessageWriter_write(&value, sizeof(uint32_t));
}

size_t MessageWriter_writeUInt64(const uint64_t value) {
    return MessageWriter_write(&value, sizeof(uint64_t));
}

size_t MessageWriter_writeFloat(const float value) {
    return MessageWriter_write(&value, sizeof(float));
}

size_t MessageWriter_begin(void) {
    RpcUartFraming_beginPacket();
    return 1;
}
size_t MessageWriter_end(void) {
    RpcUartFraming_endPacket();
    return 1;
}

size_t MessageWriter_write(const void* data, size_t size) {
    /*if (!data || MessageBuffer_availableToWrite(self) < size) {
        self->_error = true;
        return 0;
    }
    memcpy(self->_buffer + self->_wpos, data, size);
    self->_wpos += size;*/

    return RpcUartFraming_write((const uint8_t*)data, size);
}
