#include "IComponent.h"
#include "nlohmann/json.hpp"

namespace anvil {

class TransformComponent  {
public:	
    TransformComponent(const nlohmann::json& data) {
        x_ = data.value("position", nlohmann::json::object()).value("x", 0.0f);
        y_ = data.value("position", nlohmann::json::object()).value("y", 0.0f);
        
    };
    
private:
    float getX() const          { return x_; }
    float getY() const          { return y_; }
    float x_, y_;
};
    
}
