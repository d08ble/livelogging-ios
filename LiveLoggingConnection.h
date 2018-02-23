//
//  LiveLoggingConnection.h
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

#ifndef __LiveLoggingConnection__
#define __LiveLoggingConnection__

#include <iostream>

namespace acpul
{
    // platform connection proxy [
    
    class ILiveLoggingConnectionDelegate
    {
    public:
        virtual void onConnectionRecv(const wchar_t *text) = 0;
        virtual void onConnectionFail(const char *reason, void *data) = 0;
    };
    
    class ILiveLoggingConnection
    {
        ILiveLoggingConnectionDelegate *_delegate;
    public:
        virtual ~ILiveLoggingConnection() {};
        void setDelegate(ILiveLoggingConnectionDelegate *delegate)  { _delegate = delegate; }
        
        virtual void connect(const wchar_t *url) = 0;
        virtual void send(const wchar_t *path, const wchar_t *text, const wchar_t *type) = 0;
    };
    
    // platform connection proxy ]
    // factory [
    
    ILiveLoggingConnection *LiveLoggingConnectionCreate();
    void LiveLoggingConnectionDelete(ILiveLoggingConnection *c);

    // factory ]
}

#endif /* defined(__LiveLoggingConnection__) */
