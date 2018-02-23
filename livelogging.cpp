//
//  livelogging.cpp
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

#include <assert.h>
#include "livelogging.h"

using namespace acpul;

// factory [

#include "LiveLoggingConnectionImplMac.h"

LiveLogging* acpul::createLiveLogging()
{
    return new LiveLogging(LiveLoggingConnectionCreate());
}

// factory ]



LiveLogging::LiveLogging(ILiveLoggingConnection *connection)
: _connection(connection)
{
    assert(connection);
    
    connection->setDelegate(this);
}

LiveLogging::~LiveLogging()
{
    if (_connection) {
        LiveLoggingConnectionDelete(_connection);
        _connection = NULL;
    }
}

// networking [

void LiveLogging::connect(const wchar_t *url)
{
    printf("connect %S\n", url);
//    std::cout << "connect " << url << "\n";
    _connection->connect(url);
}

void LiveLogging::onConnectionRecv(const wchar_t *text)
{
    std::cout << "recv " << text << "\n";
}

void LiveLogging::onConnectionFail(const char *reason, void *data)
{
    std::cout << "fail " << reason << " : " << data << "\n";
}

// networking ]
// api [
void LiveLogging::setPathPrefix(const wchar_t *prefix)
{
    _prefix = std::wstring(prefix);
}

void LiveLogging::log(const wchar_t *path, const wchar_t *message)
{
    if (!path || !message)
        return;

    printf("LOG %S - %S\n", path, message);
    _connection->send((_prefix + std::wstring(path)).c_str(), message, L"log");
}

void LiveLogging::replace(const wchar_t *path, const wchar_t *message)
{
    if (!path || !message)
        return;
    
    printf("REPLACE %S - %S\n", path, message);
    _connection->send((_prefix + std::wstring(path)).c_str(), message, L"replace");
}

void LiveLogging::erase(const wchar_t *path)
{
    if (!path)
        return;
    
    printf("ERASE %S\n", path);
    _connection->send((_prefix + std::wstring(path)).c_str(), L"", L"erase");
}

// api ]
