//
//  LiveLoggingConnectionMAC.h
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

#import <Foundation/Foundation.h>
#import "SocketRocket.h"
#import "LiveLoggingConnectionImplMac.h"

@interface LiveLoggingConnectionMAC : NSObject
{
    // connection [
    
    NSString *_urlString;
    SRWebSocket *_ws;
    NSTimer *_reconnectTimer;

    // connection ]
    
    NSMutableArray *_sendQueue;
    int _sendQueueSize;
    
    int _cacheFirstId;
    int _cacheLastId;
    int _cacheSize;
}
@property (nonatomic, readonly) LiveLoggingConnectionImplMac *impl;
@property (nonatomic) int reconnectInterval;
@property (nonatomic) int sendQueueMemorySizeMax;
@property (nonatomic, retain) NSString *pathMessageCache;

- (void)cleanup;

- (void)connect:(NSString *)url;

- (void)send:(id)s;

+ (NSString *)wcsToString:(const wchar_t *)s;

@end

