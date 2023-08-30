#include "pch.h"
#include "ShaderStructs.h"

using namespace DirectX::SimpleMath;

SimpleEngine::Vertex::Vertex(Vector3 pos, Vector2 uv,Vector3 normal, Vector3 tangent) :
    mPosition(pos), mUV(uv), mNormal(normal), mTangent(tangent)
{
}

SimpleEngine::Vertex::Vertex(Vector3 pos) :
    mPosition(pos), mUV(0, 0), mNormal(0, 0, 1), mTangent (1, 0, 0)
{

}

SimpleEngine::Vertex::Vertex():
    Vertex(Vector3::Zero)
{
}
