//
//  EmiDispatchTimer.mm
//  eminet
//
//  Created by Per Eckerdal on 2012-06-12.
//  Copyright (c) 2012 Per Eckerdal. All rights reserved.
//

#include "EmiDispatchTimer.h"

#include "EmiObjCBindingHelper.h"
#include "EmiNetUtil.h"

#import <Foundation/Foundation.h>

EmiDispatchTimer::EmiDispatchTimer(dispatch_queue_t timerCookie) :
_timerQueue(timerCookie), _timer(NULL) {
    ASSERT(_timerQueue);
}

EmiDispatchTimer::~EmiDispatchTimer() {
    deschedule();
}

void EmiDispatchTimer::deschedule_() {
    if (_timer) {
        dispatch_source_cancel(_timer);
        _timer = NULL;
    }
}

void EmiDispatchTimer::schedule_(TimerCb *timerCb, void *data, EmiTimeInterval interval,
                                 bool repeating, bool reschedule) {
    if (!reschedule && _timer) {
        // We were told not to re-schedule the timer. 
        // The timer is already active, so do nothing.
        return;
    }
    
    if (_timer) {
        dispatch_source_cancel(_timer);
    }
    
    _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, NULL, /*mask:*/0, _timerQueue);
    
    dispatch_source_set_timer(_timer, dispatch_time(DISPATCH_TIME_NOW, interval * NSEC_PER_SEC),
                              interval * NSEC_PER_SEC, /*leeway:*/0);
    EmiDispatchTimer *timer = this;
    
    dispatch_source_set_event_handler(_timer, ^{
        if (!repeating) {
            timer->deschedule_();
        }
        
        timerCb([NSDate timeIntervalSinceReferenceDate], timer, data);
    });
    
    dispatch_resume(_timer);
}

void EmiDispatchTimer::schedule(TimerCb *timerCb, void *data, EmiTimeInterval interval,
                                bool repeating, bool reschedule) {
    DISPATCH_SYNC(_timerQueue, ^{
        schedule_(timerCb, data, interval, repeating, reschedule);
    });
}

void EmiDispatchTimer::deschedule() {
    DISPATCH_SYNC(_timerQueue, ^{
        deschedule_();
    });
}
