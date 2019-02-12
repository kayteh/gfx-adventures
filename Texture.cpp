#include "Texture.h"
#include <stdinc.h>

Texture::Texture(string path, int texSample) {
  vector<unsigned char> pixels;
  unsigned int width, height;
  lodepng::decode(pixels, width, height, path);

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texSample);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texSample);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels.front());
  glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {}