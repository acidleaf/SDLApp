#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "pch.hpp"

struct Tex2D {
	GLuint id;
	GLuint width;
	GLuint height;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};

struct Tex3D {
	GLuint id;
	GLuint width;
	GLuint height;
	GLuint depth;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};

namespace Texture {
	GLuint createTexture2D(Tex2D& tex, const void* data = nullptr);
	GLuint createTexture3D(Tex3D& tex, const void* data = nullptr);
	
	void updateTexture2D(const Tex2D& tex, const void* data);
	void updateTexture3D(const Tex3D& tex, const void* data);
};



#endif