#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

SimpleEngine::Transform::Transform(const Transform& other)
{
	std::unique_lock<std::mutex> myLock(mMutex), otherLock(other.mMutex);
	lock(myLock, otherLock);
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

SimpleEngine::Transform::Transform(Transform&& other)
{
	mPosition = std::move(other.mPosition);
	mScale = std::move(other.mScale);
	mRotation = std::move(other.mRotation);
	mIsWorldDirty = std::move(other.mIsWorldDirty);
	mWorld = std::move(other.mWorld);
	mInvWorld = std::move(other.mInvWorld);
}

SimpleEngine::Transform& SimpleEngine::Transform::operator=(Transform&& other)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mPosition = std::move(other.mPosition);
	mScale = std::move(other.mScale);
	mRotation = std::move(other.mRotation);
	mIsWorldDirty = std::move(other.mIsWorldDirty);
	mWorld = std::move(other.mWorld);
	mInvWorld = std::move(other.mInvWorld);
	return *this;
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::getPosition() const
{
	return mPosition;
}

void SimpleEngine::Transform::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mIsWorldDirty = true;
	mPosition = position;
}

DirectX::SimpleMath::Quaternion SimpleEngine::Transform::getRotation() const
{
	return mRotation;
}

void SimpleEngine::Transform::setRotation(const DirectX::SimpleMath::Quaternion& rotation)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mIsWorldDirty = true;
	mRotation = rotation;
}

DirectX::SimpleMath::Vector3 SimpleEngine::Transform::getScale() const
{
	return mScale;
}

void SimpleEngine::Transform::setScale(const DirectX::SimpleMath::Vector3& scale)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mIsWorldDirty = true;
	mScale = scale;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::getWorld() const
{
	checkDirtyWorld();
	return mWorld;
}

bool SimpleEngine::Transform::setWorld(const DirectX::SimpleMath::Matrix& world)
{
	Matrix w = world;
	Vector3 pos, scale;
	Quaternion rot;
	if (w.Decompose(scale, rot, pos)) 
	{
		std::lock_guard<std::mutex> guard(mMutex);
		mIsWorldDirty = false;
		mWorld = world;
		mInvWorld = world.Invert();
		mScale = scale;
		mRotation = rot;
		mPosition = pos;
		return true;
	}
	return false;
}

DirectX::SimpleMath::Matrix SimpleEngine::Transform::getInvWorld() const
{
	checkDirtyWorld();
	return mInvWorld;
}

void SimpleEngine::Transform::swap(Transform& other)
{
	std::lock_guard<std::mutex> guard(mMutex);
	std::swap(mPosition, other.mPosition);
	std::swap(mScale, other.mScale);
	std::swap(mRotation, other.mRotation);

	std::swap(mIsWorldDirty, other.mIsWorldDirty);
	std::swap(mWorld, other.mWorld);
	std::swap(mInvWorld, other.mInvWorld);
}

inline void SimpleEngine::Transform::checkDirtyWorld() const
{
	std::lock_guard<std::mutex> guard(mMutex);
	if (mIsWorldDirty)
	{
		mIsWorldDirty = false;
		//TODO make more effective World creation
		mWorld = Matrix::CreateScale(mScale) * Matrix::CreateFromQuaternion(mRotation) * Matrix::CreateTranslation(mPosition);
		mInvWorld = mWorld.Invert();
	}
}
