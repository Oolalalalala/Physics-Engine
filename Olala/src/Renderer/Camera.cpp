#include "pch.h"
#include "Camera.h"

namespace Olala {

	OrthographicCamera::OrthographicCamera()
		: Camera(glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f))
	{
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
		: Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
	}
}