#pragma once

namespace SimpleEngine {
    struct VertexData
    {
        VertexData(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector2 uv, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector3 tangent);
        VertexData(DirectX::SimpleMath::Vector3 pos);

        DirectX::SimpleMath::Vector3 mPos;
        DirectX::SimpleMath::Vector2 mUv;
        DirectX::SimpleMath::Vector3 mNormal;
        DirectX::SimpleMath::Vector3 mTangent;
    };

    struct ObjectConstBufferData
    {
        DirectX::SimpleMath::Matrix mWorld;
        DirectX::SimpleMath::Matrix mTranspInvWorld;
    };

    struct FrameConstBufferData
    {
        DirectX::SimpleMath::Matrix mViewProjection;
        DirectX::SimpleMath::Vector4 mCameraPos;
    };

    struct LightConstBufferData
    {
        DirectX::SimpleMath::Vector4 mDirection;
        DirectX::SimpleMath::Vector4 mIntensity;
    };
}