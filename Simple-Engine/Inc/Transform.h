#pragma once

namespace SimpleEngine
{
	struct Transform
	{
	public:

		Transform() = default;
		explicit Transform(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 scale);
		explicit Transform(DirectX::SimpleMath::Matrix world);

		Transform(const Transform&);
		Transform& operator=(Transform);

		Transform(Transform&&) noexcept;
		Transform& operator=(Transform&&) noexcept;

		DirectX::SimpleMath::Vector3 getPosition() const;
		void setPosition(const DirectX::SimpleMath::Vector3& position);

		DirectX::SimpleMath::Quaternion getRotation() const;
		void setRotation(const DirectX::SimpleMath::Quaternion& rotation);

		DirectX::SimpleMath::Vector3 getScale() const;
		void setScale(const DirectX::SimpleMath::Vector3& scale);

		DirectX::SimpleMath::Matrix getWorld() const;
		bool setWorld(const DirectX::SimpleMath::Matrix& world);
		DirectX::SimpleMath::Matrix getInvWorld() const;

		
	private:
		inline void swap(Transform& other);
		inline void move(Transform&& other);
		inline void checkDirtyWorld() const;
	private:

		DirectX::SimpleMath::Vector3 mPosition;
		DirectX::SimpleMath::Quaternion mRotation;
		DirectX::SimpleMath::Vector3 mScale{ 1.0f, 1.0f, 1.0f };

		mutable bool mIsWorldDirty = false;
		mutable DirectX::SimpleMath::Matrix mWorld;
		mutable DirectX::SimpleMath::Matrix mInvWorld;
	};


	inline Transform operator*(const Transform& rhs, const Transform& lhs)
	{
		return  SimpleEngine::Transform(rhs.getWorld() * lhs.getWorld());
	}
}

