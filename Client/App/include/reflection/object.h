#pragma once
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include "reflection/descriptor.h"
#include "reflection/member.h"
#include "reflection/signal.h"
#include "reflection/property.h"
#include "reflection/function.h"
#include "util/Name.h"

namespace RBX
{
	// TODO: check if matches
	template<typename Class>
	boost::shared_ptr<Class> shared_from(Class* r)
	{
		return boost::shared_static_cast<Class>(r->shared_from_this());
	}

	class Object
	{
	public:
		virtual ~Object() {} //TODO:check
		Object(const Object&);
		Object();
	public:
		//Object& operator=(const Object&);
	};

	class ICreator
	{
	public:
		virtual boost::shared_ptr<Object> create() const;
	public:
		//ICreator(const ICreator&);
		ICreator();
	public:
		//ICreator& operator=(const ICreator&);
	};

	template<typename T>
	class Creatable : public Object
	{
	public:
		//Creatable(const Creatable&);
	protected:
		Creatable();
		virtual ~Creatable() {} // TODO:check
	public:
		//Creatable& operator=(const Creatable&);

	public:
		// TODO: check matches
		template<typename Class>
		static boost::shared_ptr<Class> create()
		{
			return boost::shared_ptr(new Class());
		}
		template<typename Class, typename Parameter1>
		static boost::shared_ptr<Class> create(Parameter1 p1)
		{
			return boost::shared_ptr(new Class(p1));
		}
		template<typename Class, typename Parameter1, typename Parameter2>
		static boost::shared_ptr<Class> create(Parameter1 p1, Parameter2 p2)
		{
			return boost::shared_ptr(new Class(p1, p2));
		}
	private:
		static void* operator new(size_t);
		static void operator delete(void* ptr)
		{
			free(ptr);
		}
	};

	template<typename T>
	class AbstractFactoryProduct : public Creatable<T>
	{
	public:
		//AbstractFactoryProduct(const AbstractFactoryProduct&);
	protected:
		AbstractFactoryProduct();
	public:
		virtual const Name& getClassName() const;
	public:
		virtual ~AbstractFactoryProduct() {} //todo:check
	public:
		//AbstractFactoryProduct& operator=(const AbstractFactoryProduct&);
	  
	protected:
		static std::map<const Name*, const ICreator*>& getCreators();
	public:
		static boost::shared_ptr<T> create(const Name&);
	};

	template<typename Class, typename DerivedClass, const char** ClassName>
	class FactoryProduct : public DerivedClass
	{
	private:
		class Creator : public ICreator
		{
		public:
			virtual boost::shared_ptr<Object> create() const;
			const Name& getClassName() const;
		public:
			//Creator(const Creator&);
			Creator();
			~Creator();
		public:
			//Creator& operator=(const Creator&);
		};

	private:
		static const Creator creator;
	  
	public:
		//FactoryProduct(const FactoryProduct&);
	protected:
		FactoryProduct();
		virtual ~FactoryProduct();
	public:
		const ICreator& getCreator();
		virtual const Name& getClassName() const;
		//FactoryProduct& operator=(const FactoryProduct&);
	  
	public:
		static const Name& className();
	};

	template<typename DerivedClass, const char** ClassName>
	class NonFactoryProduct : public DerivedClass
	{
	public:
		//NonFactoryProduct(const NonFactoryProduct&);
		NonFactoryProduct()
			: DerivedClass()
		{
		}
	public:
		virtual const Name& getClassName() const;
	public:
		virtual ~NonFactoryProduct();
	public:
		//NonFactoryProduct& operator=(const NonFactoryProduct&);

	public:
		static const Name& className();
	};

	namespace Reflection
	{
		class ClassDescriptor : public Descriptor, public MemberDescriptorContainer<PropertyDescriptor>, public MemberDescriptorContainer<SignalDescriptor>, public MemberDescriptorContainer<FunctionDescriptor>
		{
		public:
			typedef MemberDescriptorContainer<PropertyDescriptor> PropertyContainer;
			typedef MemberDescriptorContainer<SignalDescriptor> SignalContainer;
			typedef MemberDescriptorContainer<FunctionDescriptor> FunctionContainer;

		private:
			std::vector<ClassDescriptor*> derivedClasses;
			ClassDescriptor* base;
		public:
			static bool lockedDown;
		  
		public:
			//ClassDescriptor(const ClassDescriptor&);
			ClassDescriptor(ClassDescriptor& base, const char* name);
		private:
			ClassDescriptor();
		public:
			const ClassDescriptor* getBase() const
			{
				return base;
			}
			bool isBaseOf(const char*) const;
			bool isBaseOf(const ClassDescriptor& base) const;
			bool isA(const char*) const;
			bool isA(const ClassDescriptor&) const;
			std::vector<ClassDescriptor*>::const_iterator derivedClasses_begin() const;
			std::vector<ClassDescriptor*>::const_iterator derivedClasses_end() const;
			bool operator==(const ClassDescriptor& other) const;
			bool operator!=(const ClassDescriptor& other) const;
		public:
			virtual ~ClassDescriptor();
		public:
			//ClassDescriptor& operator=(const ClassDescriptor&);
		  
		public:
			static ClassDescriptor& rootDescriptor() // TODO: check
			{
				static ClassDescriptor root;
				return root;
			}
		};

		class DescribedBase : public SignalSource
		{
		public:
			typedef MemberDescriptorContainer<PropertyDescriptor> MDCProperty;
			typedef MemberDescriptorContainer<FunctionDescriptor> MDCFunction;
			typedef MemberDescriptorContainer<SignalDescriptor> MDCSignal;

		protected:
			const ClassDescriptor* descriptor;
		  
		public:
			//DescribedBase(const DescribedBase&);
			DescribedBase();
		public:
			const ClassDescriptor& getDescriptor() const
			{
				return *descriptor;
			}
			MDCProperty::ConstIterator findProperty(const Name&) const;
			MDCProperty::Iterator findProperty(const Name&);
			MDCProperty::Iterator properties_begin();
			MDCProperty::ConstIterator properties_begin() const;
			MDCProperty::Iterator properties_end();
			MDCProperty::ConstIterator properties_end() const;
			MDCFunction::ConstIterator findFunction(const Name&) const;
			MDCFunction::ConstIterator functions_begin() const;
			MDCFunction::ConstIterator functions_end() const;
			MDCSignal::ConstIterator findSignal(const Name&) const;
			MDCSignal::Iterator findSignal(const Name&);
			MDCSignal::Iterator signals_begin();
			MDCSignal::ConstIterator signals_begin() const;
			MDCSignal::Iterator signals_end();
			MDCSignal::ConstIterator signals_end() const;
		public:
			virtual ~DescribedBase();
		public:
			//DescribedBase& operator=(const DescribedBase&);
		  
		public:
			static ClassDescriptor& classDescriptor()
			{
				return ClassDescriptor::rootDescriptor();
			}
		};
	}
}
