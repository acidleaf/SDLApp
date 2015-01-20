#include "Gui.h"
#include "App.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



void gui_RenderDrawLists(ImDrawList** const lists, int count);


// Shaders
//////////////////////////////
// GUI Shaders
//////////////////////////////
const char* guiVsh = R"shader_src(
#version 330 core
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

const char* guiFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec2 f_uv;
in vec4 f_baseColor;
out vec4 frag_color;

uniform sampler2D fontTex;


void main() {
	frag_color = f_baseColor * texture(fontTex, f_uv);
}

)shader_src";



bool Gui::init() {
	
	auto app = App::getInstance();
	
	_prevTime = SDL_GetTicks();
	
	// Setup ImGui stuffs
	if (!initImGui()) return false;
	
	
	// Init stuff for OpenGL rendering
	
	// Generate VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	// Generate VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ImDrawVert) * _maxVBOSize, nullptr, GL_STREAM_DRAW);
	// Vertices - 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), BUFFER_OFFSET(0));
	
	// TexCoords - 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), BUFFER_OFFSET(8));
	
	// Color - 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), BUFFER_OFFSET(16));
	
	
	// Load and compile shaders
	if (!_guiShader) {
		_guiShader = Shader::createProgramSrc(guiVsh, guiFsh);
		if (!_guiShader) return false;
	}
	
	// Setup MVP matrix
	_proj = glm::ortho(
		0.0f, (GLfloat)(app->resX()),
		(GLfloat)(app->resY()), 0.0f,
		-1.0f, 1.0f
	);
	
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	return true;
}


void Gui::release() {
	glDeleteTextures(1, &_fontTexID);
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vbo) glDeleteBuffers(1, &_vbo);
	glDeleteProgram(_guiShader);
	ImGui::Shutdown();
}

void gui_RenderDrawLists(ImDrawList** const lists, int count) {
	App::getInstance()->gui()->imguiRender(lists, count);
}
void gui_SetClipboardFunc(const char* text) {
	SDL_SetClipboardText(text);
}
const char* gui_GetClipboardFunc() {
	return SDL_GetClipboardText();
}


