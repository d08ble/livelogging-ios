//
//  LiveLoggingConnectionImplMac.h
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

#ifndef __LiveLoggingConnectionImplMac__
#define __LiveLoggingConnectionImplMac__

#import "livelogging.h"

class LiveLoggingConnectionImplMac : public acpul::ILiveLoggingConnection
{
public:
    LiveLoggingConnectionImplMac()
    : ILiveLoggingConnection()
    {
    }
    
    void connect(const wchar_t *url);
    void send(const wchar_t *path, const wchar_t *text, const wchar_t *type);

    void *_id;
};

#endif /* defined(__LiveLoggingConnectionImplMac__) */
