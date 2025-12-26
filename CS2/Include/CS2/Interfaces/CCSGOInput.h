#pragma once
#include <GlobalData/Include.h>
#include <Math/QAngle.h>
namespace CS2 {
    class CCSGOInput {
    private:
        inline static uint32_t pViewAnglesOffset = 0x0;
        static uintptr_t ResolveViewAnglesOffset();
    public:
        RUNTIME_OFFSET_PROPERTY(vViewAngles, QAngle_t, ResolveViewAnglesOffset)
      

    };
}