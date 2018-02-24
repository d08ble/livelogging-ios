# LiveLogging for iOS

Structured live logging for iOS. Use LiveComment for view logs.

Usage:
1. Add livelogging sources to your project
2. Add SocketRocket framework

```cpp
#include "livelogging.h"

liveLogging = createLiveLogging();
liveLogging->connect(L"ws://192.168.0.101:8995");

liveLogging->setPathPrefix(L"<iSimulator>/");

liveLogging->log(L"App/Status", L"started");
        
```

## License
Code released under the MIT licence.
