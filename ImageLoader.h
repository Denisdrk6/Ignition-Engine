#pragma once
#include "glew.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include <string>

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ilu.lib")
#pragma comment (lib, "DevIL/libx86/ilut.lib")

class ImageLoader {
public:
	ImageLoader();

	~ImageLoader() {};

	const ILuint GenerateSingleImage(void);
	const void DeleteSingleImage(ILuint ImageName);

	GLuint LoadImageTexture(std::string source);
};