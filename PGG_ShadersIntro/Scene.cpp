
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <SDL/SDL.h>



Scene::Scene()
{
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5.0f));
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);


	// Create a texture for the shadow map
	// Create a FBO that uses this texture for writing
	glGenFramebuffers(1, &depthMapFBO);


	//Creating 2D texture to use as depth buffer
	//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Attaching depth texture to depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//Framebuffer object is not complete without a color buffer so explicity setting color data to GL_NONE
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Set up the viewing matrix
	// This represents the camera's orientation and position
	//_viewMatrix = m_camera->GetViewMatrix();
	

	// Set up a projection matrix
	//_projMatrix = m_camera->GetProjMatrix();

	// Set up your scene here

	// Position of the light, in world-space
	_lightPosition = glm::vec3(10,10,0);

	// Create a game object
	// This needs a material and a mesh
	m_maxwell = new GameObject();

	m_plane = new GameObject();

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
	m_maxwell->SetMaterial(modelMaterial);
	m_plane->SetMaterial(modelMaterial);

	// The mesh is the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("Maxwell.obj");
	// Tell the game object to use this mesh
	m_maxwell->SetMesh(modelMesh);
	m_plane->SetMesh(modelMesh);

}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update( float deltaTs )
{
	// Update the game object (this is currently hard-coded to rotate)
	m_maxwell->Update(deltaTs);
	m_plane->Update(deltaTs);

	// This updates the camera's position and orientation
	//_viewMatrix = glm::rotate( glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3(0,0,-3.5f) ), 0.0f, glm::vec3(1,0,0) ), 0.0f, glm::vec3(0,1,0) );
}

void Scene::Draw()
{

	// Set the FBO as the write buffer
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set the light as the camera (i.e. use the light's matrices as the camera's matrices)
	//_viewMatrix = m_light->GetViewMatrix();
	//_projMatrix = m_light->GetProjMatrix();

		// Draw scene	
		// Draw that model, giving it the camera's position and projection

		//m_maxwell->Draw(_viewMatrix, _projMatrix);
		//m_plane->Draw(_viewMatrix, _projMatrix);


	// Set the screen as the write buffer

	// Set the depth map texture for use in the objects


		// Draw scene	
		// Draw that model, giving it the camera's position and projection
		m_maxwell->Draw(_viewMatrix, _projMatrix);
		m_plane->Draw(_viewMatrix, _projMatrix);






}



