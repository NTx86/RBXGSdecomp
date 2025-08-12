#include "util/Http.h"

namespace RBX
{
	Http::Http(const std::string* url) 
		:url(*url),
		additionalHeaders()
	{
	}
}