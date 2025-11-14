#pragma once

//Todo: Fill out class
namespace RBX 
{
	class IRenderableBucket;

	class IRenderable 
	{
	private:
		int index2d;
		int index3d;
		IRenderableBucket* bucket;
	protected:
		virtual bool shouldRender2d() const;
		virtual bool shouldRender3dAdorn() const;
	public:
		IRenderable(const IRenderable&);
		IRenderable();
		~IRenderable();
	};
}