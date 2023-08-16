#include "pch.h"
#include "Transform.h"
#include "Utils.h"

using namespace DirectX::SimpleMath;

SimpleEngine::Transform::Transform(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 scale) :
	mPosition(position),
	mRotation(rotation),
	mScale(scale),
	mWorld(),
	mInvWorld()
{
}

SimpleEngine::Transform::Transform(const Transform& other)
{
	mPosition = other.mPosition;
	mScale = other.mScale;
	mRotation = other.mRotation; 

	mIsWorldDirty = other.mIsWorldDirty;
	mWorld = other.mWorld;
	mInvWorld = other.mInvWorld;
}

SimpleEngine::Transform& SimpleEngine::Transform::operator=(Transform other)
{
	Swap(other);
	return *this;
}

SimpleEngine::Transform::Transform(Transform&& other) noexcept
{
	Move(std::move(other));
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::GetPosition() const
{
	return mPosition;
}

void SimpleEngine::Transform::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
	mIsWorldDirty = true;
	mPosition = position;
}

DirectX::SimpleMath::Quaternion SimpleEngine::Transform::GetRotation() const
{
	return mRotation;
}

void SimpleEngine::Transform::SetRotation(const DirectX::SimpleMath::Quaternion& rotation)
{
	mIsWorldDirty = true;
	mRotation = rotation;
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::GetScale() const
{
	return mScale;
}

void SimpleEngine::Transform::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	mIsWorldDirty = true;
	mScale = scale;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::GetWorld() const
{
	CheckDirtyWorld();
	return mWorld;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::GetInvWorld() const
{
	CheckDirtyWorld();
	return mInvWorld;
}


inline void SimpleEngine::Transform::Swap(Transform& other)
{
	std::swap(mPosition, other.mPosition);
	std::swap(mScale, other.mScale);
	std::swap(mRotation, other.mRotation);

	std::swap(mIsWorldDirty, other.mIsWorldDirty);
	std::swap(mWorld, other.mWorld);
	std::swap(mInvWorld, other.mInvWorld);
}

inline void SimpleEngine::Transform::Move(Transform&& other)
{
	mPosition = std::move(other.mPosition);
	mScale = std::move(other.mScale);
	mRotation = std::move(other.mRotation);

	mIsWorldDirty = std::move(other.mIsWorldDirty);
	mWorld = std::move(other.mWorld);
	mInvWorld = std::move(other.mInvWorld);
}

inline void SimpleEngine::Transform::CheckDirtyWorld() const
{
	if (mIsWorldDirty)
	{
		mIsWorldDirty = false;
		//TODO make more effective World creation
		mWorld = Matrix::CreateScale(mScale) * Matrix::CreateFromQuaternion(mRotation) * Matrix::CreateTranslation(mPosition);
		mInvWorld = mWorld.Invert();
	}
}

namespace SimpleEngine {

	Transform operator*(const Transform& rhs, const Transform& lhs)
	{
		return Transform(rhs.GetPosition() + lhs.GetPosition(), rhs.GetRotation()*lhs.GetRotation(), rhs.GetScale() * lhs.GetScale());
	}

	Transform operator/(const Transform& rhs, const Transform& lhs)
	{
		return Transform(rhs.GetPosition() - lhs.GetPosition(), rhs.GetRotation() / lhs.GetRotation(), rhs.GetScale() / lhs.GetScale());
	}

	std::ostream& operator<<(std::ostream& os, const Transform& rhs)
	{
		auto rot = rhs.GetRotation().ToEuler();
		auto pos = rhs.GetPosition();
		auto scale = rhs.GetScale();
		std::cout << "Transform {\n" <<
			"\tPosition " << pos.x << ", " << pos.y << ", " << pos.z << "\n" <<
			"\tRotation " << rot.x << ", " << rot.y << ", " << rot.z << "\n" <<
			"\tScale " << scale.x << ", " << scale.y << ", " << scale.z << "\n" <<
			"}";
		return os;
	}
}
