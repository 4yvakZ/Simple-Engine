#pragma once

namespace SimpleEngine {
    struct VertexData
    {
        VertexData(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector2 uv, DirectX::SimpleMath::Vector3 normal);
        VertexData(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Color color);

        DirectX::SimpleMath::Vector3 mPos;
        DirectX::SimpleMath::Color mColor;
        DirectX::SimpleMath::Vector2 mUv;
        DirectX::SimpleMath::Vector3 mNormal;
    };

    /*struct MaterialBuf
    {
        DirectX::SimpleMath::Vector4 mAmbient;
        DirectX::SimpleMath::Vector4 diffuse;
        DirectX::SimpleMath::Vector4 specularAlpha;
    };*/


}