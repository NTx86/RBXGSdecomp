#pragma once
#include "v8datamodel/ModelInstance.h"
#include "v8world/Controller.h"
#include "util/InsertMode.h"
#include <G3D/Rect2D.h>

namespace RBX
{
	class World;
	class PartInstance;
	class HopperBin;
	class SpawnLocation;
	class MegaDragger;

	extern const char* sRootInstance;
	class RootInstance : public Reflection::Described<RootInstance, &sRootInstance, ModelInstance>,
						 public ICameraOwner
	{
	public:
		enum MoveType
		{
			MOVE_DROP,
			MOVE_NO_DROP
		};

	private:
		ComputeProp<ControllerTypeArray, RootInstance> ControllersUsed;
		G3D::Vector3 insertPoint;
	protected:
		G3D::Rect2D viewPort;
		std::auto_ptr<World> world;

	private:
		ControllerTypeArray computeControllersUsed() const;
		G3D::Vector3 computeIdeInsertPoint() const;
		G3D::Vector3 computeCharacterInsertPoint(const Extents&) const;
		void moveSafe(std::vector<boost::weak_ptr<PartInstance>>&, G3D::Vector3, MoveType);
		void moveSafe(MegaDragger&, G3D::Vector3, MoveType);
		void moveToCharacterInsertPoint(std::vector<boost::weak_ptr<PartInstance>>&);
		void moveToIdeInsertPoint(std::vector<boost::weak_ptr<PartInstance>>&);
		void insertRaw(const std::vector<boost::shared_ptr<Instance>>&, Instance*, std::vector<boost::weak_ptr<PartInstance>>&);
		void insertToTree(const std::vector<boost::shared_ptr<Instance>>&, Instance*);
		void insertCharacterView(const std::vector<boost::shared_ptr<Instance>>&, std::vector<boost::weak_ptr<PartInstance>>&);
		void insertIdeView(const std::vector<boost::shared_ptr<Instance>>&, std::vector<boost::weak_ptr<PartInstance>>&, PromptMode);
		void insert3dView(const std::vector<boost::shared_ptr<Instance>>&, PromptMode);
		void insertHopperBin(HopperBin*);
		void insertSpawnLocation(SpawnLocation*);
	protected:
		virtual void onDescendentAdded(Instance*);
		virtual void onDescendentRemoving(const boost::shared_ptr<Instance>&);
		virtual void onLastChildRemoved();
		virtual void onChildControllerChanged();
	public:
		//RootInstance(RootInstance&);
	protected:
		RootInstance();
		virtual ~RootInstance();
	public:
		void insertInstances(const std::vector<boost::shared_ptr<Instance>>&, Instance*, InsertMode, PromptMode);
		void setInsertPoint(const G3D::Vector3&);
		void moveToPoint(PVInstance*, G3D::Vector3);
		ControllerTypeArray getControllersUsed() const;
		World* getWorld();
		const G3D::Rect2D& getViewPort();
	public:
		//RootInstance& operator=(RootInstance&);
	};
}