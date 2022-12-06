#pragma once

#include "GameObject.h"

#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy

#include <string>
#include <iostream>
#include <memory>

class Camera
{
public:

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix() { return _viewMatrix; }
	glm::mat4 GetProjMatrix() { return _projMatrix; }

protected:
	// This matrix represents the camera's position and orientation
	glm::mat4 _viewMatrix;

	// This matrix is like the camera's lens
	glm::mat4 _projMatrix;
};