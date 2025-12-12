#pragma once
#include <boost/any.hpp>
#include <typeinfo>
#include <list>
#include "reflection/descriptor.h"

namespace RBX
{
	namespace Reflection
	{
		class Type : public Descriptor
		{
		public:
			const type_info& type;
			const Name& tag;
		  
		public:
			bool operator==(const Type&) const;
			bool operator!=(const Type&) const;

		public:
			//Type(const Type&);
		protected:
			Type(const char* name, const type_info& type, const char* tag) // TODO: check if matches (will only match in a /GS project)
				: Descriptor(name),
				  type(type),
				  tag(Name::declare(tag, -1))
			{
			}
			Type(const char* name, const type_info& type)
				: Descriptor(name),
				  type(type),
				  tag(Name::lookup(name))
			{
			}
		public:
			virtual ~Type() {}

		public:
			template<typename T>
			static const Type& singleton();
		};

		class RefType : public Type
		{
		public:
			//RefType(const RefType&);
		private:
			RefType(const char* name, const type_info& type)
				: Type(name, type, "Ref")
			{
			}
		public:
			virtual ~RefType() {}
		public:
			//RefType& operator=(const RefType&);

		public:
			template<typename T>
			static const RefType& singleton()
			{
				static RefType type("Object", typeid(RefType));
				return type;
			}
		};

		class Value
		{
		private:
			const Type* _type;
			boost::any value;
		  
		public:
			//Value(const Value&);
			// TODO: check ctor
			Value();

			template<typename ValueType>
			Value(typename ValueType& value)
				: _type(&Type::singleton<typename ValueType>()),
				  value(value)
			{
			}
		public:
			Value& operator=(const Value& other)
			{
				this->_type = other._type;
				this->value = other.value;
			}
			const Type& type() const;
			bool isVoid() const;
		public:
			~Value() {}
		public:
			template<typename T>
			T& convert();

			template<typename T>
			void set(const T& value)
			{
				this->_type = &Type::singleton<T>();
				this->value = value;
			}

			// TODO: definitions for all the template functions
		};

		class SignatureDescriptor
		{
		public:
			struct Item
			{
			public:
				const Name* name;
				const Type* type;
				const Value defaultValue;
			  
			public:
				//template<typename ValueType>
				//Item(const char* name)
				//	: name(&Name::declare(name, -1)),
				//	  type(&Type::singleton<typename ValueType>()),
				//	  defaultValue()
				//{
				//}
				// TODO: This constructor must be templated since it does not appear in the PDB/resym
				// However, we can not use the templated constructor...
				// Bruh.
				// btw this doesnt match either lol
				Item(const Name& name, const Type& type)
					: name(&name),
					  type(&type),
					  defaultValue()
				{
				}
				~Item() {}
				//Item(const Item&);
			};

		public:
			const Type* resultType;
			std::list<Item> arguments;
		  
		public:
			void addArgument(const Name& name, const Type& type, const Value& defaultValue);
		public:
			//SignatureDescriptor(const SignatureDescriptor&);
			SignatureDescriptor();
			~SignatureDescriptor();
		public:
			//SignatureDescriptor& operator=(const SignatureDescriptor&);
		};
	}
}
