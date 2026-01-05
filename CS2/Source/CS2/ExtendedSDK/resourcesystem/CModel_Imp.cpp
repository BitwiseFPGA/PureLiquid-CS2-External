#include <GlobalData/Include.h>
#include <CS2/ExtendedSDK/resourcesystem/CModel_Imp.h>
#include <CS2/Offsets/modellib/PermModelData_t.hpp>
#include <CS2/SDK/modellib/PermModelData_t.hpp>
using namespace Globals;
namespace CS2 {
	namespace resourcesystem {

		::CS2::modellib::CRenderMesh* CModel_Imp::GetRenderMesh(int idx)
		{
			uintptr_t pRenderMeshBase = proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + SchemaOffsets::modellib::PermModelData_t::m_refMeshes);
			if (!pRenderMeshBase)
				return nullptr;

			return proc.ReadDirect<uintptr_t, ::CS2::modellib::CRenderMesh*>(pRenderMeshBase + (0x8 * idx));
		}
		::CS2::modellib::PermModelInfo_t* CModel_Imp::GetPermModelInfo()
		{
			uintptr_t pBase = proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + SchemaOffsets::modellib::PermModelData_t::m_modelInfo);

			return  proc.ReadDirect<uintptr_t, ::CS2::modellib::PermModelInfo_t*>(pBase);
		}
		std::string CModel_Imp::GetModelName()
		{
			auto pBase = proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x8);
			return reinterpret_cast<CUtlSymbolLarge*>(pBase)->Get();
		}
		std::string CModel_Imp::GetBoneName(int boneIdx)
		{
			auto boneCount = m_iBoneCount;
			if (boneIdx < 0 || boneIdx >= boneCount)
				return "root";

			auto names = proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x168);

			const uint64_t bone_name_address = proc.ReadDirect<uint64_t>((uint64_t)names + boneIdx * 8);
			return proc.ReadString(bone_name_address);
		}
	}
}