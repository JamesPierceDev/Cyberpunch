#include "AchievementsScene.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "RenderSystem.h"

AchievementsScene::AchievementsScene() :
	m_bgE("BG"),
	m_selectE("Selector"),
	m_initialSetup(false),
	m_camera(false),
	m_animTimer(2.25f)
{
}

void AchievementsScene::start()
{
	m_animTimer = 2.25f;

	if (!m_initialSetup)
	{
		auto pos = new PositionComponent(960, 540);
		m_bgE.addComponent("Pos", pos);
		auto sprite = new SpriteComponent(pos, Vector2f(1920, 1080), Vector2f(1920, 1080), Scene::resources().getTexture("Achievements BG"), 0);
		m_bgE.addComponent("Sprite", sprite);

		//Defining all the names for the vectors
		std::vector<std::string> names({"Punch First",
			"Final Form",
			"First Blood",
			"Damage Dealer",
			"Stunner",
			"Eh Aye",
			"Friends!",
			"Bend it like Beckham",
			"Last Woman Standing",
			"Quitter...",
			"Punching Bag",
			"Party Time",
			"The Upside Down",
			"Going Down",
			"Back to The Future",
			"Mover" });

		for (auto name : names)
			m_icons.push_back(Entity(name));

		auto selectPos = new PositionComponent(672, 216);
		auto boxPos = new PositionComponent(672, 216 + 83);
		m_selectE.addComponent("Pos", selectPos);
		m_selectE.addComponent("Box Pos", boxPos);
		m_selectE.addComponent("Sprite", new SpriteComponent(selectPos, Vector2f(700, 70), Vector2f(70, 70), Scene::resources().getTexture("Achievement Selected"), 3));
		m_selectE.addComponent("Des Sprite", new SpriteComponent(boxPos, Vector2f(2200, 100), Vector2f(220, 100), Scene::resources().getTexture("Achievement Des Box"), 3));
		m_selectE.addComponent("Description Sprite", new SpriteComponent(boxPos, Vector2f(2200, 100), Vector2f(220, 100), NULL, 2));
		std::vector<SDL_Rect> boxRects, desRects;
		for (int i = 0; i < 10; i++)
		{
			boxRects.push_back({i * 70, 0, 70, 70});
			desRects.push_back({i * 220, 0, 220, 100});
		}
		auto aAnim = new AnimationComponent(&m_selectE.getComponent("Sprite"));
		aAnim->addAnimation("Select", Scene::resources().getTexture("Achievement Selected"), boxRects, .25f);
		auto bAnim = new AnimationComponent(&m_selectE.getComponent("Des Sprite"));
		bAnim->addAnimation("Select", Scene::resources().getTexture("Achievement Des Box"), desRects, .25f);
		auto cAnim = new AnimationComponent(&m_selectE.getComponent("Description Sprite"));
		cAnim->addAnimation("Select", Scene::resources().getTexture("Punch First Des"), desRects, .25f);
		//Add animations to the component
		m_selectE.addComponent("A Animation", aAnim);
		m_selectE.addComponent("B Animation", bAnim);
		m_selectE.addComponent("C Animation", cAnim);
		aAnim->playAnimation("Select", false);
		bAnim->playAnimation("Select", false);
		cAnim->playAnimation("Select", false);

		int index = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				auto pos = new PositionComponent(672 + j * 192, 216 + i * 216);
				m_icons.at(index).addComponent("Pos", pos);
				m_icons.at(index).addComponent("Sprite", new SpriteComponent(pos, Vector2f(500, 50), Vector2f(50, 50), Scene::resources().getTexture("Achievement Locked"), 1));
				m_icons.at(index).addComponent("Info Sprite", new SpriteComponent(pos, Vector2f(2200, 100), Vector2f(220, 100), Scene::resources().getTexture(m_icons.at(index).m_ID + " Des"), 1));
				index++;
			}
		}
	}

	auto achieves = Scene::achievements().m_achievements;

	for (auto& ent : m_icons)
	{
		//If the achievement has been unlocked, set the sprite and animation for the icon
		if (achieves[ent.m_ID])
		{
			std::vector<SDL_Rect> m_animRects;
			for (int i = 0; i < 10; i++)
				m_animRects.push_back({i*50, 0, 50, 50});

			auto anim = new AnimationComponent(&ent.getComponent("Sprite"));
			anim->addAnimation("Unlocked", Scene::resources().getTexture(ent.m_ID), m_animRects, 1.0f);
			anim->playAnimation("Unlocked", false);

			ent.addComponent("Animation", anim);
			Scene::systems()["Animation"]->addComponent(anim);
		}
	}

	for (auto& icon : m_icons)
		Scene::systems()["Render"]->addComponent(&icon.getComponent("Sprite"));

	Scene::systems()["Render"]->addComponent(&m_selectE.getComponent("Sprite"));
	Scene::systems()["Render"]->addComponent(&m_selectE.getComponent("Des Sprite"));
	Scene::systems()["Render"]->addComponent(&m_selectE.getComponent("Description Sprite"));

	Scene::systems()["Animation"]->addComponent(&m_selectE.getComponent("A Animation"));
	Scene::systems()["Animation"]->addComponent(&m_selectE.getComponent("B Animation"));
	Scene::systems()["Animation"]->addComponent(&m_selectE.getComponent("C Animation"));

	//Add the bg to the render system
	Scene::systems()["Render"]->addComponent(&m_bgE.getComponent("Sprite"));
}

