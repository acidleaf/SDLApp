// Precompiled header file

#include <GL/glew.h>

#if defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <cstdio>
#include <climits>
#include <cstring>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>