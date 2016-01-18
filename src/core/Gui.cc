#include "Gui.h"
#include "App.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

static const char* guiVsh = R"shader_src(
#version 330

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec4 v_color;

out vec2 f_uv;
out vec4 f_baseColor;

uniform mat4 _proj;

void main() {
	f_uv = v_texCoord;
	f_baseColor = v_color;
	gl_Position = _proj * vec4(v_pos, 0, 1);
}
)shader_src";

static const char* guiFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec2 f_uv;
in vec4 f_baseColor;
out vec4 frag_color;

uniform sampler2D _tex;

void main() {
	frag_color = f_baseColor * texture(_tex, f_uv);
}
)shader_src";



void gui_SetClipboardText(const char* text) {
	SDL_SetClipboardText(text);
}
const char* gui_GetClipboardText() {
	return SDL_GetClipboardText();
}


bool Gui::init(SDL_Window* window, const char* ini, const char* log) {
	_window = window;
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)App::getInstance()->w(), (float)App::getInstance()->h());
	io.IniFilename = ini;
	io.LogFilename = log;
	
	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;

	//io.RenderDrawListsFn = ImGui_ImplSdlGL3_RenderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.RenderDrawListsFn = nullptr;
	io.SetClipboardTextFn = gui_SetClipboardText;
	io.GetClipboardTextFn = gui_GetClipboardText;

	/*
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(_window, &wmInfo);
	io.ImeWindowHandle = wmInfo.info.win.window;
#endif
	*/

	if (!initGL()) return false;

	return true;
}

bool Gui::initGL() {
	
	// Initialize shaders
	if (!_shaderProg) {
		_shaderProg = Shader::createProgramSrc(guiVsh, guiFsh);
		if (!_shaderProg) return false;
	}
	
	// Generate VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	// Generate VBO and IBO
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	
	// Vertices - 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), BUFFER_OFFSET(0));
	// TexCoords - 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), BUFFER_OFFSET(8));
	// Color - 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), BUFFER_OFFSET(16));
	
	// Create font atlas
	createFontTex();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return true;
}

void Gui::createFontTex() {
	ImGuiIO& io = ImGui::GetIO();
	
	// Build texture atlas
	unsigned char* data;
	int w, h;
	io.Fonts->GetTexDataAsRGBA32(&data, &w, &h);
	
	// Create OpenGL texture
	glGenTextures(1, &_fontTex);
	glBindTexture(GL_TEXTURE_2D, _fontTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	// Store our identifier
	io.Fonts->TexID = (void*)(intptr_t)_fontTex;
	
	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
}

void Gui::releaseGL() {
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vbo) glDeleteBuffers(1, &_vbo);
	if (_ibo) glDeleteBuffers(1, &_ibo);
	_vao = _vbo = _ibo = 0;
	
	glDeleteProgram(_shaderProg);
	_shaderProg = 0;
	
	if (_fontTex) {
		glDeleteTextures(1, &_fontTex);
        ImGui::GetIO().Fonts->TexID = 0;
        _fontTex = 0;
	}
}

void Gui::release() {
	releaseGL();
	ImGui::Shutdown();
}


void Gui::update() {
	ImGuiIO& io = ImGui::GetIO();
	
	// Update window size on every update to cater for window resizing
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	
	
	// Setup time step
	double curTime = SDL_GetTicks() / 1000.0;
	io.DeltaTime = _time > 0.0 ? (float)(curTime - _time) : (float)(1.0f / 60.0f);
	_time = curTime;
	
	
	// Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
	if (!(SDL_GetWindowFlags(_window) & SDL_WINDOW_MOUSE_FOCUS)) io.MousePos = ImVec2(-1, -1);
	else {
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		io.MousePos = ImVec2((float)mx, (float)my);
	}
	
	// Hide OS mouse cursor if ImGui is drawing it
	SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
	
	// Start the frame
	ImGui::NewFrame();
}



