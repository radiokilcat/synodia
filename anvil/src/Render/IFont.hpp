#pragma once

namespace anvil {

class IFont {
public:
	virtual ~IFont() = default;
	virtual int getSize() const = 0;
};

} // namespace anvil
