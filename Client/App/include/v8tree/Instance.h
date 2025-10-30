#pragma once
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "reflection/reflection.h"
#include "reflection/property.h"
#include "util/Debug.h"
#include "util/Events.h"
#include "util/Association.h"
#include "util/Utilities.h"
#include "util/Guid.h"

namespace RBX
{
	class Instance;
	class ServiceProvider;
	class RaiseDescendentAdded2;

	// EVENTS
	// TODO: are these meant to be here?
	struct ChildAdded
	{
	public:
		const boost::shared_ptr<Instance> child;
  
	public:
		//ChildAdded(const ChildAdded&);
		ChildAdded(Instance* child)
			: child(shared_from(child))
		{
		}
	private:
		ChildAdded& operator=(const ChildAdded&);
	public:
		~ChildAdded();
	};

	struct ChildRemoved
	{
	public:
		const boost::shared_ptr<Instance> child;
  
	public:
		//ChildRemoved(const ChildRemoved&);
		ChildRemoved(Instance*);
	private:
		ChildRemoved& operator=(const ChildRemoved&);
	public:
		~ChildRemoved();
	};

	struct DescendentAdded
	{
		friend class Instance;
		friend class RaiseDescendentAdded2;

	public:
		const boost::shared_ptr<Instance> instance;
		const boost::shared_ptr<Instance> parent;
	  
	public:
		//DescendentAdded(const DescendentAdded&);
	private:
		DescendentAdded(Instance* instance, Instance* parent)
			: instance(shared_from(instance)),
			  parent(shared_from(parent))
		{
		}
		DescendentAdded(boost::shared_ptr<Instance>, boost::shared_ptr<Instance>);
	public:
		~DescendentAdded() {}
	};

	struct DescendentRemoving
	{
	public:
		const boost::shared_ptr<Instance> instance;
		const boost::shared_ptr<Instance> parent;
	  
	public:
		//DescendentRemoving(const DescendentRemoving&);
		DescendentRemoving(const boost::shared_ptr<Instance>& instance, const boost::shared_ptr<Instance>& parent)
			: instance(instance),
			  parent(parent)
		{
		}
	public:
		~DescendentRemoving();
	};

	struct AncestorChanged
	{
	public:
		Instance* child;
		Instance* oldParent;
		Instance* newParent;
	  
	public:
		AncestorChanged(Instance*, Instance*, Instance*);
	};

	class PropertyChanged
	{
		friend class Instance;

	private:
		Reflection::Property property;
	  
	public:
		bool getProperty(Reflection::Property*&);
		const Reflection::Property& getProperty() const
		{
			return property;
		}
		const Reflection::PropertyDescriptor& getDescriptor()
		{
			return property.getDescriptor();
		}
		const Name& getName();
	private:
		PropertyChanged(const Reflection::Property& property)
			: property(Reflection::Property(property.getDescriptor(), property.getInstance())) //property(property)
		{
			//RBXASSERT(property.getDescriptor().isMemberOf(property.getInstance()));
		}
	public:
		//PropertyChanged(const PropertyChanged&);
	public:
		//PropertyChanged& operator=(const PropertyChanged&);
	};

