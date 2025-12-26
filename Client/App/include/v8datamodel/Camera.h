#pragma once
#include <G3DAll.h>
#include "reflection/reflection.h"
#include "v8tree/Instance.h"
#include "v8world/ContactManager.h"
#include "util/Extents.h"

namespace RBX
{
	extern const char* sCamera;

	class ICameraOwner;
	class ICameraSubject;
	class Camera : public DescribedCreatable<Camera, Instance, &sCamera>
	{
	public:
		enum CameraType
		{
			FIXED_CAMERA = 0,
			ATTACH_CAMERA = 1,
			WATCH_CAMERA = 2,
			TRACK_CAMERA = 3,
			FOLLOW_CAMERA = 4,
			CUSTOM_CAMERA = 5,
			NUM_CAMERA_TYPE = 6,
		};
		enum AnimationType
		{
			ALWAYS = 0,
			AUTO = 1,
		};
		enum ZoomType 
		{
			ZOOM_IN_OR_OUT = 0,
			ZOOM_OUT_ONLY = 1,
			ZOOM_CHAR_PART_DRAG = 2,
		};
	private:
		G3D::GCamera gCamera;
		G3D::CoordinateFrame cameraGoal;
		G3D::CoordinateFrame cameraFocus;
		CameraType cameraType;
		AnimationType animationType;
		boost::shared_ptr<Instance> cameraSubject;
		bool cameraExternallyAdjusted;
	private:
		ICameraOwner* getCameraOwner();
		void updateFocus();
		void updateGoal();
		bool characterZoom(float);
		bool nonCharacterZoom(float in);
		void tryZoomExtents(float, float, float, const Extents&, const G3D::Rect2D&);
		ContactManager& getContactManager();
		float goalToFocusDistance() const;
		void setGCameraCoordinateFrame(const G3D::CoordinateFrame&);
		G3D::CoordinateFrame computeLineOfSiteGoal();
		void getHeadingElevationDistance(float &heading, float &elevation, float &distance);
		void setHeadingElevationDistance(float, float, float);
		void tellCameraMoved();
		void getIgnorePrims(G3D::Array<Primitive const *>&);
		virtual bool askSetParent(const Instance *instance) const;

	public:
		//Camera(const RBX::Camera&);
		Camera();
		virtual ~Camera();
	public:
		const G3D::GCamera& getGCamera() const
		{
			return gCamera;
		}
		void onHeartbeat();
		void autoMode();
		void alwaysMode();
		bool isCharacterCamera() const;
		bool isFirstPersonCamera() const;
		ICameraSubject* getCameraSubject() const;
		Instance* getCameraSubjectInstance() const;
		void setCameraSubject(Instance*);
		const G3D::CoordinateFrame& getCameraFocus() const
		{
			return cameraFocus;
		}
		void setCameraFocus(const G3D::CoordinateFrame &value);
		G3D::CoordinateFrame getCameraCoordinateFrame() const
		{
			return gCamera.getCoordinateFrame();
		}
		void setCameraCoordinateFrameNoLerp(const G3D::CoordinateFrame&);
		void goalToCamera();
		CameraType getCameraType() const;
		void setCameraType(CameraType type);
		bool canZoom(int) const;
		bool canTilt(int) const;
		void onWrapMouse(const G3D::Vector2&);
		bool zoom(float in);
		bool setDistanceFromTarget(float);
		void zoomExtents(Extents, const G3D::Rect2D&, ZoomType);
		bool zoomExtents(const G3D::Rect2D &viewPort);
		void panRadians(float angle);
		void panUnits(int);
		bool tiltRadians(float);
		bool tiltUnits(int);
		void lookAt(const G3D::Vector3 &point);
		void setImageServerViewNoLerp(const G3D::CoordinateFrame&, const G3D::Rect2D&);
		//RBX::Camera& operator=(const RBX::Camera&);
  
	public: 
		static float distanceDefault();
		static float distanceMin();
		static float distanceMax();
		static float distanceMaxCharacter();
		static float distanceMinOcclude();
		static float getNewZoomDistance(float, float);

		static const Reflection::PropDescriptor<Camera, CameraType> desc_cameraType;
		static const Reflection::PropDescriptor<Camera, G3D::CoordinateFrame> desc_CoordFrame;
		static const Reflection::PropDescriptor<Camera, G3D::CoordinateFrame> desc_Focus;
		static const Reflection::PropDescriptor<Camera, Instance*> cameraSubjectProp;
	};
}