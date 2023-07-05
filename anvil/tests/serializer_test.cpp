#include <gtest/gtest.h>
#include <memory>
#include "anvil.h"
#include "gameobject.h"
namespace anvil
{
	class MockGameObject : anvil::GameObject
	{
	public:
		MockGameObject() {}
		MockGameObject(const anvil::LoaderParams* params)
			: anvil::GameObject(params)
		{

		}

		void clean() override {

		}


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			MockGameObject,
			id_,
			currentFrame_,
			currentRow_,
			position_,
			velocity_,
			acceleration_,
			width_,
			height_)
	};
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	auto serializer = JsonSerializer<anvil::MockGameObject>("output.txt");
	auto player = anvil::MockGameObject(new anvil::LoaderParams(100, 100, 50, 37, "test"));
	serializer.serialize(player);
	auto tested = serializer.deserialize();
	ASSERT_EQ(0, 0);
}