//
//  EmiSocket.h
//  eminet
//
//  Created by Per Eckerdal on 2012-02-16.
//  Copyright (c) 2012 Per Eckerdal. All rights reserved.
//

#include "EmiTypes.h"
#import "EmiConnection.h"

#import <Foundation/Foundation.h>

@class EmiConnection;
@class GCDAsyncUdpSocket;
@class EmiSocketConfig;
@class EmiSocket;

@protocol EmiSocketDelegate <NSObject>
- (void)emiSocket:(EmiSocket *)socket gotConnection:(EmiConnection *)connection;

@optional

// This method is called immediately prior to emiSocket:gotConnection:.
// It allows a listening socket to specify the connectionQueue for a new accepted
// connection. If this method is not implemented, or returns NULL, the new accepted
// connection will create its own default queue.
// 
// Since you cannot autorelease a dispatch_queue, this method uses the "new" prefix
// in its name to specify that the returned queue has been retained.
// 
// Thus you could do something like this in the implementation:
//   return dispatch_queue_create("MyQueue", NULL);
// 
// If you are placing multiple conections on the same queue, then care should be
// taken to increment the retain count each time this method is invoked.
// 
// For example, your implementation might look something like this:
//   dispatch_retain(myExistingQueue);
//   return myExistingQueue;
- (dispatch_queue_t)newConnectionQueueForConnectionFromAddress:(NSData *)address
                                                   onEmiSocket:(EmiSocket *)sock;

@end

@interface EmiSocket : NSObject {
    // TODO Hey, isn't it fundamentally incorrect to have one socket
    // instance variable for resolving hosts, potentially in parallel?
    GCDAsyncUdpSocket *_resolveSocket;
    
    void *_sock;
    
	dispatch_queue_t _delegateQueue;
    id<EmiSocketDelegate> __unsafe_unretained _delegate;
	dispatch_queue_t _socketQueue;
}

// EmiNet uses the standard delegate paradigm, but executes all delegate
// callbacks on a given delegate dispatch queue. This allows for maximum
// concurrency, while at the same time providing easy thread safety.
// 
// The socket queue is optional.
// If you pass NULL, EmiNet will automatically create its own socket queue.
// If you choose to provide a socket queue, the socket queue must not be a
// concurrent queue.
// 
// The delegate queue and socket queue can optionally be the same.
- (id)init;
- (id)initWithSocketQueue:(dispatch_queue_t)sq;
- (id)initWithDelegate:(id<EmiSocketDelegate>)delegate delegateQueue:(dispatch_queue_t)dq;
- (id)initWithDelegate:(id<EmiSocketDelegate>)delegate delegateQueue:(dispatch_queue_t)dq socketQueue:(dispatch_queue_t)sq;

// Returns YES on success
- (BOOL)startWithError:(NSError **)error;
// Returns YES on success
- (BOOL)startWithConfig:(EmiSocketConfig *)config error:(NSError **)error;

- (BOOL)connectToAddress:(NSData *)address
                delegate:(__unsafe_unretained id<EmiConnectionDelegate>)delegate
           delegateQueue:(dispatch_queue_t)delegateQueue
                userData:(id)userData
                   error:(NSError **)errPtr;
// P2P connect.
- (BOOL)connectToAddress:(NSData *)address
                  cookie:(NSData *)cookie
            sharedSecret:(NSData *)sharedSecret
                delegate:(__unsafe_unretained id<EmiConnectionDelegate>)delegate
           delegateQueue:(dispatch_queue_t)delegateQueue
                userData:(id)userData
                   error:(NSError **)errPtr;
// If an obvious error is detected, this method immediately returns NO and sets err.
// If you don't care about the error, you can pass nil for errPtr.
// Otherwise, this method returns YES and begins the asynchronous connection process.
- (BOOL)connectToHost:(NSString *)host
               onPort:(uint16_t)port
             delegate:(__unsafe_unretained id<EmiConnectionDelegate>)delegate
        delegateQueue:(dispatch_queue_t)delegateQueue
             userData:(id)userData
                error:(NSError **)errPtr;
// P2P connect.
// 
// If an obvious error is detected, this method immediately returns NO and sets err.
// If you don't care about the error, you can pass nil for errPtr.
// Otherwise, this method returns YES and begins the asynchronous connection process.
- (BOOL)connectToHost:(NSString *)host
               onPort:(uint16_t)port
               cookie:(NSData *)cookie
         sharedSecret:(NSData *)sharedSecret
             delegate:(__unsafe_unretained id<EmiConnectionDelegate>)delegate
        delegateQueue:(dispatch_queue_t)delegateQueue
             userData:(id)userData
                error:(NSError **)errPtr;

// Synchronously sets both the delegate and the delegate queue
- (void)setDelegate:(id<EmiConnectionDelegate>)delegate
      delegateQueue:(dispatch_queue_t)delegateQueue;

@property (nonatomic, readonly, unsafe_unretained) id<EmiSocketDelegate> delegate;
@property (nonatomic, readonly, assign) dispatch_queue_t delegateQueue;
@property (nonatomic, readonly, assign) dispatch_queue_t socketQueue;

@property (nonatomic, readonly, strong) NSData *serverAddress;
@property (nonatomic, readonly, assign) EmiTimeInterval connectionTimeout;
@property (nonatomic, readonly, assign) float heartbeatsBeforeConnectionWarning;
@property (nonatomic, readonly, assign) NSUInteger receiverBufferSize;
@property (nonatomic, readonly, assign) NSUInteger senderBufferSize;
@property (nonatomic, readonly, assign) BOOL acceptConnections;
@property (nonatomic, readonly, assign) uint16_t serverPort;
@property (nonatomic, readonly, assign) NSUInteger MTU;
@property (nonatomic, readonly, assign) float heartbeatFrequency;

@end