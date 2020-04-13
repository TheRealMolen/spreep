#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

// PIXIE
#include "font.h"
#include "pixie.h"
#include "imgui.h"

// SPREEP
#include "DebugLog.h"
#include "FrameRateManager.h"
#include "types.h"

using namespace std;


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
        DebugLog::Get() << "Failed to open window" << endl;
        return 1;
    }

    Pixie::Font font;
    if (!font.Load(fontFilename, fontWidth, fontHeight))
    {
        DebugLog::Get() << "Failed to load font from " << fontFilename << endl;
        return 0;
    }

    DebugLog::Get() << "Window open, ready to go!" << endl;

    FrameRateManager framerateManager(60);

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
        framerateManager.Render(window);
        Pixie::ImGui::End();

        if (!window.Update())
        {
            DebugLog::Get() << "Failed to update pixels" << endl;
            break;
        }

        framerateManager.Sync(window);
    }
    
    DebugLog::Get() << "all done" << endl;

    window.Close();
    return 0;
}

