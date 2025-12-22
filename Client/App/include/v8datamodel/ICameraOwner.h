#pragma once
#include <vector>
#include <g3d/gcamera.h>
#include "util/Extents.h"

namespace RBX
{
	class Primitive;
	class PartInstance;
	class Camera;
	class ICameraOwner
	{
	private:
		std::vector<boost::weak_ptr<PartInstance>> cameraIgnoreParts;
	  
	public:
		//ICameraOwner(const ICameraOwner&);
		ICameraOwner();
		virtual ~ICameraOwner();
	public:
		virtual Camera* getCamera() const;
		virtual const G3D::GCamera& getGCamera() const;
		virtual void cameraMoved();
		virtual Extents computeCameraOwnerExtents();
	public:
		void setCameraIgnoreParts(const std::vector<PartInstance*>&);
		void setCameraIgnoreParts(PartInstance*);
		void clearCameraIgnoreParts();
		void getCameraIgnorePrimitives(std::vector<const Primitive*>&);
	public:
		//ICameraOwner& operator=(const ICameraOwner&);
	};
}
