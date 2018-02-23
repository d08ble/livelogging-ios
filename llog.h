//
//  llog.h
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

#ifndef __llog__
#define __llog__

#include <iostream>
#include <vector>
#include "livelogging.h"

namespace acpul
{
    class LLog
    {
        LiveLogging *_log;
        std::vector<std::wstring> _names;
    public:
        LLog(LiveLogging *log)
        {
            _log = log;
        }
        
        void push(const wchar_t *name);
        void pop();
        void clear();
        
        void print(bool replace, const wchar_t *path, const wchar_t *message);
        void erase(const wchar_t *path);
    };
}

#endif /* defined(__llog__) */
