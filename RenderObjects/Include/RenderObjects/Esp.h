#pragma once
#include <Overlay/RenderObject.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
class View3D;
namespace RenderObjects {
    class CBoneNode;
    class Esp : public RenderObject {
    public:
        Esp() {
            printf("Esp constructed...\n");
        };
        void Render(ImVec2 DisplaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) override;
    private:
        void DrawHealthBarWorld(View3D* view3D, Vector3 topPos, float MaxHealth, float CurrentHealth, Vector3 size);
        void DrawHealthBarScreenAligned(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size);
        void DrawHealthBarWorldLeft(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size);

    };
}