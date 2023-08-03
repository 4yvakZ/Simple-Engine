#include "pch.h"
#include "ShaderStructs.h"

using namespace DirectX::SimpleMath;

SimpleEngine::VertexData::VertexData(Vector3 pos, Vector2 uv,Vector3 normal) :
    mPos(pos), mColor(Color(1, 1, 1, 1)), mUv(uv), mNormal(normal)
{
}

SimpleEngine::VertexData::VertexData(Vector3 pos, Color color = Color(1, 1, 1, 1)) :
    mPos(pos), mColor(color), mUv(0, 0), mNormal(0, 0, -1)
{

}