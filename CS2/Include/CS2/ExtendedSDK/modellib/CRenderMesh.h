#pragma once
#include <CS2/SDK/modellib/CRenderMesh.hpp>
#include <CS2/SDK/modellib/CHitBox.hpp>
#include <Custom/GlobalTypes.hpp>
#include <vector>

namespace CS2 {
	namespace modellib {
	
		class CRenderMeshExtended : public CRenderMesh {
		private:

			class CRenderMeshHitboxUtlVector {
			private:
				char pad[0x60];
			public:
				NESTED_PROPERTY(m_Hitboxes, GlobalTypes::CUtlVector<CHitBox>, 0x28);

			};

			NESTED_PROPERTY(m_HitboxData, GlobalTypes::CUtlVector<CRenderMeshHitboxUtlVector>, 0x138);
		public:
			std::vector<CHitBox*> GetHitboxes();
		};
	}
}