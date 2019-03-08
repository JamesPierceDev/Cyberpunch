#include "OptionsScene.h"

/// <summary>
/// 
/// </summary>
OptionsScene::OptionsScene()
	: m_txtImage(new Entity("Credits")),
	  m_camera(false)
{
}

/// <summary>
/// 
/// </summary>
OptionsScene::~OptionsScene()
{

}

/// <summary>
/// 
/// </summary>
void OptionsScene::start()
{
	auto pos = new PositionComponent(960, 540);
	m_txtImage->addComponent("Pos", pos);
	auto sprite = new SpriteComponent(pos, Vector2f(1920, 1080), Vector2f(1920, 1080), Scene::resources().getTexture("Credits"), 0);
	m_txtImage->addComponent("Sprite", sprite);

	Scene::systems()["Render"]->addComponent(&m_txtImage->getComponent("Sprite"));
}

/// <summary>
/// 
/// </summary>
void OptionsScene::stop()
{
	Scene::systems()["Render"]->deleteComponent(&m_txtImage->getComponent("Sprite"));
	delete(&m_txtImage->getComponent("Pos"));
	delete(&m_txtImage->getComponent("Sprite"));
}

/// <summary>
/// 
/// </summary>
/// <param name="dt"></param>
void OptionsScene::update(double dt)
{

}

/// <summary>
/// 
/// </summary>
/// <param name="renderer"></param>
void OptionsScene::draw(SDL_Renderer & renderer)
{
	static_cast<RenderSystem *>(Scene::systems()["Render"])->render(renderer, m_camera);
}

/// <summary>
/// 
/// </summary>
/// <param name="input"></param>
void OptionsScene::handleInput(InputSystem& input)
{
	Scene::systems()["Input"]->update(0);
	m_input = static_cast<InputComponent *>(input.m_components.at(0));

	if (m_input->m_current["ABTN"])
	{
		Scene::goToScene("Main Menu");
	}
}