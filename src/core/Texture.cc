#include "Texture.h"

GLuint Texture::create(Tex2D& tex, const void* data) {
	
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, tex.format, tex.width, tex.height, 0, tex.format, tex.type, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return tex.id;
}

void Texture::update(const Tex2D& tex, const void* data) {
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width, tex.height, tex.format, tex.type, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release(Tex2D& tex) {
	glDeleteTextures(1, &tex.id);
	tex.id = 0;
	tex.width = tex.height = 0;
}