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
	swap(other);
	return *this;
}

SimpleEngine::Transform::Transform(Transform&& other) noexcept
{
	move(std::move(other));
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::getPosition() const
{
	return mPosition;
}

void SimpleEngine::Transform::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	mIsWorldDirty = true;
	mPosition = position;
}

DirectX::SimpleMath::Quaternion SimpleEngine::Transform::getRotation() const
{
	return mRotation;
}

void SimpleEngine::Transform::setRotation(const DirectX::SimpleMath::Quaternion& rotation)
{
	mIsWorldDirty = true;
	mRotation = rotation;
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::getScale() const
{
	return mScale;
}

void SimpleEngine::Transform::setScale(const DirectX::SimpleMath::Vector3& scale)
{
	mIsWorldDirty = true;
	mScale = scale;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::getWorld() const
{
	checkDirtyWorld();
	return mWorld;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::getInvWorld() const
{
	checkDirtyWorld();
	return mInvWorld;
}


inline void SimpleEngine::Transform::swap(Transform& other)
{
	std::swap(mPosition, other.mPosition);
	std::swap(mScale, other.mScale);
	std::swap(mRotation, other.mRotation);

	std::swap(mIsWorldDirty, other.mIsWorldDirty);
	std::swap(mWorld, other.mWorld);
	std::swap(mInvWorld, other.mInvWorld);
}

inline void SimpleEngine::Transform::move(Transform&& other)
{
	mPosition = std::move(other.mPosition);
	mScale = std::move(other.mScale);
	mRotation = std::move(other.mRotation);

	mIsWorldDirty = std::move(other.mIsWorldDirty);
	mWorld = std::move(other.mWorld);
	mInvWorld = std::move(other.mInvWorld);
}

inline void SimpleEngine::Transform::checkDirtyWorld() const
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
		return Transform(rhs.getPosition() + lhs.getPosition(), rhs.getRotation()*lhs.getRotation(), rhs.getScale() * lhs.getScale());
	}

	Transform operator/(const Transform& rhs, const Transform& lhs)
	{
		return Transform(rhs.getPosition() - lhs.getPosition(), rhs.getRotation() / lhs.getRotation(), rhs.getScale() / lhs.getScale());
	}

	std::ostream& operator<<(std::ostream& os, const Transform& rhs)
	{
		auto rot = rhs.getRotation().ToEuler();
		auto pos = rhs.getPosition();
		auto scale = rhs.getScale();
		std::cout << "Transform {\n" <<
			"\tPosition " << pos.x << ", " << pos.y << ", " << pos.z << "\n" <<
			"\tRotation " << rot.x << ", " << rot.y << ", " << rot.z << "\n" <<
			"\tScale " << scale.x << ", " << scale.y << ", " << scale.z << "\n" <<
			"}";
		return os;
	}
}
