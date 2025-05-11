#pragma once
#include "../ECS/ECS.h"
#include "../Commands/ICommand.hpp"
#include <vector>
#include <memory>

namespace anvil {


// Probable not a ECS System, but a command queue, move to commands folder

class CommandSystem: public System {

    public:
        static CommandSystem& Instance() {
            static CommandSystem instance;
            return instance;
        }

        void queueCommand(std::unique_ptr<ICommand> command) {
            m_commands.push_back(std::move(command));
        }
    
        void Update(float dt, const std::unique_ptr<Registry>& registry) {
            for (auto& cmd : m_commands) {
                cmd->update(dt);
            }
    
            auto it = m_commands.begin();
            while (it != m_commands.end()) {
                if ((*it)->isReady()) {
                    (*it)->execute();
                    it = m_commands.erase(it);
                } else {
                    ++it;
                }
            }
        }
    
    private:
        std::vector<std::unique_ptr<ICommand>> m_commands;
    };
    
}
