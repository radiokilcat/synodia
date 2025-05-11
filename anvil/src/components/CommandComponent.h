#pragma once

namespace anvil {

struct CommandComponent {
    int healthPercentage;

    CommandComponent(int healthPercentage = 0) {
        this->healthPercentage = healthPercentage;
    }
};

}