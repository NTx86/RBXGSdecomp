#pragma once
#include "v8xml/XmlSerializer.h"
#include "reflection/reflection.h"

namespace RBX
{
	class DataModel;

	// NOTE: may not be intended for this file
	class IReferenceBinder
	{
	public:
		virtual void announceID(const XmlNameValuePair*, Instance*);
		virtual void announceIDREF(const XmlNameValuePair*, Reflection::DescribedBase*, const IIDREF*);
	protected:
		void assign(const IIDREF*, Reflection::DescribedBase*, const InstanceHandle&);
	public:
		//IReferenceBinder(const IReferenceBinder&);
		IReferenceBinder();
	public:
		//IReferenceBinder& operator=(const IReferenceBinder&);
	};

	class MergeBinder : public IReferenceBinder
	{
	private:
		struct IDREFItem
		{
		public:
			const IIDREF* idref;
			Reflection::DescribedBase* propertyOwner;
			InstanceHandle value;
		  
		public:
			//IDREFItem(const IDREFItem&);
			IDREFItem();
			~IDREFItem();
		public:
			//IDREFItem& operator=(const IDREFItem&);
		};

	private:
		std::vector<IDREFItem> deferredIDREFItems;

	public:
		virtual void announceID(const XmlNameValuePair*, Instance*);
		virtual void announceIDREF(const XmlNameValuePair*, Reflection::DescribedBase*, const IIDREF*);
		virtual bool resolveRefs();
	protected:
		virtual bool processID(const XmlNameValuePair*, Instance*);
		virtual bool processIDREF(const XmlNameValuePair*, Reflection::DescribedBase*, const IIDREF*);
	public:
		//MergeBinder(const MergeBinder&);
		MergeBinder();
		~MergeBinder();
	public:
		//MergeBinder& operator=(const MergeBinder&);
	};
}

class ArchiveBinder : public RBX::MergeBinder
{
private:
	struct IDREFBinding
	{
	public:
		const XmlNameValuePair* valueIDREF;
		RBX::Reflection::DescribedBase* propertyOwner;
		const RBX::IIDREF* idref;
	};

private:
	std::map<std::string, RBX::InstanceHandle> idMap;
	std::list<IDREFBinding> idrefBindings;

public:
	virtual bool processID(const XmlNameValuePair*, RBX::Instance*);
	virtual bool processIDREF(const XmlNameValuePair*, RBX::Reflection::DescribedBase*, const RBX::IIDREF*);
	bool resolveIDREF(IDREFBinding);
	virtual bool resolveRefs();
public:
	//ArchiveBinder(const ArchiveBinder&);
	ArchiveBinder();
	~ArchiveBinder();
public:
	ArchiveBinder& operator=(const ArchiveBinder&);
};

class SerializerV2
{
protected:
	int schemaVersionLoading;
public:
	static const int CURRENT_SCHEMA_VERSION;
  
public:
	void loadInstances(XmlElement*, std::vector<boost::shared_ptr<RBX::Instance>>&);
	void load(std::istream&, RBX::DataModel*);
	void loadXML(std::istream&, RBX::DataModel*);
	void merge(const XmlElement*, RBX::DataModel*);
  
public:
	static XmlElement* newRootElement();
	static void isolateHandles(XmlElement*);
	static void load(XmlElement*, RBX::DataModel*);
};
