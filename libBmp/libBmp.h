#pragma once

extern "C" bool bmpLoad(const char* filePath, unsigned char** data, int* width, int* height);
extern "C" void bmpSave(const char* filePath, unsigned char* data, int width, int height);
