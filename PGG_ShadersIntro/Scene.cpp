
#include "Scene.h"

#include <iostream>
#include <SDL/SDL.h>



Scene::Scene()
{
	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;

	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );
	

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	// Set up your scene here

	// Position of the light, in world-space
	_lightPosition = glm::vec3(10,10,0);

	// Create a game object
	// This needs a material and a mesh
	_model = new GameObject();
	m_gameObjects.push_back(_model);

	_plane = new GameObject();
	m_gameObjects.push_back(_plane);

	// Create the material for the game object
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("VertShader.txt","FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour( glm::vec3(1.0f,1.0f,1.0f) );
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("Maxwell_Diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the game object to use this material
	_model->SetMaterial(modelMaterial);
	_plane->SetMaterial(modelMaterial);

	// The mesh is the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("Maxwell.obj");
	// Tell the game object to use this mesh
	_model->SetMesh(modelMesh);
	_plane->SetMesh(modelMesh);

}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update( float deltaTs )
{
	// Update the game object (this is currently hard-coded to rotate)
	_model->Update( deltaTs );
	_plane->Update(deltaTs);

	// This updates the camera's position and orientation
	_viewMatrix = glm::rotate( glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3(0,0,-3.5f) ), _cameraAngleX, glm::vec3(1,0,0) ), _cameraAngleY, glm::vec3(0,1,0) );
}

void Scene::Draw()
{
	// Draw that model, giving it the camera's position and projection
	_model->Draw(_viewMatrix, _projMatrix);
	_plane->Draw(_viewMatrix, _projMatrix);

}



