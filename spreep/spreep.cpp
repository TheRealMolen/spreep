#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>
#include <thread>

#include "font.h"
#include "pixie.h"
#include "imgui.h"

using namespace std;

typedef uint32_t u32;
typedef uint64_t u64;

#define nicefont 1

#if nicefont
const char* fontFilename = "pixelmix-8x8.bmp";
const int fontWidth = 8;
const int fontHeight = 8;
#else
const char* fontFilename = "font.bmp";
const int fontWidth = 9;
const int fontHeight = 16;
#endif

const int width = 1280 * 3 / 4;
const int height = 720 * 3 / 4;

int main(int argc, char** argv)
{
    Pixie::Window window;
    if (!window.Open("spreep", width, height, false))
    {
        cerr << "Failed to open window" << endl;
        return 1;
    }

    Pixie::Font font;
    if (!font.Load(fontFilename, fontWidth, fontHeight))
    {
        cerr << "Failed to load font from " << fontFilename << endl;
        return 0;
    }

    u32 frameCount = 0;
    double fpsStartTime = -1;
    u32 fps = 0;

    u32 col = 0;
    while (!window.HasKeyGoneUp(Pixie::Key_Escape))
    {
        col = (col + 1) & 0xff;
        col |= (col << 8) | (col << 16);

        u32* pixels = window.GetPixels();

        // ..draw pixels!
        memset(pixels, col, width * height * sizeof(u32));

        // draw ui
        Pixie::ImGui::Begin(&window, &font);
        {
            ostringstream os;
            os << fps << "fps";
            Pixie::ImGui::Label(os.str().c_str(), width - 100, 10, 0x40b0b0b0);
        }
        Pixie::ImGui::End();

        if (!window.Update())
        {
            cerr << "Failed to update pixels" << endl;
            break;
        }

        // update time
        {
            if (fpsStartTime < 0)
                fpsStartTime = window.GetTime();
            ++frameCount;
            double timeSinceLastStart = window.GetTime() - fpsStartTime;
            if (timeSinceLastStart > 2.0f)
            {
                fps = (u32)((frameCount / timeSinceLastStart) + 0.5);
                fpsStartTime = window.GetTime();
                frameCount = 0;
            }
        }

        // sleep until 60Hz
        {
            const double oneFrameTime = 1.0f / 60.0f;
            double lastFrameTime = window.GetDelta();
            if (lastFrameTime < oneFrameTime)
            {
                uint64_t usec = (uint64_t)((oneFrameTime - lastFrameTime) * 1000000.0);
                const uint64_t sleepbias = 50;
                if (usec > sleepbias)
                {
                    this_thread::sleep_for(chrono::microseconds(usec - sleepbias));
                }
            }
        }
    }

    window.Close();
    return 0;
}

