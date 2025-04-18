#pragma once
#include <string>
#include <vector>

#include "../AssetStore/AssetStore.h"


namespace anvil {

struct AssetRequest {
    enum class Type { Texture, Font, Sound, Music };
    Type type;
    std::string id;
    std::string path;
};

class ILoadableState {
    public:
        virtual std::vector<AssetRequest> getAssetsToLoad() const = 0;
        virtual void onAssetsLoaded(AssetStore&& store) = 0;
        virtual ~ILoadableState() = default;
    };
    
}