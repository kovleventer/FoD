#include "animatabletexture.h"

#include "global.h"

/*!
 * @author kovlev
 */

ATexture::ATexture(SDL_Texture* staticTexture) {
	currentTexture = staticTexture;
	textureType = TextureType::STATIC;
}

ATexture::ATexture(std::vector< std::pair<SDL_Texture*, int> > textParts) {
	textureParts = textParts;
	sortTextureParts();
	totalAnimationCycleDuration = textureParts[textureParts.size() - 1].second;
	textureType = TextureType::ANIMATED;
	currentIndex = 0;
	currentTexture = textureParts[currentIndex].first;
	Global::animationHandler->addAnimatedTexture(this);
}

ATexture::ATexture(std::vector<SDL_Texture*> textParts) {
	if (textParts.size() != 8) {
		std::clog << "Error! invalid texture count" << std::endl;
		return;
	}
	
	//textParts pattern:
	//	3	4	5
	//	2		6
	//	1	0	7
	
	rotTextureParts = textParts;
	currentTexture = rotTextureParts[0];
	textureType = TextureType::ROTATING;
}

ATexture::~ATexture() {
	switch (textureType) {
		case TextureType::STATIC:
			SDL_DestroyTexture(currentTexture);
			break;
		case TextureType::ANIMATED:
			Global::animationHandler->removeAnimatedTexture(this);
			for (unsigned int i = 0; i < textureParts.size(); i++) {
				SDL_DestroyTexture(textureParts[i].first);
			}
			break;
		case TextureType::ROTATING:
			for (unsigned int i = 0; i < rotTextureParts.size(); i++) {
				SDL_DestroyTexture(rotTextureParts[i]);
			}
			break;
	}
}

Dimension ATexture::getDimensions() {
	int tw, th;
	//Built in SDL function, to get the width and the height of the texture
	//Params: texture, format, access, w, h
	SDL_QueryTexture(currentTexture, NULL, NULL, &tw, &th);
	return Dimension(tw, th);
}

TextureType ATexture::getTextureType() {
	return textureType;
}

int ATexture::getAnimCycleDur() {
	return totalAnimationCycleDuration;
}

int ATexture::getCurrentDur() {
	return textureParts[currentIndex].second;
}

void ATexture::render(SDL_Rect destinationRect) {
	SDL_RenderCopy(Global::renderer, currentTexture, NULL, &destinationRect);
}

void ATexture::nextTexture() {
	if (textureType == TextureType::ANIMATED) {
		currentIndex++;
		if (currentIndex == textureParts.size()) {
			currentIndex = 0;
		}
		currentTexture = textureParts[currentIndex].first;
	}
}

void ATexture::setRotation(RotationType rotType) {
	if (textureType == TextureType::ROTATING) {
		//Used for easier converting
		currentTexture = rotTextureParts[static_cast<int>(rotType)];
	}
}

void ATexture::sortTextureParts() {
	//FIXME uses shitty slow bubble sort
	//And even the damned code is copied
	bool swapped = true;
	int j = 0;
	std::pair<SDL_Texture*, int> tmp;
	while (swapped) {
		swapped = false;
		j++;
		for (unsigned int i = 0; i < textureParts.size() - j; i++) {
			if (textureParts[i].second > textureParts[i + 1].second) {
				tmp = textureParts[i];
				textureParts[i] = textureParts[i + 1];
				textureParts[i + 1] = tmp;
				swapped = true;
			}
		}
	}
}
