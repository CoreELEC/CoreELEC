#pragma once
#ifndef ES_CORE_RESOURCES_TEXTURE_RESOURCE_H
#define ES_CORE_RESOURCES_TEXTURE_RESOURCE_H

#include "math/Vector2i.h"
#include "math/Vector2f.h"
#include "resources/ResourceManager.h"
#include "resources/TextureDataManager.h"
#include "resources/TextureData.h"
#include <set>
#include <string>

// An OpenGL texture.
// Automatically recreates the texture with renderer deinit/reinit.
class TextureResource : public IReloadable
{
protected:
	TextureResource(const std::string& path, bool tile, bool dynamic, bool allowAsync, MaxSizeInfo* maxSize = nullptr);

public:
	static void cancelAsync(std::shared_ptr<TextureResource> texture);
	static std::shared_ptr<TextureResource> get(const std::string& path, bool tile = false, bool forceLoad = false, bool dynamic = true, bool asReloadable = true, MaxSizeInfo* maxSize = nullptr);
	void initFromPixels(unsigned char* dataRGBA, size_t width, size_t height);
	virtual void initFromMemory(const char* file, size_t length);

	// For scalable source images in textures we want to set the resolution to rasterize at
	void rasterizeAt(size_t width, size_t height);
	Vector2f getSourceImageSize() const;

	virtual ~TextureResource();

	bool isLoaded() const;
	bool isTiled() const;

	const Vector2i getSize() const;
	bool bind();

	static size_t getTotalMemUsage(); // returns an approximation of total VRAM used by textures (in bytes)
	static size_t getTotalTextureSize(); // returns the number of bytes that would be used if all textures were in memory
	
	virtual bool unload();
	virtual void reload();

	void onTextureLoaded(std::shared_ptr<TextureData> tex);

	static void clearQueue();

private:
	// mTextureData is used for textures that are not loaded from a file - these ones
	// are permanently allocated and cannot be loaded and unloaded based on resources
	std::shared_ptr<TextureData>		mTextureData;

	// The texture data manager manages loading and unloading of filesystem based textures
	static TextureDataManager		sTextureDataManager;

	Vector2i					mSize;
	Vector2f					mSourceSize;
	bool							mForceLoad;

	typedef std::pair<std::string, bool> TextureKeyType;
	static std::map< TextureKeyType, std::weak_ptr<TextureResource> > sTextureMap; // map of textures, used to prevent duplicate textures
	static std::set<TextureResource*> 	sAllTextures;	// Set of all textures, used for memory management
};

#endif // ES_CORE_RESOURCES_TEXTURE_RESOURCE_H
