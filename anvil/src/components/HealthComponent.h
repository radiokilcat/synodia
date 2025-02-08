#pragma once

namespace anvil {

struct HealthComponent {
    int healthPercentage;

    HealthComponent(int healthPercentage = 0) {
        this->healthPercentage = healthPercentage;
    }
};

}