#include "rectangle.hpp"

namespace sp {
    Rectangle::Rectangle(std::string id) : ADynamicComponent(id)
    {
        _rectangle = std::make_unique<sf::RoundedRectangleShape>(sf::Vector2f(0, 0), 0, 1);

        if (!_rectangle) {
            Logger::log(Logger::WARNING, "Could not allocate memory for rectangle");
            return;
        }
    }

    void Rectangle::render(std::shared_ptr<sf::RenderWindow> window)
    {
        if (!_rectangle)
            return;

        window->draw(*_rectangle);
    }

    void Rectangle::render(std::unique_ptr<sf::RenderTexture> &texture)
    {
        if (!_rectangle)
            return;

        texture->draw(*_rectangle);
    }

    void Rectangle::setBackgroundColor(const sf::Color &color)
    {
        if (!_rectangle)
            return;

        _rectangle->setFillColor(color);
    }

    const sf::Color &Rectangle::getBackgroundColor() const
    {
        return _rectangle->getFillColor();
    }

    void Rectangle::setBorderRadius(float radius)
    {
        if (!_rectangle)
            return;

        _rectangle->setCornerPointCount(radius == 0 ? 1 : static_cast<int>(radius * 0.7) + 1);
        _rectangle->setCornersRadius(radius);
    }

    float Rectangle::getBorderRadius() const
    {
        if (!_rectangle)
            return 0;

        return _rectangle->getCornersRadius();
    }

    void Rectangle::setAbsoluteSize(sf::Vector2f size)
    {
        _size = { (float) floor(size.x), (float) floor(size.y) };

        if (!_rectangle)
            return;

        _rectangle->setSize(size);
    }

    void Rectangle::setAbsolutePosition(sf::Vector2f position)
    {
        _position = { (float) floor(position.x), (float) floor(position.y) };

        if (!_rectangle)
            return;

        _rectangle->setPosition(position);
    }

    void Rectangle::setBorderSize(float size)
    {
        if (!_rectangle)
            return;

        _rectangle->setOutlineThickness(size);
    }

    float Rectangle::getBorderSize() const
    {
        if (!_rectangle)
            return 0;

        return _rectangle->getOutlineThickness();
    }

    void Rectangle::setBorderColor(const sf::Color &color)
    {
        if (!_rectangle)
            return;

        _rectangle->setOutlineColor(color);
    }

    const sf::Color &Rectangle::getBorderColor() const
    {
        return _rectangle->getOutlineColor();
    }
}