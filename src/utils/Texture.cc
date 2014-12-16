#include "Texture.h"


GLuint Texture::createTexture2D(Tex2D& tex, const void* data) {
	
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, tex.format, tex.width, tex.height, 0, tex.format, tex.type, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return tex.id;
}


GLuint Texture::createTexture3D(Tex3D& tex, const void* data) {
	
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_3D, tex.id);
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, tex.format, tex.width, tex.height, tex.depth, 0, tex.format, tex.type, data);
	
	glBindTexture(GL_TEXTURE_3D, 0);
	
	return tex.id;
}
