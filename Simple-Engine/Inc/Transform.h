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

		DirectX::SimpleMath::Vector3 GetPosition() const;
		void SetPosition(const DirectX::SimpleMath::Vector3& position);

		DirectX::SimpleMath::Quaternion GetRotation() const;
		void SetRotation(const DirectX::SimpleMath::Quaternion& rotation);

		DirectX::SimpleMath::Vector3 GetScale() const;
		void SetScale(const DirectX::SimpleMath::Vector3& scale);

		DirectX::SimpleMath::Matrix GetWorld() const;
		DirectX::SimpleMath::Matrix GetInvWorld() const;

		
	private:
		inline void Swap(Transform& other);
		inline void Move(Transform&& other);
		inline void CheckDirtyWorld() const;
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

