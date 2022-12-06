#include "Camera.h"

#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy

#include <string>

Camera::Camera()
{
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.5f));
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{

}