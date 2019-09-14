#include "resources/TextureResource.h"

#include "utils/FileSystemUtil.h"
#include "resources/TextureData.h"
#include <cstring>
#include "Settings.h"

TextureDataManager		TextureResource::sTextureDataManager;
std::map< TextureResource::TextureKeyType, std::weak_ptr<TextureResource> > TextureResource::sTextureMap;
std::set<TextureResource*> 	TextureResource::sAllTextures;

TextureResource::TextureResource(const std::string& path, bool tile, bool dynamic, bool allowAsync, MaxSizeInfo* maxSize) : mTextureData(nullptr), mForceLoad(false)
{
	// Create a texture data object for this texture
	if (!path.empty())
	{
		// If there is a path then the 'dynamic' flag tells us whether to use the texture
		// data manager to manage loading/unloading of this texture
		std::shared_ptr<TextureData> data;
		if (dynamic)
		{
			data = sTextureDataManager.add(this, tile);
			if (maxSize != nullptr)
				data->setMaxSize(*maxSize);

			data->initFromPath(path);

			bool async = false;

			std::shared_ptr<ResourceManager>& rm = ResourceManager::getInstance();
			auto fullpath = rm->getResourcePath(path);

			unsigned int width, height;

			if (allowAsync && Settings::getInstance()->getBool("AsyncImages") && ImageIO::loadImageSize(fullpath.c_str(), &width, &height))
			{
				data->setTemporarySize(width, height);
				async = true;
			}

			// Force the texture manager to load it using a blocking load
			sTextureDataManager.load(data, !async); // 

			if (async)
			{
				mSize = Vector2i(width, height);
				mSourceSize = Vector2f(width, height);
			}
			else
			{
				mSize = Vector2i((int)data->width(), (int)data->height());
				mSourceSize = Vector2f(data->sourceWidth(), data->sourceHeight());
			}
		}
		else
		{
			mTextureData = std::shared_ptr<TextureData>(new TextureData(tile));
			data = mTextureData;
			if (maxSize != nullptr)
				data->setMaxSize(*maxSize);
			data->initFromPath(path);
			// Load it so we can read the width/height
			data->load();
		}

		mSize = Vector2i((int)data->width(), (int)data->height());
		mSourceSize = Vector2f(data->sourceWidth(), data->sourceHeight());
	}
	else
	{
		// Create a texture managed by this class because it cannot be dynamically loaded and unloaded
		mTextureData = std::shared_ptr<TextureData>(new TextureData(tile));
	}

	if (sAllTextures.find(this) == sAllTextures.end())
		sAllTextures.insert(this);
}

TextureResource::~TextureResource()
{
	if (mTextureData == nullptr)
		sTextureDataManager.remove(this);

	if (sAllTextures.size() > 0)
	{
		auto pthis = sAllTextures.find(this);
		if (pthis != sAllTextures.end())
			sAllTextures.erase(pthis);
	}
}

void TextureResource::onTextureLoaded(std::shared_ptr<TextureData> tex)
{
	mSize = Vector2i((int)tex->width(), (int)tex->height());
	mSourceSize = Vector2f(tex->sourceWidth(), tex->sourceHeight());
}

void TextureResource::initFromPixels(unsigned char* dataRGBA, size_t width, size_t height)
{
	// This is only valid if we have a local texture data object
	assert(mTextureData != nullptr);
	mTextureData->releaseVRAM();

	// FCA optimisation, if streamed image size is already the same, don't free/reallocate memory (which is slow), just copy bytes
	if (mTextureData->getDataRGBA() != nullptr && mSize.x() == width && mSize.y() == height)
	{
		memcpy(mTextureData->getDataRGBA(), dataRGBA, width * height * 4);
		return;
	}

	mTextureData->releaseRAM();
	mTextureData->initFromRGBA(dataRGBA, width, height);
	// Cache the image dimensions
	mSize = Vector2i((int)width, (int)height);
	mSourceSize = Vector2f(mTextureData->sourceWidth(), mTextureData->sourceHeight());
}

void TextureResource::initFromMemory(const char* data, size_t length)
{
	// This is only valid if we have a local texture data object
	assert(mTextureData != nullptr);
	mTextureData->releaseVRAM();
	mTextureData->releaseRAM();
	mTextureData->initImageFromMemory((const unsigned char*)data, length);
	// Get the size from the texture data
	mSize = Vector2i((int)mTextureData->width(), (int)mTextureData->height());
	mSourceSize = Vector2f(mTextureData->sourceWidth(), mTextureData->sourceHeight());
}

const Vector2i TextureResource::getSize() const
{
	return mSize;
}

bool TextureResource::isTiled() const
{
	if (mTextureData != nullptr)
		return mTextureData->tiled();

	std::shared_ptr<TextureData> data = sTextureDataManager.get(this, false);
	return data->tiled();
}

bool TextureResource::bind()
{
	if (mTextureData != nullptr)
	{
		mTextureData->uploadAndBind();
		return true;
	}

	return sTextureDataManager.bind(this);	
}

void TextureResource::cancelAsync(std::shared_ptr<TextureResource> texture)
{
	if (texture == nullptr)
		return;

	sTextureDataManager.cancelAsync(texture.get());
}