void Gui::render() {
	ImGui::Render();
	
	GLint lastProg; glGetIntegerv(GL_CURRENT_PROGRAM, &lastProg);
	GLint lastTex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);
	GLint lastVBO; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastVBO);
	GLint lastIBO; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &lastIBO);
	GLint lastVAO; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVAO);
	GLint lastBlendSrc; glGetIntegerv(GL_BLEND_SRC, &lastBlendSrc);
	GLint lastBlendDst; glGetIntegerv(GL_BLEND_DST, &lastBlendDst);
	GLint lastBlendEqRGB; glGetIntegerv(GL_BLEND_EQUATION_RGB, &lastBlendEqRGB);
	GLint lastBlendEqAlpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &lastBlendEqAlpha);
	
	GLint lastViewport[4];
	glGetIntegerv(GL_VIEWPORT, lastViewport);
	
	GLboolean lastBlendState = glIsEnabled(GL_BLEND);
	GLboolean lastCullFaceState = glIsEnabled(GL_CULL_FACE);
	GLboolean lastDepthTestState = glIsEnabled(GL_DEPTH_TEST);
	GLboolean lastScissorTestState = glIsEnabled(GL_SCISSOR_TEST);
	
	ImDrawData* drawData = ImGui::GetDrawData();
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);
	
	ImGuiIO& io = ImGui::GetIO();
	
	// Handle cases of screen coordinates != from framebuffer coordinates (e.g. retina displays)
	float fbHeight = io.DisplaySize.y * io.DisplayFramebufferScale.y;
	drawData->ScaleClipRects(io.DisplayFramebufferScale);
	
	
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glm::mat4 proj = glm::ortho(
		0.0f, io.DisplaySize.x,
		io.DisplaySize.y, 0.0f,
		-1.0f, 1.0f
	);
	
	glUseProgram(_shaderProg);
	GLuint uLoc = glGetUniformLocation(_shaderProg, "_proj");
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, &proj[0][0]);
	uLoc = glGetUniformLocation(_shaderProg, "_tex");
	glUniform1i(uLoc, 0);
	
	glBindVertexArray(_vao);
	
	// Draw each command in the draw list
	for (int i = 0; i < drawData->CmdListsCount; i++) {
		const ImDrawList* cmd = drawData->CmdLists[i];
		const ImDrawIdx* iboOffset = 0;
		
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd->VtxBuffer.front(), GL_STREAM_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd->IdxBuffer.front(), GL_STREAM_DRAW);
		
		for (const ImDrawCmd* pcmd = cmd->CmdBuffer.begin(); pcmd != cmd->CmdBuffer.end(); pcmd++) {
			if (pcmd->UserCallback) pcmd->UserCallback(cmd, pcmd);
			else {
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fbHeight - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, iboOffset);
			}
			iboOffset += pcmd->ElemCount;
		}
	}
	
	
	// Restore previous states
	glUseProgram(lastProg);
	glBindTexture(GL_TEXTURE_2D, lastTex);
	glBindBuffer(GL_ARRAY_BUFFER, lastVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lastIBO);
	glBindVertexArray(lastVAO);
	glBlendEquationSeparate(lastBlendEqRGB, lastBlendEqAlpha);
	glBlendFunc(lastBlendSrc, lastBlendDst);

	if (lastBlendState) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
	
	if (lastCullFaceState) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
	
	if (lastDepthTestState) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	
	if (lastScissorTestState) glEnable(GL_SCISSOR_TEST);
	else glDisable(GL_SCISSOR_TEST);
	
	glViewport(lastViewport[0], lastViewport[1], (GLsizei)lastViewport[2], (GLsizei)lastViewport[3]);
	
}




bool Gui::handleEvents(const SDL_Event& e) {
	ImGuiIO& io = ImGui::GetIO();
	
	if (e.type == SDL_MOUSEWHEEL) {
		//io.MouseWheel = e.wheel.y;
		if (e.wheel.y > 0) io.MouseWheel = 1.0f;
		if (e.wheel.y < 0) io.MouseWheel = -1.0f;
		return true;
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
		if (e.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
		if (e.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
		return true;
	} else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
		if (e.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
		if (e.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
		return true;
	} else if (e.type == SDL_MOUSEMOTION) {
		io.MousePos = ImVec2((float)e.motion.x, (float)e.motion.y);
		return true;
	}


	if (e.type == SDL_TEXTINPUT) {
		io.AddInputCharactersUTF8(e.text.text);
		return true;
	}
	if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
		int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (e.type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		return true;
	}
	
	
	return false;
}