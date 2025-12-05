#include "util/Utilities.h"
#include "util/ContentProvider.h"
#include "v8datamodel/BrickColor.h"
#include "G3D/Color3.h"
#include "G3D/Vector3.h"
#include <stdio.h>
#include <boost/lexical_cast.hpp>

namespace RBX
{
	// TODO: order
	template<>
	class StringConverter<bool>
	{
	public:
		static std::string convertToString(const bool& value)
		{
			return value ? "true" : "false";
		}

		static bool convertToValue(const std::string& text, bool& value)
		{
			if (text == "true" || text == "True" || text == "TRUE")
			{
				value = true;
				return true;
			}
			else if (text == "false" || text == "False" || text == "FALSE")
			{
				value = false;
				return true;
			}

			return false;
		}
	};

	template<>
	class StringConverter<int>
	{
	public:
		static std::string convertToString(const int& value)
		{
			char buffer[16];
			_snprintf(buffer, sizeof(buffer), "%d", value);
			return buffer;
		}

		static bool convertToValue(const std::string& text, int& value)
		{
			if (text.size() == 0)
				return false;

			try
			{
				value = boost::lexical_cast<int>(text);
				return true;
			}
			catch (boost::bad_lexical_cast&)
			{
				return false;
			}
		}
	};

	template<>
	class StringConverter<unsigned>
	{
	public:
		static std::string convertToString(const unsigned& value)
		{
			char buffer[16];
			_snprintf(buffer, sizeof(buffer), "%u", value);
			return buffer;
		}

		static bool convertToValue(const std::string& text, unsigned& value)
		{
			if (text.size() == 0)
				return false;

			try
			{
				value = boost::lexical_cast<unsigned>(text);
				return true;
			}
			catch (boost::bad_lexical_cast&)
			{
				return false;
			}
		}
	};

	template<>
	class StringConverter<double>
	{
	public:
		static std::string convertToString(const double& value)
		{
			if (value == std::numeric_limits<double>::infinity())
			{
				return "INF";
			}
			else if (value == -std::numeric_limits<double>::infinity())
			{
				return "-INF";
			}
			else if (!(value < 0.0 || value >= 0.0))
			{
				return "NAN";
			}
			else
			{
				char buffer[32];
				_snprintf(buffer, 32, "%.9g", value);
				return buffer;
			}
		}

		static bool convertToValue(const std::string& text, double& value)
		{
			if (text.size() == 0)
				return false;

			if (text == "INF")
			{
				value = std::numeric_limits<double>::infinity();
				return true;
			}
			else if (text == "-INF")
			{
				value = -std::numeric_limits<double>::infinity();
				return true;
			}
			else if (text == "NAN")
			{
				value = std::numeric_limits<double>::quiet_NaN();
				return true;
			}
			else
			{
				try
				{
					value = boost::lexical_cast<double>(text);
					return true;
				}
				catch (boost::bad_lexical_cast&)
				{
					return false;
				}
			}
		}
	};

	template<>
	class StringConverter<float>
	{
	public:
		static std::string convertToString(const float& value)
		{
			if (value == std::numeric_limits<float>::infinity())
			{
				return "INF";
			}
			else if (value == -std::numeric_limits<float>::infinity())
			{
				return "-INF";
			}
			else if (!(value < 0.0f || value >= 0.0f))
			{
				return "NAN";
			}
			else
			{
				char buffer[32];
				_snprintf(buffer, 32, "%.9g", value);
				return buffer;
			}
		}

		static bool convertToValue(const std::string& text, float& value)
		{
			if (text.size() == 0)
				return false;

			if (text == "INF")
			{
				value = std::numeric_limits<float>::infinity();
				return true;
			}
			else if (text == "-INF")
			{
				value = -std::numeric_limits<float>::infinity();
				return true;
			}
			else if (text == "NAN")
			{
				value = std::numeric_limits<float>::quiet_NaN();
				return true;
			}
			else
			{
				try
				{
					value = boost::lexical_cast<float>(text);
					return true;
				}
				catch (boost::bad_lexical_cast&)
				{
					return false;
				}
			}
		}
	};

