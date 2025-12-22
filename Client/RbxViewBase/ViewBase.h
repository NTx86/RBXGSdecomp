#pragma once
#include <boost/shared_ptr.hpp>
#include "RenderLib/RenderStats.h"

namespace RBX
{
	class Instance;
	class ViewBase
	{
	public:
		//ViewBase(const ViewBase&);
		ViewBase()
		{
		}
		virtual ~ViewBase()
		{
		}
	public:
		virtual void render(void* rd);
		virtual float getShadingQuality() const;
		virtual float getMeshDetail() const;
		virtual void updateSettings(float shadingQuality, float meshDetail, bool shadows, float cameraDistance);
		virtual void suppressSkybox();
		virtual Instance* getWorkspace();
		virtual RenderStats& getRenderStats();
	private:
		virtual void onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent);
		virtual void updateLighting();
		virtual void invalidateLighting(bool updateSkybox);
	public:
		//ViewBase& operator=(const ViewBase&);
	  
	public:
		static bool& getShadowsEnabled();
	};
}
