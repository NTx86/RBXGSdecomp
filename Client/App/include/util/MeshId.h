#pragma once
#include "util/ContentProvider.h"

namespace RBX
{
	class MeshId : public ContentId
	{
	public:
		//MeshId(const MeshId&);
		MeshId()
			: ContentId()
		{
		}
		MeshId(const std::string& id)
			: ContentId(id)
		{
		}
		MeshId(const char* id)
			: ContentId(id)
		{
		}
		MeshId(const ContentId& id)
			: ContentId(id)
		{
		}
		~MeshId()
		{
		}
	public:
		//MeshId& operator=(const MeshId&);
	};
}
