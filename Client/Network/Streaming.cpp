#include "Streaming.h"
#include <StringCompressor.h>
#include <algorithm>

namespace RBX
{
	namespace Network
	{
		RakNet::BitStream& operator<<(RakNet::BitStream& stream, bool value)
		{
			if (value)
				stream.Write1();
			else
				stream.Write0();

			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, bool& value)
		{
			stream.Read(value);
			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, unsigned char& value)
		{
			stream.ReadBits(&value, 8);
			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, unsigned int& value)
		{
			stream.ReadBits((unsigned char*)&value, 32);
			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, float& value)
		{
			stream.ReadBits((unsigned char*)&value, 32);
			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, std::string& value)
		{
			unsigned int strSize;
			stream >> strSize;

			char* strBuf = new char[strSize + 1];
			stringCompressor->DecodeString(strBuf, strSize + 1, &stream);
			value = strBuf;

			delete[] strBuf;
			return stream;
		}

		RakNet::BitStream& operator>>(RakNet::BitStream& stream, G3D::Color3& value)
		{
			stream >> value.r;
			stream >> value.g;
			stream >> value.b;
			return stream;
		}

		template<typename T>
		RakNet::BitStream& operator>>(RakNet::BitStream& stream, T& value) // TODO: check match
		{
			return operator>>(stream, value);
		}

		bool brickEq(float a, float b)
		{
			return a == b || fabs(a - b) <= 0.0005f;
		}

		void StringReceiver::receive(RakNet::BitStream& stream, std::string& value)
		{
			unsigned char id;
			stream >> id;

			if (id == 0)
			{
				value.clear();
			}
			else if (id >> 7)
			{
				stream >> value;
				dictionary[id & 127] = value;
			}
			else
			{
				value = dictionary[id];
			}
		}

		void StringReceiver::receive(RakNet::BitStream& stream, const Name*& value)
		{
			std::string s;
			receive(stream, s);

			value = &Name::declare(s.c_str(), -1);
		}

		void IdSerializer::resolvePendingBindings(Instance* instance, Guid::Data id)
		{
			std::map<Guid::Data, std::vector<WaitItem>>::iterator iter = waitItems.find(id);
			if (iter != waitItems.end())
			{
				std::for_each(iter->second.begin(), iter->second.end(), boost::bind(&IdSerializer::setRefValue, _1, instance));
				waitItems.erase(iter);
			}
		}
	}
}
