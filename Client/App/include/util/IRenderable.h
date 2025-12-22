#pragma once
#include "RbxGraphics/Adorn.h"
#include "util/IndexArray.h"

namespace RBX 
{
	class IRenderableBucket;

	class IRenderable
	{
	private:
		int index2d;
		int index3d;
		IRenderableBucket* bucket;

	private:
		int& indexFunc2d();
		int& indexFunc3d();
	protected:
		virtual bool shouldRender2d() const
		{
			return false;
		}
		virtual bool shouldRender3dAdorn() const
		{
			return false;
		}
	public:
		//IRenderable(const IRenderable&);
		IRenderable()
		{
		}
		~IRenderable()
		{
		}
	public:
		void shouldRenderSetDirty();
		virtual void render2d(Adorn* adorn)
		{
		}
		virtual void render3dAdorn(Adorn* adorn)
		{
		}
		virtual void render3dSelect(Adorn* adorn, SelectState selectState)
		{
		}
	public:
		//IRenderable& operator=(const IRenderable&);
	};


	class IRenderableBucket
	{
	private:
		IndexArray<IRenderable, &IRenderable::indexFunc2d> renderable2ds;
		IndexArray<IRenderable, &IRenderable::indexFunc3d> renderable3ds;
	  
	protected:
		void onAdded(IRenderable*);
		void onRemoving(IRenderable*);
		void recomputeShouldRender(IRenderable*);
	public:
		//IRenderableBucket(const IRenderableBucket&);
		IRenderableBucket();
		~IRenderableBucket();
	public:
		void render2dItems(Adorn*);
		void render3dAdornItems(Adorn*);
	public:
		//IRenderableBucket& operator=(const IRenderableBucket&);
	};
}
