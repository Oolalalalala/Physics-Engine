#pragma once

#include "Collider.h"

#include "glm/glm.hpp"

namespace Olala {

	// Position is set to the center of mass
	// TODO : consider removing set/get functions
	class PhysicsObject
	{
	public:
		PhysicsObject() = default;

		glm::vec2& GetPosition() { return m_Position; }
		glm::vec2& GetVelocity() { return m_Velocity; }
		float& GetMass() { return m_Mass; }
		Collider::ColliderType& GetColliderType() { return m_Collider.Type; }
		glm::vec2& GetColliderSize() { return m_Collider.Size; }
		glm::vec2& GetColliderOffset() { return m_Collider.Offset; }
		float& GetColliderRadius() { return m_Collider.Size.x; }

		//void SetPosition(const glm::vec2& position) { m_Position = position; }
		//void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
		//void SetMass(const float& mass) { m_Mass = mass; }
		//void SetColliderType(const Collider::ColliderType& type) { m_Collider.Type = type; }
		//void SetColliderSize(const glm::vec2& size) { m_Collider.Size = size; }
		//void SetColliderOffset(const glm::vec2 offset) { m_Collider.Offset = offset; }
		//void SetColliderRadius(const float& radius) { m_Collider.Size.x = radius; }

	private:
		glm::vec2 m_Position = glm::vec2(0.f);
		glm::vec2 m_Velocity = glm::vec2(0.f);
		float m_Mass = 1.f;
		Collider m_Collider;

	public:
		bool IsStatic = false;
		bool ApplyGravity = true;
		bool ApplyCollision = true;
	};

}