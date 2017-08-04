#include "Texture.h"


Texture::Texture(const string& file)
{
	// STB image loading
	int width, height, numComp; // Width, height and number of components in the image
	unsigned char* image = stbi_load(file.c_str(),&width,&height,&numComp,4); // Load texture file

	// Error checking
	if (image == NULL)
		cerr << "Image failed to load in Texture class: " << file << endl;

	// Begin preparing texture paramenters
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wraps the texture around X/width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wraps the texture around Y/height
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Extend texture when needed
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Trim texture when needed
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	// Send texture to GPU

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image); // Free up space
}	

void Texture::Bind(unsigned int numTexture)
{
	assert(numTexture >= 0 && numTexture <= 31);
	glActiveTexture(GL_TEXTURE0 + numTexture); // Set active texture unit, OpenGL is sequential so we can just add the number of what texture we want onto the end
	glBindTexture(GL_TEXTURE_2D, texture); // Bind our texture
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture); // Removes texture
}
