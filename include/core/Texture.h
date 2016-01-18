#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "pch.h"

struct Tex2D {
	GLuint id;
	GLuint width;
	GLuint height;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};


namespace Texture {
	GLuint create(Tex2D& tex, const void* data = nullptr);
	void update(const Tex2D& tex, const void* data);
	void release(Tex2D& tex);
};



#endif