#pragma once

#include "SFML/Graphics.hpp"
#include <string>

namespace sp {
    class AssetsManager {
        public:
            bool loadFont(const std::string &id, const std::string &fontPath);
            const sf::Font &getFont(const std::string &id) const;

        private:
            std::map<std::string, sf::Font> _fonts;
    };
}