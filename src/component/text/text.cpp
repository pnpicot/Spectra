#include "text.hpp"

namespace sp {
    Text::Text(std::string id) : ADynamicComponent(id)
    {
        _rect = std::make_unique<sf::RectangleShape>();
        _text = std::make_unique<sf::Text>();
        _horizontalAlign = TextAlign::CENTER;
        _verticalAlign = TextAlign::CENTER;
        _textSize = { 0, 0, 0, 0 };

        // TODO: automatically reposition text (cache the text bounds when you can, don't always recalculate it)

        if (!_rect || !_text) {
            Logger::log(Logger::WARNING, "Could not allocate memory for text");
            return;
        }

        _rect->setFillColor(sf::Color::Transparent);
    }

    void Text::render(std::shared_ptr<sf::RenderWindow> window)
    {
        if (_rect)
            window->draw(*_rect);

        if (_text)
            window->draw(*_text);
    }

    void Text::render(std::unique_ptr<sf::RenderTexture> &texture)
    {
        if (_rect)
            texture->draw(*_rect);

        if (_text)
            texture->draw(*_text);
    }

    void Text::setAbsoluteSize(sf::Vector2f size)
    {
        _size = { (float) floor(size.x), (float) floor(size.y) };

        if (!_rect)
            return;

        _rect->setSize(size);
        repositionText();
    }

    void Text::setAbsolutePosition(sf::Vector2f position)
    {
        _position = { (float) floor(position.x), (float) floor(position.y) };

        if (!_rect)
            return;

        _rect->setPosition(position);
        repositionText();
    }

    void Text::setBackgroundColor(const sf::Color &color)
    {
        if (!_rect)
            return;

        _rect->setFillColor(color);
    }

    const sf::Color &Text::getBackgroundColor() const
    {
        return _rect->getFillColor();
    }

    void Text::setBorderSize(float size)
    {
        if (!_rect)
            return;

        _rect->setOutlineThickness(size);
    }

    float Text::getBorderSize() const
    {
        if (!_rect)
            return 0;

        return _rect->getOutlineThickness();
    }

    void Text::setBorderColor(const sf::Color &color)
    {
        if (!_rect)
            return;

        _rect->setOutlineColor(color);
    }

    const sf::Color &Text::getBorderColor() const
    {
        return _rect->getOutlineColor();
    }

    void Text::setFont(const sf::Font &font)
    {
        if (!_text)
            return;

        _text->setFont(font);
        updateTextSize();
    }

    const sf::Font *Text::getFont() const
    {
        return _text->getFont();
    }

    void Text::setFontSize(unsigned int size)
    {
        if (!_text)
            return;

        _text->setCharacterSize(size);
        updateTextSize();
    }

    unsigned int Text::getFontSize() const
    {
        if (!_text)
            return 0;

        return _text->getCharacterSize();
    }

    void Text::setTextColor(const sf::Color &color)
    {
        if (!_text)
            return;

        _text->setFillColor(color);
    }

    const sf::Color &Text::getTextColor() const
    {
        return _text->getFillColor();
    }

    void Text::setText(const std::string &string)
    {
        if (!_text)
            return;

        _text->setString(string);
        updateTextSize();
    }

    const std::string Text::getText() const
    {
        return _text->getString();
    }

    void Text::setHorizontalAlign(TextAlign align)
    {
        _horizontalAlign = align;

        repositionText();
    }

    void Text::setVerticalAlign(TextAlign align)
    {
        _verticalAlign = align;

        repositionText();
    }

    void Text::updateTextSize()
    {
        if (!_text)
            return;

        _textSize = _text->getGlobalBounds();
    }

    void Text::repositionText()
    {
        if (!_text)
            return;

        sf::Vector2f textPosition = { 0, 0 };

        {
            // HORIZONTAL

            switch (_horizontalAlign) {
                case TextAlign::START:
                    textPosition.x = _position.x;
                    break;
                case TextAlign::END:
                    textPosition.x = (_position.x + _size.x) - _textSize.width - _textSize.left;
                    break;
                default:
                    textPosition.x = (_position.x + (_size.x / 2)) - (_textSize.width / 2);
                    break;
            }
        }

        {
            // VERTICAL

            switch (_verticalAlign) {
                case TextAlign::START:
                    textPosition.y = _position.y;
                    break;
                case TextAlign::END:
                    textPosition.y = (_position.y + _size.y) - _textSize.height - _textSize.top;
                    break;
                default:
                    textPosition.y = (_position.y + (_size.y / 2)) - (_textSize.height / 2) - _textSize.top;
                    break;
            }
        }

        _text->setPosition(textPosition);
    }
}