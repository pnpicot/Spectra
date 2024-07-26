#include "assetsManager.hpp"

namespace sp {
    bool AssetsManager::loadFont(const std::string &id, const std::string &fontPath)
    {
        sf::Font newFont;

        if (!newFont.loadFromFile(fontPath))
            return false;

        _fonts.emplace(id, newFont);

        return true;
    }

    const sf::Font &AssetsManager::getFont(const std::string &id) const
    {
        return _fonts.at(id);
    }
}