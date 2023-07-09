#include <gtest/gtest.h>
#include <memory>
#include "anvil.h"
#include "gameobject.h"
#include "menustate.h"
#include "json_serializer/game_object_serializer.h"
#include "json_serializer/menu_state_serializer.h"

namespace anvil
{
	class MockGameObject : public anvil::GameObject
	{
	public:
		MockGameObject() {}
		MockGameObject(const anvil::LoaderParams* params)
			: anvil::GameObject(params)
		{

		}

		void clean() override {

		}


		bool operator==(MockGameObject& g) {
			return id_ == g.id_;
		}
	};


	class MockMenuState : public anvil::MenuState
	{
	public:
		MockMenuState()
			: anvil::MenuState()
		{

		}

		void addGameObj(MockGameObject *obj) {
			m_gameObjects.push_back(obj);
		}

		bool operator==(MockMenuState state) {
			bool eq = true;
			if (m_gameObjects.size() != state.m_gameObjects.size()) {
				return false;
			}
			for (int i = 0; i < m_gameObjects.size(); i++) {
				eq &= *(MockGameObject *)m_gameObjects[i] == *(MockGameObject*)state.m_gameObjects[i];
			}
			return eq;
		}
	};
}

TEST(SerializerTests, SerializeSingleObject) {
	anvil::GameObjectSerializer seri;
	auto serializer = JsonSerializer<anvil::GameObjectSerializer, anvil::MockGameObject>("output.txt", seri);
	auto player = anvil::MockGameObject(new anvil::LoaderParams(100, 100, 50, 37, "test"));
	serializer.serialize(player);
	auto result = serializer.deserialize();
	ASSERT_TRUE(player == result);
}

TEST(SerializerTests, SerializeScene) {
	anvil::GameObjectSerializer seri;
	auto serializer = JsonSerializer<anvil::MenuStateSerializer, anvil::MockMenuState>("output.txt", seri);
	auto menu = anvil::MockMenuState();
	auto player1 = anvil::MockGameObject(new anvil::LoaderParams(100, 100, 50, 37, "test"));
	auto player2 = anvil::MockGameObject(new anvil::LoaderParams(100, 100, 50, 37, "test"));
	menu.addGameObj(&player1);
	menu.addGameObj(&player2);
	serializer.serialize(menu);
	auto result = serializer.deserialize();
	ASSERT_TRUE(menu == result);
}