	extern const char* sInstance;
	class Instance : public boost::enable_shared_from_this<Instance>,
					 public AbstractFactoryProduct<Instance>,
					 public Reflection::Described<Instance, &sInstance, Reflection::DescribedBase>,
					 public Debugable,
					 public Notifier<Instance, ChildAdded>,
					 public Notifier<Instance, ChildRemoved>,
					 public Notifier<Instance, DescendentAdded>,
					 public Notifier<Instance, DescendentRemoving>,
					 public Notifier<Instance, AncestorChanged>,
					 public Notifier<Instance, PropertyChanged>,
					 public boost::noncopyable
	{
		friend class RaiseDescendentAdded2;

	private:
		Association<Instance> assoc;
		Instance* parent;
		CopyOnWrite<std::vector<boost::shared_ptr<Instance>>> children;
		std::string name;
		bool archivable;
		Guid guid;

	public:
		static Reflection::BoundProp<bool, 1> propArchivable;
		static const Reflection::PropDescriptor<Instance, std::string> desc_Name;
		static const Reflection::RefPropDescriptor<Instance, Instance> propParent;
		static Reflection::SignalDesc<Instance, void(boost::shared_ptr<Instance>)> event_childAdded;
		static Reflection::SignalDesc<Instance, void(boost::shared_ptr<Instance>)> event_childRemoved;
		static Reflection::SignalDesc<Instance, void(boost::shared_ptr<Instance>)> event_descendentAdded;
		static Reflection::SignalDesc<Instance, void(boost::shared_ptr<Instance>)> event_descendentRemoving;
		static Reflection::SignalDesc<Instance, void(boost::shared_ptr<Instance>)> event_ancestryChanged;
		static Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> event_propertyChanged;
	  
	private:
		void predelete();
	public:
		//Instance(const Instance&);
	protected:
		Instance(const char*);
		Instance();
		virtual ~Instance();
	public:
		void assignGuid(const Guid::Data&);
		const Guid& getGuid() const;
	protected:
		virtual void onGuidChanged();
	public:
		void remove();
		const Association<Instance>& association() const;
		Association<Instance>& association();
		boost::shared_ptr<Instance> clone();
		void removeAllChildren();
		std::string getClassNameStr() const
		{
			return getClassName().toString();
		}
		void setParent(Instance*);
		void setParent2(boost::shared_ptr<Instance>);
		void promoteChildren();
		const std::string& getName() const
		{
			return name;
		}
		virtual void setName(const std::string&);
		bool isAncestorOf(const Instance*) const;
		bool isAncestorOf2(boost::shared_ptr<Instance>);
		bool isDescendentOf2(boost::shared_ptr<Instance>);
		bool isDescendentOf(const Instance*) const;
		size_t numChildren() const;
		int findChildIndex(const Instance*) const;
		const Instance* getChild(size_t) const;
		Instance* getChild(size_t);
		Instance* findFirstChildByName(const std::string&) const;
		Instance* findFirstChildByNameRecursive(const std::string&) const;
		boost::shared_ptr<Instance> findFirstChildByName2(std::string, bool);
		const CopyOnWrite<std::vector<boost::shared_ptr<Instance>>>& getChildren() const;
		boost::shared_ptr<const std::vector<boost::shared_ptr<Instance>>> getChildren2()
		{
			return children.read();
		}
		bool canAddChild(const boost::shared_ptr<Instance>&) const;
		bool canAddChild(const Instance*) const;
		bool canSetParent(const Instance*) const;
		Instance* getParent() const
		{
			return parent;
		}
		const Instance* getRootAncestor() const;
		Instance* getRootAncestor();
		bool contains(const Instance*) const;
	protected:
		virtual bool askAddChild(const Instance*) const;
		virtual bool askSetParent(const Instance*) const;
		virtual void onAddListener(Listener<Instance, DescendentAdded>*) const;
		virtual void onAddListener(Listener<Instance, ChildAdded>*) const;
		virtual void onAncestorChanged(const AncestorChanged&);
		virtual void onDescendentAdded(Instance*);
		virtual void onDescendentRemoving(const boost::shared_ptr<Instance>&);
		virtual void onChildAdded(Instance*);
		virtual void onChildRemoving(Instance*);
		virtual void onChildRemoved(Instance*);
	private:
		virtual void onLastChildRemoved();
		void writeProperties(XmlElement*) const;
	protected:
		virtual void readProperty(const XmlElement*, IReferenceBinder&);
	public:
		virtual void onServiceProvider(const ServiceProvider*, const ServiceProvider*);
		void readProperties(const XmlElement*, IReferenceBinder&);
		virtual boost::shared_ptr<Instance> createChild(const Name&);
		virtual XmlElement* write();
		void writeChildren(XmlElement*);
		XmlElement* writeDelete();
		void read(const XmlElement*, IReferenceBinder&);
		void readChildren(const XmlElement*, IReferenceBinder&);
		void readChild(const XmlElement*, IReferenceBinder&);
		void raisePropertyChanged(const Reflection::PropertyDescriptor&);
	protected:
		void raiseChanged(const Reflection::PropertyDescriptor&);
		virtual void onChildChanged(Instance*, const PropertyChanged&);
	public:
		//Instance& operator=(const Instance&);
	public:
		template<typename Function>
		void for_eachChild(Function func) const
		{
			if (&(*children))
			{
				boost::shared_ptr<const std::vector<boost::shared_ptr<Instance>>> c = children.read();
				for (std::vector<boost::shared_ptr<Instance>>::const_iterator iter = c->begin(); iter != c->end(); iter++)
				{
					func((*iter).get());
				}
			}
		}
	  
	private:
		static void predelete(Instance*);
	public:
		static XmlElement* toNewXmlRoot(Instance*);
		static boost::shared_ptr<Instance> fromXmlRoot(XmlElement*);
		static Instance* getRootAncestor(Instance*);
		static const Instance* getRootAncestor(const Instance*);
	private:
		static void signalDescendentAdded(Instance*, Instance*, Instance*);
		static void signalDescendentRemoving(const boost::shared_ptr<Instance>&, Instance*, Instance*);

	public:
		// NOTE: This is entirely inlined. See assertions in later client builds.
		template<typename To>
		static To* fastDynamicCast(Instance* instance)
		{
			return dynamic_cast<To*>(instance);
		}
	};
}