	template<>
	class StringConverter<G3D::Color3>
	{
	public:
		static std::string convertToString(const G3D::Color3& value)
		{
			char buffer[64];
			_snprintf(buffer, sizeof(buffer), "%g, %g, %g", value.r, value.g, value.b);
			return buffer;
		}

		static bool convertToValue(const std::string& text, G3D::Color3& value)
		{
			if (text[0] == '#')
			{
				if (text.size() != 7)
					return false;

				value.r = strtol(text.substr(1, 2).c_str(), 0, 16) * (1.0f/255.0f);
				value.g = strtol(text.substr(3, 2).c_str(), 0, 16) * (1.0f/255.0f);
				value.b = strtol(text.substr(5, 2).c_str(), 0, 16) * (1.0f/255.0f);
				return true;
			}
			else if (text.substr(0, 2) == "0x")
			{
				if (text.size() != 8)
					return false;

				value.r = strtol(text.substr(2, 2).c_str(), 0, 16) * (1.0f/255.0f);
				value.g = strtol(text.substr(4, 2).c_str(), 0, 16) * (1.0f/255.0f);
				value.b = strtol(text.substr(6, 2).c_str(), 0, 16) * (1.0f/255.0f);
				return true;
			}
			else
			{
				int index1 = (int)text.find_first_of(",;", 0, 2);
				if (index1 < 0)
					return false;

				value.r = atof(text.substr(0, index1).c_str());

				int startIndex2 = index1 + 1;
				int index2 = (int)text.find_first_of(",;", startIndex2, 2);
				if (index2 < startIndex2)
					return false;

				value.g = atof(text.substr(startIndex2, index2 - startIndex2).c_str());

				int startIndex3 = index2 + 1;
				int index3 = (int)text.size();
				if (index3 < startIndex3)
					return false;

				value.b = atof(text.substr(startIndex3, index3 - startIndex3).c_str());

				return true;
			}
		}
	};

	template<>
	class StringConverter<G3D::Vector3>
	{
	public:
		static std::string convertToString(const G3D::Vector3& value)
		{
			std::string text = StringConverter<float>::convertToString(value.x);
			text += ", ";
			text = StringConverter<float>::convertToString(value.y);
			text += ", ";
			text = StringConverter<float>::convertToString(value.z);
			return text;
		}

		static bool convertToValue(const std::string& text, G3D::Vector3& value)
		{
			int index1 = (int)text.find_first_of(",;", 0, 2);
			if (index1 < 0)
				return false;

			value.x = atof(text.substr(0, index1).c_str());

			int startIndex2 = index1 + 1;
			int index2 = (int)text.find_first_of(",;", startIndex2, 2);
			if (index2 < startIndex2)
				return false;

			value.y = atof(text.substr(startIndex2, index2 - startIndex2).c_str());

			int startIndex3 = index2 + 1;
			int index3 = (int)text.size();
			if (index3 < startIndex3)
				return false;

			value.z = atof(text.substr(startIndex3, index3 - startIndex3).c_str());

			return true;
		}
	};

	template<>
	class StringConverter<BrickColor>
	{
	public:
		static std::string convertToString(const BrickColor& value)
		{
			return value.name();
		}

		// NOTE: inlined
		static bool convertToValue(const std::string& text, BrickColor& value);
	};

	// TODO: Once RBX::MeshId, RBX::TextureId, or RBX::Soundscape::SoundId are added,
	// copy the StringConverter definition below and replace ContentId with the respective class name
	template<>
	class StringConverter<ContentId>
	{
	public:
		static std::string convertToString(const ContentId& value)
		{
			return value.toString();
		}

		// TODO: does not match
		static bool convertToValue(const std::string& text, ContentId& value)
		{
			value = ContentId(text);
			return true;
		}
	};
}
