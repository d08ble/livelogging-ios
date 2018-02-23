//
//  llog.cpp
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

#include "llog.h"
#include "cppformat/format.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace acpul;

void LLog::push(const wchar_t *name)
{
    std::wstring s(name);
    _names.push_back(s);
}

void LLog::pop()
{
    if (_names.size() > 0)
        _names.pop_back();
}

void LLog::clear()
{
    _names.clear();
}

void LLog::print(bool replace, const wchar_t *path, const wchar_t *message)
{
    fmt::WMemoryWriter os;
    
    // make path [
    
    if (!path) {
        
        for (int i = 0; i < _names.size(); i++) {
            if (i > 0)
                os << "/";
            os << _names[i];
        }
        path = os.c_str();
    }
    
    // make path ]
    
    if (!replace)
        _log->log(path, message);
    else
        _log->replace(path, message);
}

void LLog::erase(const wchar_t *path)
{
    fmt::WMemoryWriter os;
    
    // make path [
    
    if (!path) {
        
        for (int i = 0; i < _names.size(); i++) {
            if (i > 0)
                os << "/";
            os << _names[i];
        }
        path = os.c_str();
    }
    
    // make path ]
    
    _log->erase(path);
}
