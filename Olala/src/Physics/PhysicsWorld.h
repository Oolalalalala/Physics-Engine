#pragma once

#include "Scene/Scene.h"

namespace Olala {

	class PhysicsWorld
	{
	public:
		PhysicsWorld(Scene& scene);
		~PhysicsWorld();

		void OnUpdate(float dt);

	private:
		Scene& m_Scene;
	};

}