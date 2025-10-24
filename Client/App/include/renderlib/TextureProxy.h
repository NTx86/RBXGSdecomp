#pragma once

#include "rbxgraphics/TextureProxyBase.h"

namespace RBX
{
	namespace Render
	{
		class TextureProxy : public TextureProxyBase
		{
		protected:
			ReferenceCountedPointer<G3D::Texture> texture;
			bool loaded;
			TextureManager* textureManager;
			bool tiled;
			ReferenceCountedPointer<TextureProxy> root;
			ReferenceCountedPointer<TextureProxy> shadeTexture;
			Color3 shadeColor;
		public:
			TextureProxy(const TextureProxy&);
			TextureProxy(G3D::TextureManager&, const std::string&, bool);
		protected:
			TextureProxy(ReferenceCountedPointer<TextureProxy>);
		public:
			ReferenceCountedPointer<TextureProxy> shade(const Color3&, const ReferenceCountedPointer<TextureProxy>&);
			ReferenceCountedPointer<TextureProxy> alphaOnlyVersion();

			virtual ReferenceCountedPointer<G3D::Texture> getIfResolved() const;
			virtual ReferenceCountedPointer<G3D::Texture> resolve(RenderDevice*);
			virtual Vector2 getSize();
			virtual ~TextureProxy();
			
			TextureProxy& operator=(const TextureProxy&);
			
			static void shadePixel(const Color3uint8&, Color3uint8&);
		};
	}
}