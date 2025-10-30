#pragma once
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>
#include "reflection/type.h"

namespace RBX
{
	namespace Reflection
	{
		class EnumDescriptor : public Type
		{
		public:
			class Item : public Descriptor
			{
			public:
				const int value;
				const size_t index;
			  
			public:
				//Item(const Item&);
				Item(const char* name, int value, size_t index)
					: Descriptor(name),
					  value(value),
					  index(index)
				{
				}
				virtual ~Item() {}
			public:
				//Item& operator=(const Item&);
			};

		protected:
			std::vector<const Item*> allItems;
			size_t enumCount;
			size_t enumCountMSB;
		
		public:
			//EnumDescriptor(const EnumDescriptor&);
		protected:
			EnumDescriptor(const char* typeName, const type_info& type);
			virtual ~EnumDescriptor();
		
		public:
			size_t getEnumCount() const
			{
				return enumCount;
			}
			size_t getEnumCountMSB() const
			{
				return enumCountMSB;
			}
			std::vector<const Item*>::const_iterator begin() const
			{
				return allItems.begin();
			}
			std::vector<const Item*>::const_iterator end() const
			{
				return allItems.end();
			}
			bool isValue(int intValue) const
			{
				return std::find_if(begin(), end(), boost::bind(&equalValue, _1, intValue)) != end();
			}
			//EnumDescriptor& operator=(const EnumDescriptor&);
		  
		public:
			static std::vector<const EnumDescriptor*>::const_iterator enumsBegin()
			{
				return allEnums().begin();
			}
			static std::vector<const EnumDescriptor*>::const_iterator enumsEnd()
			{
				return allEnums().end();
			}
		private:
			static std::vector<const EnumDescriptor*>& allEnums();
			static bool equalValue(const Item* item, int intValue)
			{
				return item->value == intValue;
			}
		};

		template<typename Enum>
		class EnumDesc : public EnumDescriptor
		{
		private:
			std::map<const Name*, Enum> nameToEnum;
			std::map<const Name*, Enum> nameToEnumLegacy;
			std::vector<const Name*> enumToName;
			std::map<std::string, Enum> stringToEnum;
			std::map<std::string, Enum> stringToEnumLegacy;
			std::vector<std::string> enumToString;
			std::vector<Enum> indexToEnum;
			std::vector<size_t> enumToIndex;
		  
		public:
			//EnumDesc(const EnumDesc&);
		private:
			EnumDesc();
		private:
			virtual ~EnumDesc() {}

		private:
			void addPair(Enum value, const char* name) // TODO: not 100% match
			{
				size_t index = enumCount;
				Item* item = new Item(name, value, index);
				allItems.push_back(item);

				RBXASSERT(value>=0);
				RBXASSERT(value<=100);

				if (enumToIndex.size() <= (size_t)value)
					enumToIndex.resize(value + 1, -1);
				enumToIndex[value] = index;

				indexToEnum.push_back(value);

				if (enumToName.size() <= (size_t)value)
					enumToName.resize(value + 1, &Name::getNullName());
				enumToName[value] = &item->name;

				if (enumToString.size() <= (size_t)value)
					enumToString.resize(value + 1, std::string());
				enumToString[value] = name;

				nameToEnum[&item->name] = value;

				stringToEnum[name] = value;

				enumCount++;

				size_t newEnumCountMCB = -1;
				for (size_t i = enumCount; i != 0; ++newEnumCountMCB)
					i >>= 1;
				enumCountMSB = newEnumCountMCB;
			}
			void addLegacyName(const char* name, Enum value)
			{
				addLegacyName(Name::declare(name, -1), value);
			}
			void addLegacyName(const Name& name, Enum value)
			{
				nameToEnumLegacy[&name] = value;
				stringToEnumLegacy[name.name] = value;
			}
		public:
			const Name& convertToName(const Enum&) const;
			std::string convertToString(const Enum& value) const
			{
				if (value < 0)
					return "";
				if ((size_t)value >= enumToString.size())
					return "";
				return enumToString[value];
			}
			bool convertToValue(size_t, Enum&) const;
			bool convertToValue(const std::string& text, Enum& value) const
			{
				std::map<std::string, Enum>::const_iterator iter = stringToEnum.find(text);
				if (iter != stringToEnum.end())
				{
					value = (*iter).second;
					return true;
				}

				iter = stringToEnumLegacy.find(text);
				if (iter != stringToEnumLegacy.end())
				{
					value = (*iter).second;
					return true;
				}

				return false;
			}
			bool convertToValue(const Name& name, Enum& value) const
			{
				std::map<const Name*, Enum>::const_iterator iter = nameToEnum.find(&name);
				if (iter != nameToEnum.end())
				{
					value = (*iter).second;
					return true;
				}

				iter = nameToEnumLegacy.find(&name);
				if (iter != nameToEnumLegacy.end())
				{
					value = (*iter).second;
					return true;
				}

				return false;
			}
			size_t convertToIndex(Enum value) const
			{
				RBXASSERT(value>=0);
				if ((size_t)value < enumToIndex.size())
					return enumToIndex[value];
				return -1;
			}
			//EnumDesc& operator=(const EnumDesc&);
		  
		public:
			static const EnumDesc& singleton()
			{
				static EnumDesc s;
				return s;
			}
		};
	}
}
