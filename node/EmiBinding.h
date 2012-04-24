#define BUILDING_NODE_EXTENSION
#ifndef emilir_EmiBinding_h
#define emilir_EmiBinding_h

#include "EmiError.h"

#include "../core/EmiTypes.h"
#include <node.h>
#include <node_buffer.h>
#include <uv.h>

class EmiAddressCmp;

class EmiBinding {
private:
    inline EmiBinding();
public:
    
    typedef EmiError                   Error;
    typedef EmiAddressCmp              AddressCmp;
    typedef uv_udp_t                   SocketHandle;
    typedef struct sockaddr_storage    Address;
    typedef v8::Local<v8::Object>      TemporaryData;
    typedef v8::Persistent<v8::Object> PersistentData;
    
    static void panic();
    
    inline static EmiError makeError(const char *domain, int32_t code) {
        return EmiError(domain, code);
    }
    
    static v8::Persistent<v8::Object> makePersistentData(const v8::Local<v8::Object>& data,
                                                         size_t offset,
                                                         size_t length);
    inline static void releasePersistentData(v8::Persistent<v8::Object> buf) {
        buf.Dispose();
    }
    inline static v8::Local<v8::Object> castToTemporary(const v8::Persistent<v8::Object>& data) {
        return v8::Local<v8::Object>::New(data);
    }
    
    inline static const uint8_t *extractData(v8::Handle<v8::Object> data) {
        return (const uint8_t *)(data.IsEmpty() ? NULL : node::Buffer::Data(data));
    }
    inline static size_t extractLength(v8::Handle<v8::Object> data) {
        return data.IsEmpty() ? 0 : node::Buffer::Length(data);
    }
    
    static const size_t HMAC_HASH_SIZE = 32;
    static void hmacHash(const uint8_t *key, size_t keyLength,
                         const uint8_t *data, size_t dataLength,
                         uint8_t *buf, size_t bufLen);
    static void randomBytes(uint8_t *buf, size_t bufSize);
};

#endif