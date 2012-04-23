//
//  EmiSockConfig.h
//  roshambo
//
//  Created by Per Eckerdal on 2012-04-23.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef roshambo_EmiSockConfig_h
#define roshambo_EmiSockConfig_h

#include "EmiTypes.h"

template<class Address>
class EmiSockConfig {
public:
    EmiSockConfig() :
    mtu(EMI_DEFAULT_MTU),
    heartbeatFrequency(EMI_DEFAULT_HEARTBEAT_FREQUENCY),
    tickFrequency(EMI_DEFAULT_TICK_FREQUENCY),
    connectionTimeout(EMI_DEFAULT_CONNECTION_TIMEOUT),
    heartbeatsBeforeConnectionWarning(EMI_DEFAULT_HEARTBEATS_BEFORE_CONNECTION_WARNING),
    receiverBufferSize(EMI_DEFAULT_RECEIVER_BUFFER_SIZE),
    senderBufferSize(EMI_DEFAULT_SENDER_BUFFER_SIZE),
    acceptConnections(false),
    port(0),
    address(),
    fabricatedPacketDropRate(0) {}
    
    size_t mtu;
    float heartbeatFrequency;
    float tickFrequency;
    EmiTimeInterval connectionTimeout;
    float heartbeatsBeforeConnectionWarning;
    size_t receiverBufferSize;
    size_t senderBufferSize;
    bool acceptConnections;
    uint16_t port;
    Address address;
    float fabricatedPacketDropRate;
};

#endif