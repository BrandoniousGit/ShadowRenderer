
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "GameObject.h"

GameObject::GameObject()
{
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
}

GameObject::~GameObject()
{
	// Do any clean up here
	delete _mesh;
}

void GameObject::Update( float deltaTs )
{
	// Put any update code here
	// Change the _position and _rotation to move the model
}

// Use this function for drawing the scene from camera's POV
void GameObject::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 lightMatrix)
{
	if( _mesh != NULL )
	{
		if( _material != NULL )
		{
			
			// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
			_modelMatrix = glm::translate(glm::mat4(1.0f), _position );
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1, 0, 0));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0, 0, 1));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0, 1, 0));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.x, glm::vec3(1, 0, 0));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.z, glm::vec3(0, 0, 1));
			_modelMatrix = glm::scale(_modelMatrix, _scale);

			// Give all the matrices to the material
			// This makes sure they are sent to the shader
			_material->SetMatrices(_modelMatrix, _invModelMatrix, viewMatrix, projMatrix, lightMatrix);
			// This activates the shader
			_material->Apply();
		}

		// Sends the mesh data down the pipeline
		_mesh->Draw();
	}
}


// Use this function for drawing the scene from light's POV
void GameObject::LightDraw(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	if (_mesh != NULL)
	{
		if (_lightMaterial != NULL)
		{

			// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
			_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1, 0, 0));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0, 0, 1));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0, 1, 0));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.x, glm::vec3(1, 0, 0));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.z, glm::vec3(0, 0, 1));
			_modelMatrix = glm::scale(_modelMatrix, _scale);

			// Give all the matrices to the material
			// This makes sure they are sent to the shader
			_lightMaterial->SetMatrices(_modelMatrix, _invModelMatrix, viewMatrix, projMatrix);
			// This activates the shader
			_lightMaterial->Apply();
		}

		// Sends the mesh data down the pipeline
		_mesh->Draw();
	}
}

