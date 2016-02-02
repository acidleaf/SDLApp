#pragma once

#include <glad/glad.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
//#include <SDL2/SDL_net.h>
#else
#include <SDL.h>
#include <SDL_net.h>
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

#define MAX_ROI 100
#define MAX_RESULTS 500
#define LABEL_SIZE 32

#define HOSTNAME_SIZE 128
#define RCV_BUFFER_SIZE 1024

#define LOCATER_LBL "locater"