#include <GlobalData/Include.h>
#include <Math/Matrix.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h> 
#include <CS2/SDK/client/C_CSPlayerPawn.hpp>
#include <CS2/Offsets/client/CSkeletonInstance.hpp>
#include <CS2/ExtendedSDK/resourcesystem/CModel_Imp.h>
#include <CS2/Offsets/client/CModelState.hpp>
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
		::CS2::resourcesystem::CModel_Imp* C_CSPlayerPawnExtended::GetCModel_Imp()
		{
			auto pGameSceneNodePtr = reinterpret_cast<uintptr_t>(m_pGameSceneNode);
			if (!pGameSceneNodePtr)
				return nullptr;
			auto ptr = proc.ReadDirect<uintptr_t>(pGameSceneNodePtr + SchemaOffsets::client::CSkeletonInstance::m_modelState + SchemaOffsets::client::CModelState::m_hModel);
			return proc.ReadDirect<resourcesystem::CModel_Imp*>(ptr);
		}
		client::CGameSceneNodeExtended* C_CSPlayerPawnExtended::GetGameSceneNodeExtended()
		{
			return reinterpret_cast<client::CGameSceneNodeExtended*>(m_pGameSceneNode);
		}
	}
}