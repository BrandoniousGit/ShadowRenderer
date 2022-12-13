#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"

// The GameObject contains a mesh, a material and position / orientation information
class GameObject
{
public:

	GameObject();
	~GameObject();

	void SetMesh(Mesh *input) {_mesh = input;}
	void SetMaterial(Material *input) {_material = input;}

	void SetLightMaterial(Material* input) { _lightMaterial = input; }
	
	//Setters and Getters for Position
	void SetPosition(float posX, float posY, float posZ) { _position.x = posX; _position.y = posY; _position.z = posZ; }
	void SetPosition(glm::vec3 value) { _position = value; }
	void AddPosition(float posX, float posY, float posZ) { _position.x += posX; _position.y += posY; _position.z += posZ; }
	void AddPosition(glm::vec3 value) { _position += value; }
	glm::vec3 GetPosition() { return _position; }

	//Setters and Getters for Rotation
	void SetRotation(float rotX, float rotY, float rotZ) { _rotation.x = rotX; _rotation.y = rotY; _rotation.z = rotZ; }
	void SetRotation(glm::vec3 value) { _rotation = value; }
	void AddRotation(float rotX, float rotY, float rotZ) { _rotation.x += rotX; _rotation.y += rotY; _rotation.z += rotZ; }
	void AddRotation(glm::vec3 value) { _rotation += value; }
	glm::vec3 GetRotation() { return _rotation; }

	//Setters and Getters for Size
	void SetScale(float sX, float sY, float sZ) { _scale.x = sX; _scale.y = sY; _scale.z = sZ; }
	void SetScale(glm::vec3 value) { _scale = value; }
	void AddScale(float sX, float sY, float sZ) { _scale.x += sX; _scale.y += sY; _scale.z += sZ; }
    void AddScale(glm::vec3 value) { _scale += value; }
	glm::vec3 GetScale() { return _scale; }

	void Update( float deltaTs );

	// Need to give it the camera's orientation and projection
	void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 lightMatrix);

	void LightDraw(glm::mat4 viewMatrix, glm::mat4 projMatrix);

protected:

	// The actual model geometry
	Mesh *_mesh;
	// The material contains the shader
	Material *_material;
	Material* _lightMaterial;

	// Matrix for the position and orientation of the game object
	glm::mat4 _modelMatrix;
	glm::mat4 _invModelMatrix;
	
	// Position of the model
	// The model matrix must be built from this and the _rotation
	glm::vec3 _position;
	
	// Orientation of the model
	// The model matrix must be built from this and the _position
	glm::vec3 _rotation;

	glm::vec3 _scale;
};
#endif