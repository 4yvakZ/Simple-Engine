#include "pch.h"
#include "ShaderStructs.h"

using namespace DirectX::SimpleMath;

SimpleEngine::VertexData::VertexData(Vector3 pos, Vector2 uv,Vector3 normal) :
    mPos(pos), mUv(uv), mNormal(normal)
{
}

SimpleEngine::VertexData::VertexData(Vector3 pos) :
    mPos(pos), mUv(0, 0), mNormal(0, 0, 1)
{

}