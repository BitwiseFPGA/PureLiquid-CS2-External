#pragma once
#include <cstdint>
#include <Math/Matrix.h>
#include <DirectXMath.h>
using namespace DirectX;
namespace CS2 {
	class ViewMatrixResolver {
	private:
		inline static uintptr_t pViewMatrixAddr = 0x0;
	public:
		static uintptr_t ResolveViewMatrixAddr();
		static ViewMatrix_t GetViewMatrix();
		inline static XMMATRIX GetView3DMatrix()
		{
			auto vm = GetViewMatrix();
			return XMMATRIX(
				vm.arrData[0][0], vm.arrData[1][0], vm.arrData[2][0], vm.arrData[3][0],
				vm.arrData[0][1], vm.arrData[1][1], vm.arrData[2][1], vm.arrData[3][1],
				vm.arrData[0][2], vm.arrData[1][2], vm.arrData[2][2], vm.arrData[3][2],
				vm.arrData[0][3], vm.arrData[1][3], vm.arrData[2][3], vm.arrData[3][3]
			);
		}
	};
}