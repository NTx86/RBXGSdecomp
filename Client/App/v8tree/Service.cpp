#include "v8tree/Service.h"

namespace RBX
{
	const char* sServiceProvider = "ServiceProvider";

	Reflection::BoundFuncDesc<ServiceProvider, boost::shared_ptr<Instance>(std::string), 1> ServiceProvider::func_service(&ServiceProvider::findServiceByClassNameString, "service", "name", Reflection::FunctionDescriptor::AnyCaller);
	Reflection::BoundFuncDesc<ServiceProvider, boost::shared_ptr<Instance>(std::string), 1> ServiceProvider::func_GetService(&ServiceProvider::findServiceByClassNameString, "GetService", "name", Reflection::FunctionDescriptor::AnyCaller);

	ServiceProvider::ServiceProvider()
	{
	}

	void ServiceProvider::onChildAdded(Instance* instance)
	{
		RBXASSERT(Instance::fastDynamicCast<ServiceProvider>(instance)==NULL);

		if (Instance::fastDynamicCast<Service>(instance))
		{
			if (!instance->getClassName().empty())
			{
				RBXASSERT(!findServiceByClassName(instance->getClassName()));
				serviceMap[&instance->getClassName()] = shared_from(instance);
			}
			Notifier<ServiceProvider, ServiceAdded>::raise(ServiceAdded(instance));
		}
	}

	void ServiceProvider::clearServices()
	{
		RBXASSERT(numChildren()==0);
		serviceArray.clear();
		serviceMap.clear();
	}

	boost::shared_ptr<Instance> ServiceProvider::createChild(const Name& className)
	{
		RBXASSERT(className!=RBX::Name::getNullName());

		boost::shared_ptr<Instance> instance = findServiceByClassName(className);
		if (instance)
		{
			return instance;
		}

		return AbstractFactoryProduct<Instance>::create(className);
	}
}
