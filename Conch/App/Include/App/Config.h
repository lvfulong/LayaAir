
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <string>

struct Config
{
    std::string title;
    bool resizeable = true;
    bool headless = false;
};

#endif
