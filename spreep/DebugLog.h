#pragma once

#include <fstream>

class DebugLog
{
public:
    static std::ofstream& Get()
    {
        return s_log;
    }

private:
    static std::ofstream s_log;
};
