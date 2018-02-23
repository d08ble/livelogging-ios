//
//  LiveLoggingConnectionMAC.m
//  livelogging-ios
//
//  Copyright (c) 2018 d08ble. All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//

#import "LiveLoggingConnectionMAC.h"
#import <iostream>

using namespace acpul;

// runloop [

void __runloop_run()
{
    NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
    [runLoop run];
}

// runloop ]
// factory [

ILiveLoggingConnection *acpul::LiveLoggingConnectionCreate()
{
//    LiveLoggingConnectionImplMac *c = (LiveLoggingConnectionImplMac *)[LiveLoggingConnectionMAC new].impl;
//    return c;
    return [LiveLoggingConnectionMAC new].impl;
}

void acpul::LiveLoggingConnectionDelete(ILiveLoggingConnection *c)
{
    LiveLoggingConnectionImplMac *c1 = (LiveLoggingConnectionImplMac *)c;
    LiveLoggingConnectionMAC *nc = (__bridge LiveLoggingConnectionMAC *)(c1->_id);
    [nc cleanup];
}

// factory ]
// impl [

void LiveLoggingConnectionImplMac::connect(const wchar_t *url)
{
    LiveLoggingConnectionMAC *nc = (__bridge LiveLoggingConnectionMAC *)(_id);
    
    NSString *s = [LiveLoggingConnectionMAC wcsToString:url];

    [nc connect:s];
}

#if defined(__BIG_ENDIAN__)
# define WCHAR_ENCODING NSUTF32BigEndianStringEncoding
#elif defined(__LITTLE_ENDIAN__)
# define WCHAR_ENCODING NSUTF32LittleEndianStringEncoding
#endif

void LiveLoggingConnectionImplMac::send(const wchar_t *path, const wchar_t *text, const wchar_t *type)
{
    @autoreleasepool {
        LiveLoggingConnectionMAC *nc = (__bridge LiveLoggingConnectionMAC *)(_id);

        static NSDateFormatter *dateFormat = nil;
        if (!dateFormat) {
            dateFormat = [NSDateFormatter new];
            [dateFormat setDateFormat: @"yyyy-MM-dd HH:mm:ss"];
        }
        NSDictionary *dict = @{
                               @"time": [dateFormat stringFromDate:[NSDate new]],
                               @"path": [LiveLoggingConnectionMAC wcsToString:path],
                               @"data": [LiveLoggingConnectionMAC wcsToString:text],
                               @"type": [LiveLoggingConnectionMAC wcsToString:type],
                               };

        [nc send:dict];
    }
}

// impl

static NSMutableArray *_poolConnections = nil;

@implementation LiveLoggingConnectionMAC

- (id)init;
{
    if ([super init]) {
        
        [LiveLoggingConnectionMAC poolAddConnection:self];
        
        _sendQueue = [NSMutableArray new];
        
        _impl = new LiveLoggingConnectionImplMac();
        _impl->_id = (__bridge void *)self;

        self.reconnectInterval = 2.0;
        
        self.sendQueueMemorySizeMax = 1000000;
        _sendQueueSize = 0;
        
        _cacheFirstId = 0;
        _cacheLastId = 0;
        
        [self initCache];
    }
    return self;
}

- (void)cleanup
{
    [LiveLoggingConnectionMAC poolRemoveConnection:self];
    if (_impl) {
        delete _impl;
        _impl = nil;
    }
}

// pool [

+ (void)poolAddConnection:(LiveLoggingConnectionMAC *)connection
{
    if (!_poolConnections)
        _poolConnections = [NSMutableArray new];
    [_poolConnections addObject:connection];
}

+ (void)poolRemoveConnection:(LiveLoggingConnectionMAC *)connection
{
    if (_poolConnections)
        [_poolConnections removeObject:connection];
}

// pool ]

- (void)connect:(NSString *)url
{
    _urlString = url;
    [self reconnect];
}

// reconnect [

- (void)reconnect;
{
    [self stopReconnectTimer];
    
    _ws.delegate = nil;
    [_ws close];
    
    _ws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:_urlString]]];
    _ws.delegate = self;
    
    [_ws open];
}

// reconnect ]
// send [

- (void)send:(id)s;
{
    static int i = 0;
    NSLog(@"send %i", i++);
//    return;
    @synchronized (self) {
        int size = 0;
        if ([s isKindOfClass:[NSDictionary class]]) {
            //convert object to data
            NSError* error;
            NSData *data = [NSJSONSerialization dataWithJSONObject:s
                                                options:NSJSONWritingPrettyPrinted
                                                  error:&error];
            if (error) {
                NSLog(@"error:%@", error);
                return;
            }
            s = data;
            size = data.length;
//            if (size > 100)
//                return;
        }
/*        else if ([s isKindOfClass:[NSString class]]) {
            size = ([(NSString *)s length] + 1) * 4; // todo: recheck sizeof wchar_t
        }*/
        else {
            NSLog(@"send bad type");
        }
//        return;
        [self cacheDrain];
        
        if (_sendQueueSize < self.sendQueueMemorySizeMax) {
            _sendQueueSize += size;
            [_sendQueue addObject:s];
        }
        else {
            [self addToCache:s];
        }
        
    }

    if (_ws.readyState == SR_OPEN)
    {
        [self sendQueue];
    }
    else {
//        NSLog(@"error %@", s);
    }
}

