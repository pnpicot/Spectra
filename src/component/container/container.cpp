#include "container.hpp"

namespace sp {
    Container::Container(std::string id) : ADynamicComponent(id)
    {
        _texture = nullptr;
        _sprite = nullptr;
        _backgroundColor = sf::Color::Transparent;
    }

    bool Container::create(int width, int height)
    {
        _texture = std::make_unique<sf::RenderTexture>();
        _sprite = std::make_unique<sf::Sprite>();

        if (!_texture || !_sprite) {
            Logger::log(Logger::WARNING, "Could not allocate memory for container creation");
            return false;
        }

        sf::ContextSettings ctxSettings;

        ctxSettings.antialiasingLevel = 4;

        sf::IntRect textureRect = { 0, 0, width, height };

        // TODO: find a solution to render textures being too heavy on processor and storage (would it be possible to draw everything on a single texture but restrict the drawing area so there's no overflow ?)

        if (!_texture->create(width, height, ctxSettings)) {
            Logger::log(Logger::WARNING, "Could not allocate memory for container creation");
            return false;
        }

        _sprite->setTextureRect(textureRect);

        return true;
    }

    const sf::Color &Container::getBackgroundColor() const
    {
        return _backgroundColor;
    }

    void Container::setBackgroundColor(sf::Color backgroundColor)
    {
        _backgroundColor = backgroundColor;
    }

    void Container::setAbsoluteSize(sf::Vector2f size)
    {
        _size = { (float) floor(size.x), (float) floor(size.y) };

        if (!_sprite)
            return;

        sf::IntRect newRect = _sprite->getTextureRect();

        newRect.width = static_cast<int>(size.x);
        newRect.height = static_cast<int>(size.y);

        _sprite->setTextureRect(newRect);
    }

    void Container::setAbsolutePosition(sf::Vector2f position)
    {
        _position = { (float) floor(position.x), (float) floor(position.y) };

        if (!_sprite)
            return;

        _sprite->setPosition(position);
    }

    void Container::render(std::shared_ptr<sf::RenderWindow> window)
    {
        _texture->clear(_backgroundColor);
        renderChildren();
        _texture->display();
        _sprite->setTexture(_texture->getTexture(), false);
        window->draw(*_sprite);
    }

    void Container::render(std::unique_ptr<sf::RenderTexture> &texture)
    {
        _texture->clear(_backgroundColor);
        renderChildren();
        _texture->display();
        _sprite->setTexture(_texture->getTexture(), false);
        texture->draw(*_sprite);
    }

    void Container::renderChildren()
    {
        for (auto &child : _children) {
            child->render(_texture);
        }
    }

    int Container::getComponentIndex(const std::string &componentId)
    {
        unsigned int index = 0;

        for (auto &child : _children) {
            if (child->getId() == componentId)
                return index;

            index++;
        }

        return -1;
    }

    std::shared_ptr<sp::IDynamicComponent> Container::getComponent(const std::string &componentId)
    {
        for (auto &child : _children) {
            if (child->getId() == componentId)
                return child;
        }

        return nullptr;
    }

    void Container::appendComponent(std::shared_ptr<sp::ADynamicComponent> component)
    {
        if (component == nullptr)
            return;

        component->setRootComponent(false);
        component->setParent(this);
        _children.emplace_back(component);
        ComponentHelper::sortComponents(_children);
    }

    void Container::removeComponent(const std::string &componentId)
    {
        int componentIndex = getComponentIndex(componentId);

        if (componentIndex == -1)
            return;

        std::shared_ptr<sp::IDynamicComponent> child = _children[componentIndex];

        child->setParent(nullptr);
        _children.erase(_children.begin() + componentIndex);
    }
}