std::shared_ptr<TextureResource> TextureResource::get(const std::string& path, bool tile, bool forceLoad, bool dynamic, bool asReloadable, MaxSizeInfo* maxSize)
{
	std::shared_ptr<ResourceManager>& rm = ResourceManager::getInstance();

	const std::string canonicalPath = Utils::FileSystem::getCanonicalPath(path);
	if(canonicalPath.empty())
	{
		std::shared_ptr<TextureResource> tex(new TextureResource("", tile, false, false));
		rm->addReloadable(tex); //make sure we get properly deinitialized even though we do nothing on reinitialization
		return tex;
	}

	// internal resources should not be dynamic
	if (canonicalPath.length() > 0 && canonicalPath[0] == ':')
		dynamic = false;

	TextureKeyType key(canonicalPath, tile);
	auto foundTexture = sTextureMap.find(key);
	if(foundTexture != sTextureMap.cend())
	{
		if (!foundTexture->second.expired())
		{
			std::shared_ptr<TextureResource> rc = foundTexture->second.lock();

			if (maxSize != nullptr && !maxSize->empty() && Settings::getInstance()->getBool("OptimizeVRAM"))
			{
				auto dt = sTextureDataManager.get(rc.get());
				if (dt != nullptr)
				{
					dt->setMaxSize(*maxSize);

					if (dt->isLoaded() && !dt->isMaxSizeValid())
					{
						dt->releaseVRAM();
						dt->releaseRAM();
						dt->load();
					}
				}
			}

			return rc;
		}
		else
			sTextureMap.erase(foundTexture);
	}

	// need to create it
	std::shared_ptr<TextureResource> tex;
	tex = std::shared_ptr<TextureResource>(new TextureResource(key.first, tile, dynamic, !forceLoad, maxSize));
	std::shared_ptr<TextureData> data = sTextureDataManager.get(tex.get(), !forceLoad);

	if (asReloadable)
	{
		sTextureMap[key] = std::weak_ptr<TextureResource>(tex);

		// Add it to the reloadable list
		rm->addReloadable(tex);
	}

	if (data != nullptr && maxSize != nullptr)
		data->setMaxSize(*maxSize);

	// Force load it if necessary. Note that it may get dumped from VRAM if we run low
	if (forceLoad)
	{
		tex->mForceLoad = forceLoad;

		if (data != nullptr && !data->isLoaded())
			data->load();
	}

	return tex;
}

// For scalable source images in textures we want to set the resolution to rasterize at
void TextureResource::rasterizeAt(size_t width, size_t height)
{
	// Avoids crashing if a theme (in grids) defines negative sizes
	if (width < 0) width = -width;
	if (height < 0) height = -height;

	std::shared_ptr<TextureData> data;
	if (mTextureData != nullptr)
		data = mTextureData;
	else
		data = sTextureDataManager.get(this);

	// mSourceSize = Vector2f((float)width, (float)height);
	data->setSourceSize((float)width, (float)height);
	
	if (mForceLoad || (mTextureData != nullptr))
		if (!data->isLoaded())
			data->load();
}

Vector2f TextureResource::getSourceImageSize() const
{
	return mSourceSize;
}

bool TextureResource::isLoaded() const
{
	if (mTextureData != nullptr)
		return mTextureData->isLoaded();

	auto data = sTextureDataManager.get(this, false); 
	if (data != nullptr)
		return data->isLoaded();

	return true;
}

size_t TextureResource::getTotalMemUsage()
{
	size_t total = 0;
	// Count up all textures that manage their own texture data
	for (auto tex : sAllTextures)
	{
		if (tex->mTextureData != nullptr)
			total += tex->mTextureData->getVRAMUsage();
	}
	// Now get the committed memory from the manager
	total += sTextureDataManager.getCommittedSize();
	// And the size of the loading queue
	total += sTextureDataManager.getQueueSize();
	return total;
}

size_t TextureResource::getTotalTextureSize()
{
	size_t total = 0;
	// Count up all textures that manage their own texture data
	for (auto tex : sAllTextures)
	{
		if (tex->mTextureData != nullptr)
			total += tex->getSize().x() * tex->getSize().y() * 4;
	}
	// Now get the total memory from the manager
	total += sTextureDataManager.getTotalSize();
	return total;
}

bool TextureResource::unload()
{
	// Release the texture's resources
	std::shared_ptr<TextureData> data;
	if (mTextureData == nullptr)
		data = sTextureDataManager.get(this, false);
	else
		data = mTextureData;

	if (data != nullptr && data->isLoaded())
	{
		data->releaseVRAM();
		data->releaseRAM();

		return true;
	}

	return false;
}

void TextureResource::reload()
{
	// For dynamically loaded textures the texture manager will load them on demand.
	// For manually loaded textures we have to reload them here
	if (mTextureData && !mTextureData->isLoaded())
		mTextureData->load();
	else if (mTextureData == nullptr)
		sTextureDataManager.get(this);
}

void TextureResource::clearQueue()
{
	sTextureDataManager.clearQueue();
}