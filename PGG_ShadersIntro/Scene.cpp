
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <SDL/SDL.h>



Scene::Scene()
{
	_cameraAngleX = 0.0f;
	_cameraAngleY = 0.0f;

	//_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5.0f));
	//_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);


	// Create a texture for the shadow map
	// Create a FBO that uses this texture for writing
	glGenFramebuffers(1, &depthMapFBO);

	//Creating 2D texture to use as depth buffer
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

	// Position of the light, in world-space
	_lightPosition = glm::vec3(0,3,0);

	// Creating a game object
	m_maxwell = new GameObject();
	m_plane = new GameObject();
	// Creating the material for the game object
	Material* maxwellMaterial = new Material();
	Material* planeMaterial = new Material();

	//Creating Light material for game object
	Material* maxwellLightMaterial = new Material();
	Material* planeLightMaterial = new Material();

	// Setting Shaders
	maxwellMaterial->LoadShaders("VertShader.txt", "FragShader.txt");
	planeMaterial->LoadShaders("VertShader.txt", "FragShader.txt");
	//Loading Light Shaders
	maxwellLightMaterial->LoadShaders("lightVertShader.txt", "lightFragShader.txt");
	planeLightMaterial->LoadShaders("lightVertShader.txt", "lightFragShader.txt");

	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	maxwellMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	planeMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f) );
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	maxwellMaterial->SetTexture("Maxwell_Diffuse.bmp");
	planeMaterial->SetTexture("WelcomeMat_diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	maxwellMaterial->SetLightPosition(_lightPosition);
	planeMaterial->SetLightPosition(_lightPosition);

	//Setting up the light space matrix
	_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	_lightView = glm::translate(glm::mat4(1.0f), _lightPosition);

	//Normal Materials
	m_maxwell->SetMaterial(maxwellMaterial);
	m_plane->SetMaterial(planeMaterial);
	//Light Materials
	m_maxwell->SetLightMaterial(maxwellLightMaterial);
	m_plane->SetLightMaterial(planeLightMaterial);

	// The mesh is the geometry for the object
	Mesh *maxwellMesh = new Mesh();
	Mesh* planeMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	maxwellMesh->LoadOBJ("Maxwell.obj");
	m_maxwell->SetMesh(maxwellMesh);
	planeMesh->LoadOBJ("WelcomeMatOBJ.obj");
	m_plane->SetMesh(planeMesh);

}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update( float deltaTs )
{
	//Update functions for game objects
	m_maxwell->Update(deltaTs);
	m_plane->Update(deltaTs);

	//_lightPosition += glm::vec3(0.0f, -0.01f, 0.0f);
	//std::cout << _lightPosition.y;
}

void Scene::Draw()
{
	// Set the FBO as the write buffer
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set the light as the camera (i.e. use the light's matrices as the camera's matrices)
	_viewMatrix = _lightView;
	_projMatrix = _lightProjection;

	// Draw scene	
    m_maxwell->LightDraw(_viewMatrix, _projMatrix);
	m_plane->LightDraw(_viewMatrix, _projMatrix);

	// Set the screen as the write buffer
	// Set the depth map texture for use in the objects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1080, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//Setting the camera matricies
	_viewMatrix = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15.0f)), _cameraAngleX, glm::vec3(1, 0, 0)), _cameraAngleY, glm::vec3(0, 1, 0));
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	// Draw scene	
	m_maxwell->Draw(_viewMatrix, _projMatrix);
	m_plane->Draw(_viewMatrix, _projMatrix);
}