#ifndef __RENDERPASS_H__
#define __RENDERPASS_H__

#include "Texture.h"

class RenderPass {
protected:
	GLuint _fbo = 0, _width = 0, _height = 0;
	
	Tex2D _colorTex, _depthTex;
	
public:
	static const GLuint RP_COLOR = 0x01;
	static const GLuint RP_DEPTH = 0x02;
	
	bool init(GLuint w, GLuint h, GLuint flags = RP_COLOR);
	void release();
	
	void bind(bool clear = true);
	void unbind();
	
	const Tex2D& colorTex() const { return _colorTex; }
	const Tex2D& depthTex() const { return _depthTex; }
};


#endif