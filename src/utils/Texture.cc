#include "Texture.h"


GLuint Texture::createTexture(TexData& tex, const void* data) {
	
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, tex.format, tex.width, tex.height, 0, tex.format, GL_UNSIGNED_BYTE, data);
	
	
	return tex.id;
}

void Texture::updateTexture(const TexData& tex, const void* data) {
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width, tex.height, tex.format, GL_UNSIGNED_BYTE, data);
}

