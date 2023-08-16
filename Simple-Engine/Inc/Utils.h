#pragma once

namespace DirectX
{
	namespace SimpleMath 
	{
		inline Vector4 ToVector4(const Vector3& vector3, const float w) 
		{
			return Vector4(vector3.x, vector3.y, vector3.z, w);
		}
	}
}