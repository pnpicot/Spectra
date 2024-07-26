#pragma once

#include "../ADynamicComponent.hpp"
#include "../../sfml_extends/roundedRectangle.hpp"
#include <cmath>

namespace sp {
    class Rectangle: public ADynamicComponent {
        friend class Engine;
        friend class LayoutManager;

        public:
            Rectangle(std::string id);
            ~Rectangle() = default;

        public:
            void render(std::shared_ptr<sf::RenderWindow> window);
            void render(std::unique_ptr<sf::RenderTexture> &texture);
            void setBackgroundColor(const sf::Color &color);
            const sf::Color &getBackgroundColor() const;
            void setBorderRadius(float radius);
            float getBorderRadius() const;
            void setBorderSize(float size);
            float getBorderSize() const;
            void setBorderColor(const sf::Color &color);
            const sf::Color &getBorderColor() const;

        protected:
            void setAbsoluteSize(sf::Vector2f size);
            void setAbsolutePosition(sf::Vector2f position);

        private:
            std::unique_ptr<sf::RoundedRectangleShape> _rectangle;
    };
}