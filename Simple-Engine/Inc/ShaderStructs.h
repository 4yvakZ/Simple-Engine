#pragma once

namespace SimpleEngine {
    struct Vertex
    {
        explicit Vertex(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector2 uv, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector3 tangent);
        explicit Vertex(DirectX::SimpleMath::Vector3 pos);
        Vertex();

        DirectX::SimpleMath::Vector3 mPosition;
        DirectX::SimpleMath::Vector2 mUV;
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
        DirectX::SimpleMath::Matrix mView;
        DirectX::SimpleMath::Matrix mViewProjection;
        DirectX::SimpleMath::Vector4 mCameraPos;
    };

    struct LightConstBufferData
    {
        DirectX::SimpleMath::Vector4 mPosition;
        DirectX::SimpleMath::Vector4 mDirection;
        DirectX::SimpleMath::Vector4 mIntensity;
    };

    struct CascadeData {
        DirectX::SimpleMath::Matrix mViewProjection[4];
        float mDistances[4];
    };
}