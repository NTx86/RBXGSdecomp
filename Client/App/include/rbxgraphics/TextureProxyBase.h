#pragma once

#include <G3DAll.h>

namespace RBX {
	class TextureProxyBase : public ReferenceCountedObject
	{
	public:
		std::string filename;

		TextureProxyBase(const TextureProxyBase&);
		TextureProxyBase();

		virtual ~TextureProxyBase()
		{
		}

		virtual Vector2 getSize();
		TextureProxyBase& operator=(const TextureProxyBase&);
	};
};