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

bool Gui::initImGui() {
	int w = App::getInstance()->resX();
	int h = App::getInstance()->resY();
	
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DeltaTime = 1.0f / 60.0f;
	io.PixelCenterOffset = 0.0f;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDLK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDLK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDLK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDLK_DOWN;
    io.KeyMap[ImGuiKey_Home] = SDLK_HOME;
    io.KeyMap[ImGuiKey_End] = SDLK_END;
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
	
	io.RenderDrawListsFn = gui_RenderDrawLists;
	//io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
	//io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;
	
	
	
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
	ImGuiIO& io = ImGui::GetIO();
	
	// Setup timestep
	static float prevTime = 0.0f;
	const float curTime = SDL_GetTicks() * 0.001f;
	io.DeltaTime = (curTime - prevTime) / 1000.0f;
	prevTime = curTime;
	
	
	// Start the frame
	ImGui::NewFrame();
	
}


// This function shall be called after all your ImGui drawing code are called
void Gui::render() {
	ImGui::Render();
}

void gui_RenderDrawLists(ImDrawList** const lists, int count) {
	App::getInstance()->gui()->imguiRender(lists, count);
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
	} if (e.type == SDL_MOUSEWHEEL) {
		io.MouseWheel += (float)e.wheel.y;
	}
}