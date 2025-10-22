#pragma once
#include "util/Name.h"

namespace RBX {
	class INamed {
	public:
		virtual const Name& getName() const;
		INamed(const Name&);
		INamed();
		//INamed& operator=(const INamed&);
	};
};