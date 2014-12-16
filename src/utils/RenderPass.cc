#include "RenderPass.h"
#include "Texture.h"

bool RenderPass::init(GLuint width, GLuint height, GLuint flags) {
	glGetError();
	_width = width;
	_height = height;
	
	
	// Generate textures for the frame buffer
	if (flags & RP_COLOR) {
		_colorTex.width = _width;
		_colorTex.height = _height;
		Texture::createTexture2D(_colorTex);
	}
	
	
	if (flags & RP_DEPTH) {
		// Generate depth buffer
		_depthTex.width = _width;
		_depthTex.height = _height;
		_depthTex.format = GL_DEPTH_COMPONENT;
		_depthTex.type = GL_FLOAT;
		Texture::createTexture2D(_depthTex);
	}
	
	
	// Generate frame buffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	
	if (flags & RP_COLOR) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTex.id, 0);
	if (flags & RP_DEPTH) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTex.id, 0);
	
	
	
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Frame buffer error: %d\n", status);
		return false;
	}
	
	// Reset to original state
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}


void RenderPass::bind(bool clear) {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPass::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderPass::release() {
	glDeleteTextures(1, &_colorTex.id);
	glDeleteTextures(1, &_depthTex.id);
	glDeleteFramebuffers(1, &_fbo);
}