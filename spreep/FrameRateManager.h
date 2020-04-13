#pragma once

#include "types.h"


namespace Pixie {
class Window;
}


class FrameRateManager
{
public:
    FrameRateManager(u32 desiredFps)
        : m_desiredFrameLength(1.0 / desiredFps)
        , m_lastWait(0)
        , m_frameCount(0)
        , m_fpsStartTime(-1)
        , m_fps(0)
    { /**/
    }

    // delay this frame until we reach our target fps
    void Sync(const Pixie::Window& window);

    void Render(const Pixie::Window& window) const;

private:
    double m_desiredFrameLength;
    u64 m_lastWait;

    u32 m_frameCount;
    double m_fpsStartTime;
    u32 m_fps;
};

