#pragma once

namespace SimpleEngine
{
	struct Transform
	{
	public:

		Transform() = default;
		explicit Transform(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 scale);

		Transform(const Transform&);
		Transform& operator=(Transform);

		Transform(Transform&&) noexcept;

		DirectX::SimpleMath::Vector3 getPosition() const;
		void setPosition(const DirectX::SimpleMath::Vector3& position);

		DirectX::SimpleMath::Quaternion getRotation() const;
		void setRotation(const DirectX::SimpleMath::Quaternion& rotation);

		DirectX::SimpleMath::Vector3 getScale() const;
		void setScale(const DirectX::SimpleMath::Vector3& scale);

		DirectX::SimpleMath::Matrix getWorld() const;
		DirectX::SimpleMath::Matrix getInvWorld() const;

		
	private:
		inline void swap(Transform& other);
		inline void move(Transform&& other);
		inline void checkDirtyWorld() const;
	private:

		DirectX::SimpleMath::Vector3 mPosition;
		DirectX::SimpleMath::Quaternion mRotation;
		DirectX::SimpleMath::Vector3 mScale{ 1.0f, 1.0f, 1.0f };

		mutable bool mIsWorldDirty = true;
		mutable DirectX::SimpleMath::Matrix mWorld;
		mutable DirectX::SimpleMath::Matrix mInvWorld;
	};

	Transform operator*(const Transform& rhs, const Transform& lhs);
	Transform operator/(const Transform& rhs, const Transform& lhs);
	std::ostream& operator<<(std::ostream& os, const Transform& rhs);
}

