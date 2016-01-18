#ifndef __UTILS_H__
#define __UTILS_H__

#include "pch.h"

uint32_t crc32(const uint8_t* data, size_t length, uint32_t prev = 0);

#endif