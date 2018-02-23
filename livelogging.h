//
//  livelogging.h
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

#ifndef __livelogging__
#define __livelogging__

#include "LiveLoggingConnection.h"

namespace acpul
{
    class LiveLogging : public ILiveLoggingConnectionDelegate
    {
        ILiveLoggingConnection *_connection;
        
        std::wstring _prefix;
        
    public:
        LiveLogging(ILiveLoggingConnection *connection);
        ~LiveLogging();

        // server(options)
        // client(url) - r/w access
        // connect(url) - write only
        // open(file)

        void onConnectionRecv(const wchar_t *text);
        void onConnectionFail(const char *reason, void *data);
        
        void connect(const wchar_t *url);
        
        void setPathPrefix(const wchar_t *prefix);
        
        void log(const wchar_t *path, const wchar_t *message);
        void replace(const wchar_t *path, const wchar_t *message);
        void erase(const wchar_t *path);
    };
    
    
    // factory [
    
    LiveLogging* createLiveLogging();
    
    // factory ]
}

#endif /* defined(__livelogging__) */
