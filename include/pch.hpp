// Precompiled header file

#include <GL/glew.h>

#if defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <cstdio>
#include <climits>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
