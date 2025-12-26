#include <GlobalData/Include.h>
#include <Math/Matrix.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h> 
#include <CS2/SDK/client/C_CSPlayerPawn.hpp>
#include <CS2/Offsets/client/CSkeletonInstance.hpp>
using namespace Globals;
namespace CS2 {
	namespace client {
		std::vector<Matrix2x4_t> C_CSPlayerPawnExtended::GetBoneMatrices(int boneCount)
		{
			auto pGameSceneNodePtr = reinterpret_cast<uintptr_t>(m_pGameSceneNode);
			if (!pGameSceneNodePtr)
				return {};
			auto pAddr = pGameSceneNodePtr + SchemaOffsets::client::CSkeletonInstance::m_modelState + 0x80;
			return proc.ReadArray< Matrix2x4_t>(proc.ReadDirect<uintptr_t>(pAddr), boneCount);
		}

	}
}