void AchievementsScene::stop()
{
	for (auto& icon : m_icons)
		Scene::systems()["Render"]->deleteComponent(&icon.getComponent("Sprite"));

	Scene::systems()["Render"]->deleteComponent(&m_selectE.getComponent("Sprite"));
	Scene::systems()["Render"]->deleteComponent(&m_selectE.getComponent("Des Sprite"));
	Scene::systems()["Render"]->deleteComponent(&m_selectE.getComponent("Description Sprite"));

	Scene::systems()["Animation"]->deleteComponent(&m_selectE.getComponent("A Animation"));
	Scene::systems()["Animation"]->deleteComponent(&m_selectE.getComponent("B Animation"));
	Scene::systems()["Animation"]->deleteComponent(&m_selectE.getComponent("C Animation"));

	//Remove components from the systems
	Scene::systems()["Render"]->deleteComponent(&m_bgE.getComponent("Sprite"));
}

void AchievementsScene::update(double dt)
{
	//Update the animations
	Scene::systems()["Animation"]->update(dt);

	m_animTimer -= dt;

	if (m_animTimer <= 0)
	{
		//Player all the animations for the entities if they have one
		for (auto& ent : m_icons)
		{
			//If the entity has an animation, play it
			auto a = dynamic_cast<AnimationComponent*>(&ent.getComponent("Animation"));

			if (nullptr != a)
			{
				a->getCurrentAnimation()->resetAnimation();
				a->playAnimation("Unlocked", false);
			}
		}

		m_animTimer = 2.25f;
	}
}

void AchievementsScene::handleInput(InputSystem & input)
{
	//Update the input system
	Scene::systems()["Input"]->update(0);
	m_moveChange.zeroVector(); //Reset the movement change

	if (!m_initialSetup)
	{
		m_input = static_cast<InputComponent*>(input.m_components.at(0));
		m_initialSetup = true;
	}

	if (m_input->isButtonPressed("ABTN"))
		Scene::goToScene("Main Menu");

	if (m_input->isButtonPressed("STICKLEFT"))
		m_moveChange.x = -1;
	if (m_input->isButtonPressed("STICKRIGHT"))
		m_moveChange.x = 1;
	if (m_input->isButtonPressed("STICKDOWN"))
		m_moveChange.y = 1;
	if (m_input->isButtonPressed("STICKUP"))
		m_moveChange.y = -1;

	if (m_moveChange.x != 0 || m_moveChange.y != 0)
	{
		auto boxPos = static_cast<PositionComponent*>(&m_selectE.getComponent("Pos"));
		auto desPos = static_cast<PositionComponent*>(&m_selectE.getComponent("Box Pos"));

		auto newPos = boxPos->position + Vector2f(192 * m_moveChange.x, 216 * m_moveChange.y);
		

		//Clamp the position of the achievement selector
		if (newPos.x < 672)
			newPos.x = 672;
		if (newPos.x > 1248)
			newPos.x = 1248;
		if (newPos.y < 216)
			newPos.y = 216;
		if (newPos.y > 864)
			newPos.y = 864;

		//If we have selected a new achievement
		if (newPos != boxPos->position)
		{
			boxPos->position = newPos;
			desPos->position.x = boxPos->position.x;
			desPos->position.y = boxPos->position.y + 83;

			auto aAnim = static_cast<AnimationComponent*>(&m_selectE.getComponent("A Animation"));
			auto bAnim = static_cast<AnimationComponent*>(&m_selectE.getComponent("B Animation"));
			aAnim->getCurrentAnimation()->resetAnimation();
			aAnim->playAnimation("Select", false);
			bAnim->getCurrentAnimation()->resetAnimation();
			bAnim->playAnimation("Select", false);

			for (auto& icon : m_icons)
			{
				//If the entities position matches the selected position, set the description
				if (static_cast<PositionComponent&>(icon.getComponent("Pos")).position == boxPos->position)
				{
					auto anim = static_cast<AnimationComponent*>(&m_selectE.getComponent("C Animation"));
					anim->getCurrentAnimation()->resetAnimation();
					anim->getCurrentAnimation()->setTexture(static_cast<SpriteComponent*>(&icon.getComponent("Info Sprite"))->getTexture());
					anim->playAnimation("Select", false);
					break;
				}
			}
		}
	}
}

void AchievementsScene::draw(SDL_Renderer & renderer)
{
	static_cast<RenderSystem*>(Scene::systems()["Render"])->render(renderer, m_camera);
}
