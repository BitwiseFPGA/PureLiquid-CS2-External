#include <SDK/client/C_CSPlayerPawn.hpp>
namespace CS2 {
	namespace resourcesystem {
		class CModel_Imp;
	}
	namespace client {
		class CGameSceneNodeExtended;
		class C_CSPlayerPawnExtended : public C_CSPlayerPawn {
		public:
			std::vector<Matrix2x4_t> GetBoneMatrices(int boneCount);
			::CS2::resourcesystem::CModel_Imp* GetCModel_Imp();
			client::CGameSceneNodeExtended* GetGameSceneNodeExtended();
		};
	}
}