- (void)sendQueue;
{
    @synchronized (self) {
        
        for (id v in _sendQueue ) {
            _sendQueueSize -= [v length];
            
            [_ws send:v];
        }
        
        [_sendQueue removeAllObjects];
    }
}

// disk cache [

- (void)initCache
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingPathComponent:@"liveloggingcache"];
    NSError *error = nil;
    [[NSFileManager defaultManager] removeItemAtPath:path error:&error];
    error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:NO attributes:nil error:&error];
    if (error) {
        NSLog(@"LiveLoggingConnectionMAC.initCache error: %@", error);
    }
    
    self.pathMessageCache = path;
}

- (void)addToCache:(NSData *)s
{
    int fid = _cacheLastId;

    NSString *file = [self.pathMessageCache stringByAppendingPathComponent:[NSString stringWithFormat:@"%i.message", fid]];
    
    [s writeToFile:file atomically:YES];
    
    _cacheSize += s.length;
    
    _cacheLastId++;
}

- (void)cacheDrain
{
    if (_sendQueueSize < self.sendQueueMemorySizeMax) {
        if (_cacheFirstId < _cacheLastId) {
            for (; _cacheFirstId < _cacheLastId;) {
                int fid = _cacheFirstId++;
                
                NSString *file = [self.pathMessageCache stringByAppendingPathComponent:[NSString stringWithFormat:@"%i.message", fid]];
                
                NSData *data = [NSData dataWithContentsOfFile:file];
                
                NSError *error = nil;
                [[NSFileManager defaultManager] removeItemAtPath:file error:&error];

                int size = data.length;
                
                [_sendQueue addObject:data];
                
                _cacheSize -= size;
                _sendQueueSize += size;
                if (_sendQueueSize > self.sendQueueMemorySizeMax)
                    break;
            }
        }
        else {
            _cacheFirstId = _cacheLastId = 0;
        }
    }
}

// disk cache ]

// send ]
// reconnect timer [

- (void)restartReconnectTimer;
{
    [self stopReconnectTimer];
    // start [
    _reconnectTimer = [NSTimer scheduledTimerWithTimeInterval:self.reconnectInterval
                                                       target:self
                                                     selector:@selector(onReconnectTimer:)
                                                     userInfo:nil
                                                      repeats:NO];
    // start ]
}

// stop [

- (void)stopReconnectTimer;
{
    [_reconnectTimer invalidate];
    _reconnectTimer = nil;
}

// stop ]
// event [

- (void)onReconnectTimer:(id)sender;
{
    _reconnectTimer = nil;
    [self reconnect];
}

// event ]
// reconnect timer ]
// WS-Events [
// recv [

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message;
{
/*    @try {
        NSError *error = nil;
        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:[message dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers //|NSJSONReadingAllowFragments|NSJSONReadingMutableLeaves
                                                               error:&error];
        if (error) {
            NSLog(@"ACPUNet error:%@ message:%@", error, message);
            //            NSLog(@"error:%@", error);
            return;
        }
        NSString *cmd = [dict objectForKey:@"cmd"];
        if ([cmd isEqualToString:@"apply"]) {
            [self notify:dict];
        }
        else if ([cmd isEqual:@"_data"]) {
            [self processData:dict];
        }
        else if ([cmd isEqual:@"stream"]) {
            [self streamProcess:dict];
        }
    }
    @catch (NSException *exception) {
        NSLog(@"ACPUNet exception:%@ message:%@", exception, message);
    }*/
}

// notify [

- (void)notify:(id)dict;
{
/*    [[NSNotificationCenter defaultCenter] postNotification:
     [NSNotification notificationWithName:ACPUNetNotification
                                   object:nil
                                 userInfo:dict]]; //@{@"status":status}*/
}

// notify ]
// recv ]
// opened [

- (void)webSocketDidOpen:(SRWebSocket *)webSocket;
{
    [self sendQueue];
}

// opened ]
// error [

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error;
{
    // error
    [self error:-1 reason:[NSString stringWithFormat:@"%@", error ]];
}

// error ]
// closed [

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean;
{
    // error
    [self error:code reason:reason];
}

// closed ]
// WS-Events ]

- (void)error:(NSInteger)code reason:(NSString *)reason;
{
    NSLog(@"WS_ERROR: %i %@", (int)code, reason);
    [self restartReconnectTimer];
}

#if defined(__BIG_ENDIAN__)
# define WCHAR_ENCODING NSUTF32BigEndianStringEncoding
#elif defined(__LITTLE_ENDIAN__)
# define WCHAR_ENCODING NSUTF32LittleEndianStringEncoding
#endif

+ (NSString *)wcsToString:(const wchar_t *)s;
{
    return [[NSString alloc] initWithBytes:s
                                    length:wcslen(s)*sizeof(wchar_t) encoding:WCHAR_ENCODING];
}
@end
