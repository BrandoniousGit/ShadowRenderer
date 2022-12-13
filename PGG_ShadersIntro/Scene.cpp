
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <SDL/SDL.h>



Scene::Scene()
{
	_cameraAngleX = 0.0f;
	_cameraAngleY = 0.0f;

	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5.0f));
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);


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
	_lightPosition = glm::vec3(-2.0f, 5.0f, -1.0f);

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

	// Setting default Textures
	maxwellMaterial->SetTexture("Maxwell_Diffuse.bmp");
	planeMaterial->SetTexture("WelcomeMat_diffuse.bmp");

	// Setting the Shadow Maps
	maxwellMaterial->SetShadowMap(depthMap);
	planeMaterial->SetShadowMap(depthMap);

	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	maxwellMaterial->SetLightPosition(_lightPosition);
	planeMaterial->SetLightPosition(_lightPosition);

	//Setting up the light space matrix
	_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	//_lightView = glm::translate(glm::mat4(1.0f), _lightPosition);
	_lightView = glm::lookAt(glm::vec3(-1.0f, 5.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

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

	_viewMatrix = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15.0f)), _cameraAngleX, glm::vec3(1, 0, 0)), _cameraAngleY, glm::vec3(0, 1, 0));
	//_lightPosition += glm::vec3(0.0f, -0.01f, 0.0f);
	//std::cout << _lightPosition.y;
}

void Scene::Draw()
{
	// Set the FBO as the write buffer
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw scene from light's POV
    m_maxwell->LightDraw(_lightView, _lightProjection);
	m_plane->LightDraw(_lightView, _lightProjection);

	// Set the screen as the write buffer
	// Set the depth map texture for use in the objects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1080, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw scene from Camera's POV
	m_maxwell->Draw(_viewMatrix, _projMatrix, _lightSpaceMatrix);
	m_plane->Draw(_viewMatrix, _projMatrix, _lightSpaceMatrix);
}