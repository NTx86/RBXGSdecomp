#include "renderlib/TextureProxy.h"

namespace RBX
{
	namespace Render 
	{
		TextureProxy::TextureProxy(G3D::TextureManager& textureManager, const std::string& filename, bool tiled)
			: texture(NULL),
			  loaded(false),
			  textureManager(&textureManager),
			  tiled(tiled),
			  root(NULL),
			  shadeTexture(NULL)
		{
			//Can't be done in intiallizer list
			this->filename = filename;
		}

		TextureProxy::~TextureProxy()
		{
		}


		TextureProxy::TextureProxy(ReferenceCountedPointer<TextureProxy> root)
			: texture(NULL),
			  loaded(false),
			  textureManager(NULL),
			  root(root),
			  shadeTexture(NULL)
		{
			this->filename = "";
		}
		
		ReferenceCountedPointer<G3D::Texture> TextureProxy::getIfResolved() const
		{
			return this->texture;
		}


		ReferenceCountedPointer<TextureProxy> TextureProxy::alphaOnlyVersion()
		{
			TextureProxy* textureProxy;

			if (this == NULL)
			{
				textureProxy = NULL;
			} else
			{
				textureProxy = new TextureProxy(this);
			}

			return textureProxy;
		}

	}
}