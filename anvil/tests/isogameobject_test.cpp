#include <gtest/gtest.h>
#include "anvil.h"
#include "isogameobject.h"

namespace anvil
{
    class MockGameObject : anvil::IsoGameObject
    {
    public:
        MockGameObject(const anvil::LoaderParams* params)
            : anvil::IsoGameObject(params)
        {

        }

        std::pair<int, int> IsoPosition(float x, float y) {
            return getIsoPosition(x, y);
        };

        std::pair<int, int> Position2D(float x, float y) {
            return getScreenPosition(x, y);
        };

        void clean() override {

        }


        bool operator==(MockGameObject& g) {
            return id_ == g.id_;
        }
    };
}

// Demonstrate some basic assertions.
TEST(isoObjectTest, BasicAssertions) {
    int startX = 100;
    int startY = 100;
    auto player = anvil::MockGameObject(new anvil::LoaderParams(startX, startY, 50, 37, "test"));
    auto isometric_position = player.IsoPosition(startX, startY);
    auto position2D = player.Position2D(isometric_position.first, isometric_position.second);

    bool result = position2D.first == startX;
    bool result1 = position2D.second == startY;

    ASSERT_TRUE(result);
    ASSERT_TRUE(result1);
}
