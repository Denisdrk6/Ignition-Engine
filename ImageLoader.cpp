#include "ImageLoader.h"

ImageLoader::ImageLoader()
{
    ilInit();
    ilutRenderer(ILUT_OPENGL);
}

const ILuint ImageLoader::GenerateSingleImage(void)
{
    ILuint ImageName; // The image name to return.
    ilGenImages(1, &ImageName); // Grab a new image name.
    return ImageName; // Go wild with the return value.
}

GLuint ImageLoader::LoadImageTexture(std::string source)
{
    ILuint imageName;
    imageName = GenerateSingleImage();
    ilBindImage(imageName);

    if (!ilLoadImage(source.c_str()))
    {
        ILenum error = ilGetError();
        printf("%d: %s/n", error, iluErrorString(error));
    }
    GLuint texture = ilutGLBindTexImage();

    DeleteSingleImage(imageName);

    return texture;
}

const void ImageLoader::DeleteSingleImage(ILuint ImageName)
{
    ilDeleteImages(1, &ImageName); // Delete the image name.
    return;
}