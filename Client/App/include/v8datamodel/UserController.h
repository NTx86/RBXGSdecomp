#pragma once
#include "v8tree/Service.h"
#include "v8world/Controller.h"
#include "util/UserInputBase.h"

namespace RBX
{
	class PVInstance;

	extern const char* sControllerService;
	class ControllerService : public DescribedCreatable<ControllerService, Instance, &sControllerService>, public Service
	{
	private:
		UserInputBase* hardwareDevice;
		G3D::ReferenceCountedPointer<Controller> playerController;
		G3D::ReferenceCountedPointer<Controller> keyboardPrimaryController;
		G3D::ReferenceCountedPointer<Controller> keyboardSecondaryController;
		G3D::ReferenceCountedPointer<Controller> nullController;

	public:
		//ControllerService(const ControllerService&);
		ControllerService();
	public:
		G3D::ReferenceCountedPointer<Controller> getController(Controller::ControllerType, const PVInstance*);
		const UserInputBase* getHardwareDevice() const;
		UserInputBase* getHardwareDevice();
		void setHardwareDevice(UserInputBase*);
	public:
		virtual ~ControllerService();
	public:
		//ControllerService& operator=(const ControllerService&);
	};
}
