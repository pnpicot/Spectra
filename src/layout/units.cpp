#include "units.hpp"

namespace sp {
    void UnitsHelper::updateTextSize(const sf::Vector2f &parentSize, const sf::Vector2f &windowSize, std::shared_ptr<sf::Text> text, Units unit, float size)
    {
        switch (unit) {
            case Units::CWP:
                {
                    float newSize = parentSize.x * (size / 100.0f);
                    text->setCharacterSize(static_cast<unsigned int>(newSize));
                }
                break;
            case Units::CHP:
                {
                    float newSize = parentSize.y * (size / 100.0f);
                    text->setCharacterSize(static_cast<unsigned int>(newSize));
                }
                break;
            case Units::WWP:
                {
                    float newSize = windowSize.x * (size / 100.0f);
                    text->setCharacterSize(static_cast<unsigned int>(newSize));
                }
                break;
            case Units::WHP:
                {
                    float newSize = windowSize.y * (size / 100.0f);
                    text->setCharacterSize(static_cast<unsigned int>(newSize));
                }
                break;
            default:
                break;
        }
    }
}