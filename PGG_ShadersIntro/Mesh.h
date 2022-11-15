
#ifndef __MESH__
#define __MESH__

#include <GLM/glm.hpp>
#include <SDL/SDL.h>
#include "glew.h"
#include <string>

// For loading a mesh from OBJ file and keeping a reference for it
class Mesh
{
public:

	Mesh();
	~Mesh();
	
	// OBJ file must be triangulated
	void LoadOBJ( std::string filename );

	// Draws the mesh - must have shaders applied for this to display!
	void Draw();

protected:
	
	// OpenGL Vertex Array Object
	GLuint _VAO;

	// Number of vertices in the mesh
	unsigned int _numVertices;

};


#endif
