#pragma once
#include <set>
#include <vector>
#include <G3DAll.h>
#include "util/HitTestFilter.h"
#include "util/Extents.h"

namespace RBX
{
	class Contact;
	class Primitive;
	class World;
	class SpatialHash;

	class ContactManager
	{
	private:
		SpatialHash* spatialHash;
		World* world;
	private:
		static bool ignoreBool;

	private:
		Contact* createContact(Primitive*, Primitive*);
		void stepBroadPhase();
		Primitive* getSlowHit(const G3D::Array<Primitive*>&, const G3D::Ray&, const G3D::Array<Primitive const*>*, const HitTestFilter*, G3D::Vector3&, float, bool&, bool&) const;
		Primitive* getFastHit(const G3D::Ray&, const G3D::Array<RBX::Primitive const*>*, const HitTestFilter*, G3D::Vector3&, bool&, bool&) const;
	public:
		ContactManager(World*);
		~ContactManager();
	public:
		const SpatialHash& getSpatialHash();
		Primitive* getHit(const G3D::Ray&, const std::vector<Primitive const*>*, const HitTestFilter*, G3D::Vector3&, bool&) const;
		Primitive* getHit(const G3D::Ray&, const G3D::Array<Primitive const*>*, const HitTestFilter*, G3D::Vector3&, bool&) const;
		void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found);
		bool intersectingOthers(Primitive*, const std::set<Primitive*>&, float);
		bool intersectingOthers(const G3D::Array<Primitive *>&, float);
		bool intersectingOthers(Primitive*, float);
		void onNewPair(Primitive*, Primitive*);
		void onReleasePair(Primitive* p0, Primitive* p1);
		void onPrimitiveAdded(Primitive* p);
		void onPrimitiveRemoved(Primitive* p);
		void onPrimitiveExtentsChanged(Primitive* p);
		void onPrimitiveGeometryTypeChanged(Primitive*);
		void stepWorld();
		RBX::Primitive* getHitLegacy(const G3D::Ray&, const Primitive*, const HitTestFilter*, G3D::Vector3&, float&, const float&) const;
	};
}
