#pragma once

#include <string>
#include <memory>
#include "SFML/Graphics.hpp"
#include "../../logger/logger.hpp"
#include "../ADynamicComponent.hpp"
#include "../../helpers/componentHelper.hpp"
#include <cmath>

namespace sp {
    class Container: public ADynamicComponent {
        friend class Engine;
        friend class LayoutManager;

        public:
            Container(std::string id);
            ~Container() = default;

        public:
            bool create(int width, int height);
            const sf::Color &getBackgroundColor() const;
            void setBackgroundColor(sf::Color backgroundColor);
            void render(std::shared_ptr<sf::RenderWindow> window);
            void render(std::unique_ptr<sf::RenderTexture> &texture);
            void appendComponent(std::shared_ptr<sp::ADynamicComponent> component);
            void removeComponent(const std::string &componentId);
            int getComponentIndex(const std::string &componentId);
            std::shared_ptr<sp::IDynamicComponent> getComponent(const std::string &componentId);

        protected:
            void setAbsoluteSize(sf::Vector2f size);
            void setAbsolutePosition(sf::Vector2f position);

        private:
            void renderChildren();

        protected:
            std::vector<std::shared_ptr<sp::IDynamicComponent>> _children;

        private:
            std::unique_ptr<sf::RenderTexture> _texture;
            std::unique_ptr<sf::Sprite> _sprite;
            sf::Color _backgroundColor;
    };
}