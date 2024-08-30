#pragma once
#include "CommonObject.h"
//#include <imgui.h>

class DrawThread
{
public:
    void operator()(CommonObjects& common);
};
