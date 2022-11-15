#include "GameObject.h"

#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy

#include <string>

class Camera
{
public:

	Camera();
	~Camera();

	// Use these to adjust the camera's orientation
	// Camera is currently set up to rotate about the world-space origin NOT the camera's origin
	void ChangeCameraAngleX(float value) { _cameraAngleX += value; }
	void ChangeCameraAngleY(float value) { _cameraAngleY += value; }

protected:

	// Currently one object, this could be a list of objects!
	GameObject* _model;

	// This matrix represents the camera's position and orientation
	glm::mat4 _viewMatrix;

	// This matrix is like the camera's lens
	glm::mat4 _projMatrix;

	// Current rotation information about the camera
	float _cameraAngleX, _cameraAngleY;

	// Position of the single point-light in the scene
	glm::vec3 _lightPosition;
};