#pragma once
#include <g3d/vector3.h>
#include <g3d/coordinateframe.h>
#include <g3d/ray.h>
#include "RbxGraphics/Adorn.h"
#include "v8tree/Instance.h"
#include "v8world/Controller.h"
#include "v8datamodel/IPrimaryPart.h"
#include "util/IControllable.h"
#include "util/ILocation.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"

namespace RBX
{
	class Primitive;

	extern const char* sPVInstance;
	class PVInstance : public Reflection::Described<PVInstance, &sPVInstance, Instance>,
					   public IControllable,
					   public virtual IPrimaryPart,
					   public virtual ILocation
	{
	protected:
		ComputeProp<bool, PVInstance> IsControllable;
		ComputeProp<bool, PVInstance> IsTopFlag;
		ComputeProp<G3D::ReferenceCountedPointer<Controller>, PVInstance> TopPVController;
	private:
		bool canSelect;
		Controller::ControllerType controllerType;
		bool showControllerFlag;
	protected:
		boost::scoped_ptr<G3D::CoordinateFrame> legacyOffset;
	public:
		static const Reflection::EnumPropDescriptor<PVInstance, Controller::ControllerType> prop_ControllerType;
	  
	private:
		bool computeIsControllable() const;
		bool computeIsTopFlag() const;
		G3D::ReferenceCountedPointer<Controller> computeTopPVController() const;
		void dirtyAll();
	public:
		//PVInstance(const PVInstance&);
	protected:
		PVInstance(const char* name);
	public:
		virtual ~PVInstance();
	public:
		Controller::ControllerType getControllerType() const;
		void setControllerType(Controller::ControllerType);
		bool getShowControllerFlag() const;
		void setShowControllerFlag(bool);
	protected:
		virtual size_t topHashCode() const;
		virtual size_t childHashCode() const;
		virtual void onChildAdded(Instance*);
		virtual void onChildRemoving(Instance*);
		virtual void onDescendentAdded(Instance*);
		virtual void onDescendentRemoving(const boost::shared_ptr<Instance>&);
		virtual void readProperty(const XmlElement*, IReferenceBinder&);
	public:
		virtual bool isControllable() const;
	protected:
		void renderCoordinateFrame(Adorn*);
		void onControllerChanged();
		virtual void onChildControllerChanged();
		virtual void onParentControllerChanged();
		virtual void onExtentsChanged() const;
	public:
		void moveToPoint(G3D::Vector3);
		Controller* getTopPVController() const;
		bool isChaseable() const;
		virtual Extents getExtentsWorld() const;
		virtual Extents getExtentsLocal() const;
		virtual const Primitive* getBiggestPrimitive() const;
		virtual bool hitTest(const G3D::Ray&, G3D::Vector3&);
		void writeCoordinateFrameData(XmlState*);
		void writeVelocityData(XmlState*);
		const PVInstance* getTopLevelPVParent() const;
		PVInstance* getTopLevelPVParent();
		bool isTopLevelPVInstance() const;
		void setPVGridOffsetLegacy(const G3D::CoordinateFrame&);
		G3D::CoordinateFrame* getLegacyOffset();
		void clearLegacyOffset();
		virtual void legacyTraverseState(const G3D::CoordinateFrame&);
	public:
		//PVInstance& operator=(const PVInstance&);
	};
}
