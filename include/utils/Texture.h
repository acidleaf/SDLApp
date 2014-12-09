#ifndef __TEXTURE_H__
#define __TEXTURE_H__


struct TexData {
	GLuint id;
	GLuint width;
	GLuint height;
	GLenum format;
};

namespace Texture {
	GLuint createTexture(TexData& tex, const void* data = nullptr);
	void updateTexture(const TexData& tex, const void* data);
};



#endif