#pragma once
#include "Window.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "KeyboardInputHandle.h"
#include "MouseInputHandle.h"
#include "Textures.h"
#include "Sprite.h"
#include "Shader.h" 
#include "SpriteSheet.h"
#include "PlayerController.h"
#include "Scene.h"
#include "Renderer.h"

namespace XTEngine2d
{

	class XT_API GLFWWindow : public Window
	{
	private:
		GLFWwindow* m_Window;
		unsigned int m_Width, m_Height;
		KeyboardInputHandle Keyboard_Input;


	public:
		GLFWWindow(unsigned int width, unsigned int height);
		virtual ~GLFWWindow();

		void OnUpdate() override;
		bool Close();
		void Clear() override;
		void RenderUI() override;
		void OnRender() override;
		int GetWidth() override { return w_Width; }
		int GetHeight() override { return w_Height; }

		void* GetNativeWindow() const override { return m_Window; }


	private:
		void Init();
		void Shutdown();
	};
}

