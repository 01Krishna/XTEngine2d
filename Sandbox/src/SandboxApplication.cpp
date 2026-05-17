#include "../SandboxApplication.h"

//This is the main client application. It creates a class that inherits from the XTEngine2d::Application class
// and implements the CreateApplication function to return an instance of the Sandbox class.
//The run function is called in the main function defined in the EntryPoint.h file, which starts the application loop.



void Sandbox::Init()
{
	XTEngine2d::Application::Init();

	//playerTex = XTEngine2d::AssetManager::LoadTexture("C:\\Testing Resources\\Sprout Lands - Sprites - Basic pack\\Characters\\Basic Charakter Spritesheet.png");
	//playerTex = XTEngine2d::AssetManager::LoadTexture("C:\\Users\\HP\\Downloads\\ghostrunner-hel-cyberpunk-playstation-4-xbox-one-pc-games-3840x2160-7585.jpg");
	//XTEngine2d::Sprite sprite;
	//XTEngine2d::SpriteSheet sheet(playerTex.get(), 3840, 2160);
	//sprite.uv = sheet.GetUV(1, 1);
	//sprite.texture = playerTex;

	//assert(m_Scene != nullptr);

	//Entity player = m_Scene->m_Registry.CreateEntity();

	//m_Scene->m_Registry.AddComponent(player, XTEngine2d::Transform(glm::vec2(100,100),glm::vec2(400, 400),glm::vec2(30,30)));
	//m_Scene->m_Registry.AddComponent(player, XTEngine2d::PlayerController());
	//m_Scene->m_Registry.AddComponent(player, XTEngine2d::Sprite(sprite.texture, sprite.uv));
	//m_Scene->m_Registry.AddComponent(player, XTEngine2d::Tag("Player"));


	//std::shared_ptr<XTEngine2d::Texture> secondTex;
	//secondTex = XTEngine2d::AssetManager::LoadTexture("C:\\Users\\HP\\Downloads\\wp5029421-cyberpunk-2077-girl-4k-wallpapers.jpg");
	//XTEngine2d::Sprite sprite2;
	//XTEngine2d::SpriteSheet sheet2(secondTex.get(), 1920, 1080);
	//sprite2.uv = sheet2.GetUV(1, 1);
	//sprite2.texture = secondTex;
	//
	//Entity secondEntity = m_Scene->m_Registry.CreateEntity();
	//m_Scene->m_Registry.AddComponent(secondEntity, XTEngine2d::Transform(glm::vec2(100, 100), glm::vec2(400, 400), glm::vec2(30, 30)));
	//m_Scene->m_Registry.AddComponent(secondEntity, XTEngine2d::Sprite(sprite2.texture, sprite2.uv));
	//m_Scene->m_Registry.AddComponent(secondEntity, XTEngine2d::Tag("Second Entity"));

	//std::shared_ptr<XTEngine2d::Texture> thirdTex;
	//thirdTex = XTEngine2d::AssetManager::LoadTexture("C:\\Users\\HP\\Downloads\\wp5029421-cyberpunk-2077-girl-4k-wallpapers.jpg");
	//XTEngine2d::Sprite sprite3;
	//XTEngine2d::SpriteSheet sheet3(thirdTex.get(), 1920, 1080);
	//sprite3.uv = sheet3.GetUV(1, 1);
	//sprite3.texture = thirdTex;
	//
	//Entity thirdEntity = m_Scene->m_Registry.CreateEntity();
	//m_Scene->m_Registry.AddComponent(thirdEntity, XTEngine2d::Transform(glm::vec2(200, 200), glm::vec2(400, 400), glm::vec2(30, 30)));
	//m_Scene->m_Registry.AddComponent(thirdEntity, XTEngine2d::Sprite(sprite3.texture, sprite3.uv));
	//m_Scene->m_Registry.AddComponent(thirdEntity, XTEngine2d::Tag("Third Entity"));

	//Entity cameraEntity = m_Scene->m_Registry.CreateEntity();
	//XTEngine2d::Camera camera;
	//camera.SetViewPortSize(800, 600);
	//m_Scene->m_Registry.AddComponent(cameraEntity, camera);
	//m_Scene->m_Registry.AddComponent(cameraEntity, XTEngine2d::Transform(glm::vec2(10, 10), glm::vec2(0, 0), glm::vec2(0, 0)));
	//m_Scene->m_Registry.AddComponent(cameraEntity, XTEngine2d::CameraController());
	//m_Scene->m_Registry.AddComponent(cameraEntity, XTEngine2d::Tag("Camera"));

}
	
void Sandbox::Run()
{
	XTEngine2d::Application::Run();
}
	


XTEngine2d::Application* XTEngine2d::CreateApplication()
{
	return new Sandbox();
}