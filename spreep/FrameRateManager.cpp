#include "FrameRateManager.h"

#include <chrono>
#include <sstream>
#include <thread>

#include "imgui.h"
#include "pixie.h"

#include "DebugLog.h"


using namespace std;




void FrameRateManager::Sync(const Pixie::Window& window)
{
    const double fpsRecalcRate = 1.0;

    // update time
    if (m_fpsStartTime < 0)
        m_fpsStartTime = window.GetTime();

    ++m_frameCount;

    double timeSinceLastStart = window.GetTime() - m_fpsStartTime;
    if (timeSinceLastStart > fpsRecalcRate)
    {
        m_fps = (u32)((m_frameCount / timeSinceLastStart) + 0.5);
        DebugLog::Get() << "  -- update fps (" << m_fps << "): rendered " << m_frameCount << " frames over " << timeSinceLastStart << "s\n";

        m_fpsStartTime = window.GetTime();
        m_frameCount = 0;
    }

    // sleep until 60Hz
    double lastFrameTime = window.GetDelta();
    double lastNonWaitTime = lastFrameTime - (m_lastWait / 1000000.0);
    if (lastNonWaitTime < m_desiredFrameLength)
    {
        u64 usec = u64((m_desiredFrameLength - lastNonWaitTime) * 1000000.0);

        //DebugLog::Get() << "frame @ " << u64(window.GetTime()*1000) << " took " << u64(lastFrameTime*1000) << "ms; sleeping for " << usec << "us\n";

        const uint64_t sleepbias = 500;
        if (usec > sleepbias)
        {
            m_lastWait = usec - sleepbias;
            this_thread::sleep_for(chrono::microseconds(m_lastWait));
        }
    }
    else
    {
        //DebugLog::Get() << "frame @ " << u64(window.GetTime() * 1000) << " took " << u64(lastFrameTime * 1000) << "ms; not sleeping\n";
    }
}

void FrameRateManager::Render(const Pixie::Window& window) const
{
    ostringstream os;
    os << m_fps << "fps";
    Pixie::ImGui::Label(os.str().c_str(), window.GetWidth() - 100, 10, 0x40b0b0b0);
}


