#pragma once
#include "reflection/member.h"
#include "v8xml/XmlElement.h"
#include "v8xml/SerializerV2.h"
#include "util/Handle.h"

namespace RBX
{
	class Instance;

	// NOTE: may not be intended for this file (could suit object.h or reflection.h better)
	class IIDREF
	{
	private:
		virtual void assignIDREF(Reflection::DescribedBase*, const InstanceHandle&) const;
	public:
		//IIDREF(const IIDREF&);
		IIDREF()
		{
		}
	public:
		//IIDREF& operator=(const IIDREF&);
	};

	// NOTE: may not be intended for this file
	class IReferenceBinder
	{
	public:
		//virtual void announceID(const XmlNameValuePair*, Instance*); // TODO: uncomment once XmlNameValuePair is added
		//virtual void announceIDREF(const XmlNameValuePair*, Reflection::DescribedBase*, const IIDREF*); // TODO: uncomment once IIDREF and XmlNameValuePair are added
	protected:
		void assign(const IIDREF*, Reflection::DescribedBase*, const InstanceHandle&);
	public:
		//IReferenceBinder(const IReferenceBinder&);
		IReferenceBinder();
	public:
		//IReferenceBinder& operator=(const IReferenceBinder&);
	};

	namespace Reflection
	{
		class Property;
		class PropertyDescriptor : public MemberDescriptor
		{
		public:
			enum Functionality : size_t
			{
				STANDARD = 5,
				UI = 1,
				STREAMING = 4,
				LEGACY = 0
			};
		public:
			typedef Property Describing;

		private:
			size_t bIsPublic : 1;
			size_t bCanStreamWrite : 1;
		public:
			const Type& type;
		  
		public:
			//PropertyDescriptor(const PropertyDescriptor&);
		protected:
			PropertyDescriptor(ClassDescriptor& classDescriptor, const Type& type, const char* name, const char* category, Functionality flags);
		public:
			bool isPublic() const;
			virtual bool isReadOnly() const;
			bool canStreamWrite() const;
			bool operator==(const PropertyDescriptor&) const;
			virtual bool equalValues(const DescribedBase*, const DescribedBase*) const;
			virtual bool hasStringValue() const;
			virtual std::string getStringValue(const DescribedBase*) const;
			virtual bool setStringValue(DescribedBase*, const std::string&) const;
			XmlElement* write(const DescribedBase*, bool) const;
			virtual void read(DescribedBase*, const XmlElement*, IReferenceBinder&) const;
		private:
			virtual void writeValue(const DescribedBase*, XmlElement*) const;
			virtual void readValue(DescribedBase*, const XmlElement*, IReferenceBinder&) const;
		public:
			virtual ~PropertyDescriptor();
		public:
			//PropertyDescriptor& operator=(const PropertyDescriptor&);
		};

		class ConstProperty
		{
		protected:
			const PropertyDescriptor* descriptor;
			const DescribedBase* instance;
		  
		public:
			//ConstProperty(const ConstProperty&);
			ConstProperty(const PropertyDescriptor& descriptor, const DescribedBase* instance)
				: descriptor(&descriptor),
				  instance(instance)
			{
				RBXASSERT(descriptor.isMemberOf(instance));
			}
		public:
			const DescribedBase* getInstance() const
			{
				return instance;
			}
			const PropertyDescriptor& getDescriptor() const
			{
				return *descriptor; // might be wrong?
			}
			//ConstProperty& operator=(const ConstProperty&); // maybe?
			const Name& getName() const;
			bool hasStringValue() const;
			std::string getStringValue() const;
			XmlElement* write() const;
		};

		class Property : public ConstProperty
		{
		public:
			//Property(const Property&);
			Property(const PropertyDescriptor& descriptor, DescribedBase* instance)
				: ConstProperty(descriptor, instance)
			{
			}
		public:
			//Property& operator=(const Property&); // maybe?
			bool operator==(const Property&) const;
			bool operator!=(const Property&) const;
			DescribedBase* getInstance() const
			{
				return const_cast<DescribedBase*>(instance); // might not be right
			}
			bool setStringValue(const std::string&);
			void read(const XmlElement*, IReferenceBinder&);
		};

		template<typename PropType>
		class TypedPropertyDescriptor : public PropertyDescriptor
		{
		public:
			class GetSet
			{
			public:
				virtual bool isReadOnly() const;
				virtual PropType getValue(const DescribedBase*) const;
				virtual void setValue(DescribedBase*, const PropType&) const;
			public:
				//GetSet(const GetSet&);
				GetSet()
				{
				}
			public:
				//GetSet& operator=(const GetSet&);
			};

		protected:
			std::auto_ptr<GetSet> getset;
		  
		public:
			//TypedPropertyDescriptor(TypedPropertyDescriptor&);
		protected:
			TypedPropertyDescriptor(ClassDescriptor& classDescriptor, const char* name, const char* category, std::auto_ptr<GetSet> getset, Functionality flags)
				: PropertyDescriptor(classDescriptor, Type::singleton<PropType>(), name, category, flags),
				  getset(getset)
			{
			}
			TypedPropertyDescriptor(ClassDescriptor&, const Type&, const char*, const char*, std::auto_ptr<GetSet>, Functionality);
		public:
			virtual bool isReadOnly() const;
			PropType getValue(const DescribedBase*) const;
			void setValue(DescribedBase*, const PropType&) const;
			virtual bool equalValues(const DescribedBase*, const DescribedBase*) const;
			virtual bool hasStringValue() const;
			virtual std::string getStringValue(const DescribedBase*) const;
			virtual bool setStringValue(DescribedBase*, const std::string&) const;
		private:
			virtual void readValue(DescribedBase*, const XmlElement*, IReferenceBinder&) const;
			virtual void writeValue(const DescribedBase*, XmlElement*) const;
		public:
			virtual ~TypedPropertyDescriptor();
		public:
			//TypedPropertyDescriptor& operator=(TypedPropertyDescriptor&);
		};

		class RefPropertyDescriptor : public PropertyDescriptor
		{
		public:
			virtual DescribedBase* getRefValue(const DescribedBase*) const;
			virtual void setRefValue(DescribedBase*, DescribedBase*) const;
		public:
			//RefPropertyDescriptor(const RefPropertyDescriptor&);
		protected:
			RefPropertyDescriptor(ClassDescriptor& classDescriptor, const Type& type, const char* name, const char* category, Functionality flags)
				: PropertyDescriptor(classDescriptor, type, name, category, flags)
			{
			}
		protected:
			virtual bool hasStringValue() const;
			virtual std::string getStringValue(const DescribedBase*) const;
			virtual bool setStringValue(DescribedBase*, const std::string&) const;
		public:
			virtual ~RefPropertyDescriptor();
		public:
			//RefPropertyDescriptor& operator=(const RefPropertyDescriptor&);
		};
	}
}
