#pragma once

#include "../component/IDynamicComponent.hpp"
#include "../component/text/text.hpp"
#include "unitsEnums.hpp"

namespace sp {
    class UnitsHelper {
        public:
            static void updateTextSize(const sf::Vector2f &parentSize, const sf::Vector2f &windowSize, std::shared_ptr<sf::Text> text, Units unit, float size);
    };
}