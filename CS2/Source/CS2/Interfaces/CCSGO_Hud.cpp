#include <CS2/Interfaces/CCSGO_Hud.h>
#include <GlobalData/Include.h>
using namespace Globals;
namespace CS2 {
    uintptr_t CCSGO_Hud::FindHudElement(const std::string& elementName, DWORD sub)
    {
        if (!this || elementName.empty()) {
            return NULL;
        }

        uintptr_t treeData = reinterpret_cast<uintptr_t>(this) + 0x248;
        uint32_t flags = proc.ReadDirect<uint32_t>(reinterpret_cast<uintptr_t>(this) + 0x24C);
        uintptr_t arrayBase = proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x250);

        int32_t currentIndex = proc.ReadDirect<int32_t>(treeData + 0x10);
        int32_t foundIndex = -1;

        while (currentIndex != -1) {
            uintptr_t nodeAddr = ((flags & 0x7FFFFFFF) != 0)
                ? arrayBase + (32LL * currentIndex)
                : 32LL * currentIndex;

            int32_t leftIndex = proc.ReadDirect<int32_t>(nodeAddr + 0x00);
            int32_t rightIndex = proc.ReadDirect<int32_t>(nodeAddr + 0x04);
            uintptr_t pName = proc.ReadDirect<uintptr_t>(nodeAddr + 0x10);

            std::string nodeName = proc.ReadString(pName, 128);
            int cmp = _stricmp(elementName.c_str(), nodeName.c_str());

            if (cmp == 0) {
                foundIndex = currentIndex;
                break;
            }
            else if (cmp < 0)
                currentIndex = leftIndex;
            else
                currentIndex = rightIndex;
        }

        if (foundIndex == -1) {
            return NULL;
        }

        uintptr_t nodeAddr = ((flags & 0x7FFFFFFF) != 0)
            ? arrayBase + (32LL * foundIndex)
            : 32LL * foundIndex;

        auto res = proc.ReadDirect<uintptr_t>(nodeAddr + 0x18);
        return sub ? res - sub : res;
    }
}