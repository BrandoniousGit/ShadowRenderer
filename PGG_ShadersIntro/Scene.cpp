
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
	glCullFace(GL_FRONT);
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
	glCullFace(GL_BACK);

	// Position of the light, in world-space
	_lightPosition = glm::vec3(2.0f, 5.0f, 1.0f);

	// Creating a game object
	m_maxwell = new GameObject();
	m_plane = new GameObject();
	m_flopp = new GameObject();

	// Creating the material for the game object
	maxwellMaterial = new Material();
	planeMaterial = new Material();
	floppMaterial = new Material();

	//Creating Light material for game object
	Material* maxwellLightMaterial = new Material();
	Material* planeLightMaterial = new Material();
	Material* floppLightMaterial = new Material();

	// Setting Shaders
	maxwellMaterial->LoadShaders("Resources/VertShader.txt", "Resources/FragShader.txt");
	planeMaterial->LoadShaders("Resources/VertShader.txt", "Resources/FragShader.txt");
	floppMaterial->LoadShaders("Resources/VertShader.txt", "Resources/FragShader.txt");

	//Loading Light Shaders
	maxwellLightMaterial->LoadShaders("Resources/lightVertShader.txt", "Resources/lightFragShader.txt");
	planeLightMaterial->LoadShaders("Resources/lightVertShader.txt", "Resources/lightFragShader.txt");
	floppLightMaterial->LoadShaders("Resources/lightVertShader.txt", "Resources/lightFragShader.txt");

	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	maxwellMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	planeMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f) );
	floppMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));

	// Setting default Textures
	maxwellMaterial->SetTexture("Resources/Maxwell_Diffuse.bmp");
	planeMaterial->SetTexture("Resources/WelcomeMat_diffuse.bmp");
	floppMaterial->SetTexture("Resources/Maxwell_Diffuse_Inverted.bmp");

	// Setting the Shadow Maps
	maxwellMaterial->SetShadowMap(depthMap);
	planeMaterial->SetShadowMap(depthMap);
	floppMaterial->SetShadowMap(depthMap);

	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	maxwellMaterial->SetLightPosition(_lightPosition);
	planeMaterial->SetLightPosition(_lightPosition);
	floppMaterial->SetLightPosition(_lightPosition);

	//Setting up the light space matrix
	_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	//_lightView = glm::translate(glm::mat4(1.0f), _lightPosition);
	_lightView = glm::lookAt(glm::vec3(_lightPosition),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//Normal Materials
	m_maxwell->SetMaterial(maxwellMaterial);
	m_plane->SetMaterial(planeMaterial);
	m_flopp->SetMaterial(floppMaterial);

	//Light Materials
	m_maxwell->SetLightMaterial(maxwellLightMaterial);
	m_plane->SetLightMaterial(planeLightMaterial);
	m_flopp->SetLightMaterial(floppLightMaterial);

	// The mesh is the geometry for the object
	Mesh *maxwellMesh = new Mesh();
	Mesh* planeMesh = new Mesh();
	Mesh* floppMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	maxwellMesh->LoadOBJ("Resources/Maxwell.obj");
	m_maxwell->SetMesh(maxwellMesh);

	planeMesh->LoadOBJ("Resources/WelcomeMatOBJ.obj");
	m_plane->SetMesh(planeMesh);

	floppMesh->LoadOBJ("Resources/Maxwell.obj");
	m_flopp->SetMesh(floppMesh);
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
	m_flopp->Update(deltaTs);

	_viewMatrix = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15.0f)), _cameraAngleX, glm::vec3(1, 0, 0)), _cameraAngleY, glm::vec3(0, 1, 0));
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
	m_flopp->LightDraw(_lightView, _lightProjection);

	// Set the screen as the write buffer
	// Set the depth map texture for use in the objects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1080, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_lightSpaceMatrix = _lightProjection * _lightView;
	// Draw scene from Camera's POV
	m_maxwell->Draw(_viewMatrix, _projMatrix, _lightSpaceMatrix);
	m_plane->Draw(_viewMatrix, _projMatrix, _lightSpaceMatrix);
	m_flopp->Draw(_viewMatrix, _projMatrix, _lightSpaceMatrix);
}