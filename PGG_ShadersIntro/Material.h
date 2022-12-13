
#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <GLM/glm.hpp>
#include "glew.h"

// Encapsulates shaders and textures
class Material
{
public:
	Material();
	~Material();

	// Loads shaders from file
	// Returns false if there was an error - it will also print out messages to console
	bool LoadShaders( std::string vertFilename, std::string fragFilename );

	// For setting the standard matrices needed by the shader
	void SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix);
	void SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 lightMatrix);
	
	// For setting material properties
	void SetEmissiveColour( glm::vec3 input ) { _emissiveColour = input;}
	void SetDiffuseColour( glm::vec3 input ) { _diffuseColour = input;}
	void SetSpecularColour( glm::vec3 input ) { _specularColour = input;}

	// Set light position in world space
	void SetLightPosition( glm::vec3 input ) { _lightPosition = input;}

	// Sets texture
	// This applies to ambient, diffuse and specular colours
	// If you want textures for anything else, you'll need to do that yourself ;) 
	bool SetTexture( std::string filename ) { _texture1 = LoadTexture(filename); return _texture1>0; }
	bool SetShadowMap( unsigned int value ) { _shadowMap = value;  return _shadowMap>0; }

	// Sets the material, applying the shaders
	void Apply();

protected:

	// Utility function
	bool CheckShaderCompiled( GLint shader );
	
	// The OpenGL shader program handle
	int _shaderProgram;

	// Locations of Uniforms in the vertex shader
	int _shaderModelMatLocation;
	int _shaderInvModelMatLocation;
	int _shaderViewMatLocation;
	int _shaderProjMatLocation;
	int _shaderLightSpaceMatrixMatLocation;
	int _shaderShadowMapSamplerLocation;

	// Location of Uniforms in the fragment shader
	int _shaderDiffuseColLocation, _shaderEmissiveColLocation, _shaderSpecularColLocation;
	int _shaderWSLightPosLocation;
	int _shaderTex1SamplerLocation;

	// Local store of material properties to be sent to the shader
	glm::vec3 _emissiveColour, _diffuseColour, _specularColour;
	glm::vec3 _lightPosition;

	// Loads a .bmp from file
	unsigned int LoadTexture( std::string filename );
	
	// OpenGL handle for the texture
	unsigned int _texture1;
	unsigned int _shadowMap;
};
#endif
