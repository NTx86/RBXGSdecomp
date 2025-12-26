#pragma once
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/ICameraOwner.h"
#include "util/IRenderable.h"
#include "util/ICameraSubject.h"
#include "util/ISelectable3d.h"
#include "util/Extents.h"

namespace RBX
{
	class Primitive;
	class PartInstance;

	extern const char* sModel;
	class ModelInstance : public DescribedCreatable<ModelInstance, PVInstance, &sModel>,
						  public IRenderable,
						  public virtual IPrimaryPart,
						  public virtual ICameraSubject,
						  public virtual ISelectable3d
	{
	private:
		G3D::CoordinateFrame modelInPrimary;
		PartInstance* primaryPart;
		boost::shared_ptr<PartInstance> candidatePrimaryPart;
		ComputeProp<float, ModelInstance> FlagHeight;
		ComputeProp<Extents, ModelInstance> LocalGridExtents;
		ComputeProp<Extents, ModelInstance> WorldGridExtents;

	public:
		static bool showModelCoord;
	  
	private:
		void updatePrimaryPart(PartInstance*) const;
		float computeFlagHeight() const;
		Extents computeLocalGridExtents() const;
		Extents computeWorldGridExtents() const;
		void dirtyAll() const;
		virtual bool shouldRender3dAdorn() const;
		virtual void render3dAdorn(Adorn*);
		virtual void render3dSelect(Adorn*, SelectState);
	protected:
		virtual void onDescendentAdded(Instance*);
		virtual void onDescendentRemoving(const boost::shared_ptr<Instance>&);
		virtual bool askSetParent(const Instance*) const;
	private:
		virtual void onLastChildRemoved();
	public:
		//ModelInstance(const ModelInstance&);
		ModelInstance();
		virtual ~ModelInstance();
	public:
		PartInstance* getPrimaryPartInternal() const;
		void setPrimaryPart(PartInstance*);
		const G3D::CoordinateFrame& getModelInPrimary() const;
		void setModelInPrimary(const G3D::CoordinateFrame&);
		virtual void onExtentsChanged() const;
		virtual const Primitive* getBiggestPrimitive() const;
		virtual bool hitTest(const G3D::Ray&, G3D::Vector3&);
		virtual Extents getExtentsWorld() const;
		virtual Extents getExtentsLocal() const;
		virtual const G3D::CoordinateFrame getLocation() const;
		virtual void onCameraNear(float);
		virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>&);
		virtual PartInstance* getPrimaryPart();
		virtual const PartInstance* getPrimaryPartConst() const;
		void setFrontAndTop(const G3D::Vector3&);
		void destroyIfNoParts(const ModelInstance*);
		void makeJoints();
		void breakJoints();
		float computeTotalMass() const;
		float computeLargestMoment() const;
	protected:
		virtual void legacyTraverseState(const G3D::CoordinateFrame&);
	public:
		//ModelInstance& operator=(const ModelInstance&);
	};
}

