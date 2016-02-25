#pragma once

#include <glad/glad.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define SAFE_RELEASE(x) if (x) { delete[] x; x = nullptr; }

#ifdef USE_SDL_LOG
#define printf SDL_Log
#endif

#define LABEL_SIZE 32
