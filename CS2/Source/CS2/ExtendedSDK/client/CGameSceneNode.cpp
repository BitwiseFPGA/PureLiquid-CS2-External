#include <CS2/ExtendedSDK/client/CGameSceneNode.h>
#include <CS2/SDK/client/CGameSceneNode.hpp>
#include <GlobalData/Include.h>
using namespace Globals;
namespace CS2 {
	namespace client {

		uint32_t client::CGameSceneNodeExtended::GetBonesOffset()
		{
			if (!dwBonesOffset) {
				auto hClient = proc.GetRemoteModule("client.dll");
				if (!hClient || hClient && !hClient->GetAddr()) {
					return NULL;
				}
				auto bonesDisp = hClient->ScanMemory("48 8B 8B ?? ?? ?? ?? 48 85 C9 75 ?? 90 38 8B");
				dwBonesOffset = hClient->ResolveDisp32(bonesDisp, 2);
				if (!dwBonesOffset)
					return NULL;
			}

			return dwBonesOffset;
		}
	}
}