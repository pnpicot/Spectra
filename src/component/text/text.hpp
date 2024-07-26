#pragma once

#include "../ADynamicComponent.hpp"
#include <cmath>
#include "../../layout/unitsEnums.hpp"
#include "../../layout/units.hpp"

namespace sp {
    enum class TextAlign {
        START,
        CENTER,
        END
    };

    class Text: public ADynamicComponent {
        public:
            Text(std::string id, sf::Vector2f *windowSize = nullptr);
            ~Text() = default;

        public:
            void render(std::shared_ptr<sf::RenderWindow> window);
            void render(std::unique_ptr<sf::RenderTexture> &texture);
            void setBackgroundColor(const sf::Color &color);
            const sf::Color &getBackgroundColor() const;
            void setBorderSize(float size);
            float getBorderSize() const;
            void setBorderColor(const sf::Color &color);
            const sf::Color &getBorderColor() const;

        public:
            void setFont(const sf::Font &font);
            const sf::Font *getFont() const;
            void setFontSize(float size, Units unit = Units::NONE);
            float getFontSize() const;
            void setTextColor(const sf::Color &color);
            const sf::Color &getTextColor() const;
            void setText(const std::string &string);
            const std::string getText() const;
            void setHorizontalAlign(TextAlign align);
            void setVerticalAlign(TextAlign align);

        protected:
            void setAbsoluteSize(sf::Vector2f size);
            void setAbsolutePosition(sf::Vector2f position);

        private:
            void updateTextSize();
            void repositionText();

        private:
            std::unique_ptr<sf::RectangleShape> _rect;
            std::shared_ptr<sf::Text> _text;
            TextAlign _horizontalAlign;
            TextAlign _verticalAlign;
            sf::FloatRect _textSize;
            Units _sizeUnit;
            float _adaptiveSize;
            sf::Vector2f *_windowSize;
            float _initialTop;
            float _initialLeft;
            unsigned int _initialSize;
    };
}