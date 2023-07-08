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


		bool operator==(MockGameObject& g) {
			return id_ == g.id_;
		}
	};
}

// Demonstrate some basic assertions.
TEST(SerializeTest, BasicAssertions) {
	auto serializer = JsonSerializer<anvil::MockGameObject>("output.txt");
	auto player = anvil::MockGameObject(new anvil::LoaderParams(100, 100, 50, 37, "test"));
	serializer.serialize(player);
	auto tested = serializer.deserialize();
	bool result = player == tested;
	ASSERT_TRUE(result);
}