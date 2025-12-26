#pragma once
#include <SDK/client/C_CSPlayerPawn.hpp>
namespace CS2 {
	namespace client {
		class C_CSPlayerPawnExtended : public C_CSPlayerPawn {
		public:
			std::vector<Matrix2x4_t> GetBoneMatrices(int boneCount);
		};
	}
}