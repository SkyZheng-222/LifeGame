#include "Texture.h"

SDL_Renderer* gRenderer = nullptr;

Texture::Texture() {
  // Initialize
  mTexture = nullptr;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() {
  // Deallocate
  free();
}

bool Texture::loadFromFile(const std::string& path) {
  // Get rid of preexisting texture
  free();

  // The final texture
  SDL_Texture* newTexture = nullptr;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == nullptr) {
	printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
		   IMG_GetError());
  } else {
	// Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE,
					SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	// Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == nullptr) {
	  printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
			 SDL_GetError());
	} else {
	  // Get image dimensions
	  mWidth = loadedSurface->w;
	  mHeight = loadedSurface->h;
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
  }

  // Return success
  mTexture = newTexture;
  return mTexture != nullptr;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRenderedText(std::string textureText,
                                    SDL_Color textColor) {
  // Get rid of preexisting texture
  free();

  // Render text surface
  SDL_Surface* textSurface =
      TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
  if (textSurface != nullptr) {
    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == nullptr) {
      printf("Unable to create texture from rendered text! SDL Error: %s\n",
             SDL_GetError());
    } else {
      // Get image dimensions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }

    // Get rid of old surface
    SDL_FreeSurface(textSurface);
  } else {
    printf("Unable to render text surface! SDL_ttf Error: %s\n",
           TTF_GetError());
  }

  // Return success
  return mTexture != nullptr;
}
#endif

void Texture::free() {
  // Free texture if it exists
  if (mTexture != nullptr) {
	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
  }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  // Modulate texture rgb
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle,
					  SDL_Point* center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  // Set clip rendering dimensions
  if (clip != nullptr) {
	renderQuad.w = clip->w;
	renderQuad.h = clip->h;
  }

  // Render to screen
  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() const { return mWidth; }

int Texture::getHeight() const { return mHeight; }