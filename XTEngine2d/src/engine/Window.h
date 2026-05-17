#pragma once
#include "Core.h"
#include "Log.h"
#include "Scene.h"

namespace XTEngine2d
{
	class Scene;

	class XT_API Window
	{

	public:
		int w_Width = 800, w_Height = 600;
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual void* GetNativeWindow() const = 0;
		static Window* Create();
		virtual bool Close() = 0;
		virtual void RenderUI() = 0;
		virtual void Clear() = 0;
		virtual void OnRender() = 0;
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
	};
};