bool Gui::initImGui() {
	int w = App::getInstance()->resX();
	int h = App::getInstance()->resY();
	
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DeltaTime = 1.0f / 60.0f;
	io.PixelCenterOffset = 0.0f;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
	
	io.RenderDrawListsFn = gui_RenderDrawLists;
	io.SetClipboardTextFn = gui_SetClipboardFunc;
	io.GetClipboardTextFn = gui_GetClipboardFunc;
	
	
	
	// Generate font texture
	// GUI will handle its own texture loading and stuff
	glGenTextures(1, &_fontTexID);
	glBindTexture(GL_TEXTURE_2D, _fontTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	
	const void* pngData;
	GLuint pngSize;
	ImGui::GetDefaultFontData(nullptr, nullptr, &pngData, &pngSize);
	int texW, texH, texComp;
	void* texData = stbi_load_from_memory((const GLubyte*)pngData, (int)pngSize, &texW, &texH, &texComp, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	stbi_image_free(texData);
	
	
	return true;
}


void Gui::update() {
	// Start the frame
	ImGui::NewFrame();
	
	
	ImGuiIO& io = ImGui::GetIO();
	
	// Setup timestep
	const GLuint curTime = SDL_GetTicks();
	if (curTime == _prevTime) io.DeltaTime = 1.0f / 60.0f;
	else io.DeltaTime = (curTime - _prevTime) * 0.001f;
	_prevTime = curTime;
	
	
	// Deal with text input
	if (io.WantCaptureKeyboard) SDL_StartTextInput();
	else SDL_StopTextInput();
	
}


// This function shall be called after all your ImGui drawing code are called
void Gui::render() {
	ImGui::Render();
}




void Gui::imguiRender(ImDrawList** const lists, int listCount) {
	if (listCount == 0) return;
	
	glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _fontTexID);
	
	
	glUseProgram(_guiShader);
	
	GLuint uLoc = glGetUniformLocation(_guiShader, "_proj");
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, &_proj[0][0]);
	uLoc = glGetUniformLocation(_guiShader, "fontTex");
	glUniform1i(uLoc, 0);
	
	/*
	// Grow our buffer according to what we need
    size_t total_vtx_count = 0;
	for (int i = 0; i < listCount; i++) {
		total_vtx_count += lists[i]->vtx_buffer.size();
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
	if (neededBufferSize > _maxVBOSize) {
		_maxVBOSize = neededBufferSize + 5000;
		glBufferData(GL_ARRAY_BUFFER, _maxVBOSize, nullptr, GL_STREAM_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
	
	
	// Actual rendering of the ImDrawLists
	glBindVertexArray(_vao);
	for (int i = 0; i < listCount; ++i) {
		
		const ImDrawList* cmdList = lists[i];
		const unsigned char* vtx_buffer = (const unsigned char*)cmdList->vtx_buffer.begin();
		
		
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ImDrawVert) * cmdList->vtx_buffer.size(), vtx_buffer);
		
		// I got lazy here, copied from the example
		int vtx_offset = 0;
		const ImDrawCmd* pcmd_end = cmdList->commands.end();
		for (const ImDrawCmd* pcmd = cmdList->commands.begin(); pcmd != pcmd_end; pcmd++) {
			
			glScissor((int)pcmd->clip_rect.x, (int)(ImGui::GetIO().DisplaySize.y - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
			
			glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
			vtx_offset += pcmd->vtx_count;
		}
		
	}
	
	/*
	// Grow our buffer according to what we need
    size_t total_vtx_count = 0;
	for (int i = 0; i < listCount; i++) {
		total_vtx_count += lists[i]->vtx_buffer.size();
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
	if (neededBufferSize > _maxVBOSize) {
		_maxVBOSize = neededBufferSize + 5000;
		glBufferData(GL_ARRAY_BUFFER, _maxVBOSize, nullptr, GL_STREAM_DRAW);
	}
	
	
	// Copy and convert all vertices into a single contiguous buffer
	unsigned char* bufferData = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (!bufferData) return;
    for (int i = 0; i < listCount; i++) {
        const ImDrawList* cmd_list = lists[i];
        memcpy(bufferData, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
        bufferData += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
    }
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	// Actual rendering of the ImDrawLists
	glBindVertexArray(_vao);
	
	int cmd_offset = 0;
	for (int i = 0; i < listCount; ++i) {
		const ImDrawList* cmdList = lists[i];
		int vtx_offset = cmd_offset;
		const ImDrawCmd* pcmd_end = cmdList->commands.end();
		for (const ImDrawCmd* pcmd = cmdList->commands.begin(); pcmd != pcmd_end; pcmd++) {
			glScissor((int)pcmd->clip_rect.x, (int)(ImGui::GetIO().DisplaySize.y - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
			glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
			cmd_offset += pcmd->vtx_count;
		}
	}
	*/
	
	glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
	
}

////////////////////////////////////////
// Input handling functions
////////////////////////////////////////
void Gui::handleEvents(const SDL_Event& e) {
	ImGuiIO& io = ImGui::GetIO();
	
	if (e.type == SDL_MOUSEMOTION) {
		io.MousePos = ImVec2((float)e.motion.x, (float)e.motion.y);
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
		if (e.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
		if (e.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
	}
	if (e.type == SDL_MOUSEWHEEL) {
		io.MouseWheel += (float)e.wheel.y;
	}
	
	
	if (e.type == SDL_KEYDOWN) {
		io.KeysDown[e.key.keysym.scancode] = true;
		
		SDL_Keymod mod = SDL_GetModState();
		io.KeyCtrl = (mod & KMOD_CTRL);
		io.KeyShift = (mod & KMOD_SHIFT);
		
		
		
		/*
		if (c >= 0x00 & c <= 0x7A) {
			if (c >= 'a' && c <= 'z') {
				if (io.KeyShift) io.AddInputCharacter(c - 0x20);
				else io.AddInputCharacter(c);
			} else if (c >= '0' && c <= '9') {
				if (io.KeyShift) io.AddInputCharacter(c - 0x0F);
				else io.AddInputCharacter(c);
			} else io.AddInputCharacter(c);
		}
		*/
		
	} else if (e.type == SDL_KEYUP) {
		io.KeysDown[e.key.keysym.scancode] = false;
		io.KeyCtrl = (e.key.keysym.mod & KMOD_CTRL);
		io.KeyShift = (e.key.keysym.mod & KMOD_SHIFT);
	}
	
	
	if (e.type == SDL_TEXTINPUT) {
		const char* text = e.text.text;
		for (int i = 0; text[i] != 0; ++i) {
			io.AddInputCharacter(text[i]);
		}
	}
}