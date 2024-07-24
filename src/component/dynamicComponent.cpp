#include "ADynamicComponent.hpp"

namespace sp {
    ADynamicComponent::ADynamicComponent(const std::string &id) : _id(id)
    {
        _zIndex = 0;
        _rootComponent = false;
        _parent = nullptr;
        _size = { 0, 0 };
        _position = { 0, 0 };

        initProperties();
        updateProperties();
    }

    int ADynamicComponent::getZIndex() const
    {
        return _zIndex;
    }

    const std::string &ADynamicComponent::getId() const
    {
        return _id;
    }

    void ADynamicComponent::setZIndex(int zIndex)
    {
        _zIndex = zIndex;
    }

    void ADynamicComponent::resize(sf::Vector2f size, std::vector<PropertyUnit> units)
    {
        _requestedSize = size;

        updateProperty(PropertyLabel::WIDTH, units.size() >= 1 ? units[0] : PropertyUnit::PX, size.x);
        updateProperty(PropertyLabel::HEIGHT, units.size() >= 2 ? units[1] : PropertyUnit::PX, size.y);
    }

    void ADynamicComponent::move(sf::Vector2f position, std::vector<PropertyUnit> units)
    {
        _requestedPosition = position;

        updateProperty(PropertyLabel::LEFT, units.size() >= 1 ? units[0] : PropertyUnit::PX, position.x);
        updateProperty(PropertyLabel::TOP, units.size() >= 2 ? units[1] : PropertyUnit::PX, position.y);
    }

    bool ADynamicComponent::isRootComponent() const
    {
        return _rootComponent;
    }

    sp::IDynamicComponent *ADynamicComponent::getParent()
    {
        return _parent;
    }

    void ADynamicComponent::setRootComponent(bool state)
    {
        _rootComponent = state;
    }

    void ADynamicComponent::setParent(sp::IDynamicComponent *parent)
    {
        _parent = parent;
    }

    const sf::Vector2f  &ADynamicComponent::getRootWindowSize() const
    {
        return _rootWindowSize;
    }

    void ADynamicComponent::setRootWindowSize(sf::Vector2f size)
    {
        _rootWindowSize = size;
    }

    void ADynamicComponent::initProperties()
    {
        _properties[PropertyLabel::POSITION].value = static_cast<float>(PositionMode::RELATIVE);
        _properties[PropertyLabel::LAYOUT_MODE].layoutMode = LayoutMode::DEFAULT;
        _properties[PropertyLabel::LAYOUT_DIRECTION].value = static_cast<float>(LayoutDirection::ROW);
        _properties[PropertyLabel::HORIZONTAL_ALIGN].value = static_cast<float>(LayoutAlignMode::NONE);
        _properties[PropertyLabel::VERTICAL_ALIGN].value = static_cast<float>(LayoutAlignMode::NONE);

        std::vector<PropertyLabel> zeroPxProperties = {
            PropertyLabel::PADDING_LEFT,
            PropertyLabel::PADDING_TOP,
            PropertyLabel::PADDING_RIGHT,
            PropertyLabel::PADDING_BOTTOM,
            PropertyLabel::MARGIN_LEFT,
            PropertyLabel::MARGIN_TOP,
            PropertyLabel::MARGIN_RIGHT,
            PropertyLabel::MARGIN_BOTTOM,
            PropertyLabel::TRANSLATE_X,
            PropertyLabel::TRANSLATE_Y
        };

        for (auto &label : zeroPxProperties) {
            _properties[label].unit = PropertyUnit::PX;
            _properties[label].value = 0;
        }
    }

    void ADynamicComponent::updateProperties()
    {
        _properties[PropertyLabel::WIDTH].unit = PropertyUnit::PX;
        _properties[PropertyLabel::WIDTH].value = _size.x;

        _properties[PropertyLabel::HEIGHT].unit = PropertyUnit::PX;
        _properties[PropertyLabel::HEIGHT].value = _size.y;

        _properties[PropertyLabel::LEFT].unit = PropertyUnit::PX;
        _properties[PropertyLabel::LEFT].value = _position.x;

        _properties[PropertyLabel::TOP].unit = PropertyUnit::PX;
        _properties[PropertyLabel::TOP].value = _position.y;
    }

    void ADynamicComponent::updateProperty(PropertyLabel label, PropertyUnit unit, float value)
    {
        _properties[label].unit = unit;
        _properties[label].value = value;
    }

    void ADynamicComponent::setPositionMode(PositionMode mode)
    {
        _properties[PropertyLabel::POSITION].value = static_cast<float>(mode);
    }

    PropertySettings &ADynamicComponent::getPropertySettings(PropertyLabel label)
    {
        return _properties[label];
    }

    void ADynamicComponent::setPadding(float value, PropertyUnit unit)
    {
        std::initializer_list<PropertyLabel> labels = {
            PropertyLabel::PADDING_LEFT,
            PropertyLabel::PADDING_TOP,
            PropertyLabel::PADDING_RIGHT,
            PropertyLabel::PADDING_BOTTOM
        };

        for (auto &label : labels) {
            _properties[label].unit = unit;
            _properties[label].value = value;
        }
    }

    void ADynamicComponent::setPadding(sf::Vector2f values, std::vector<PropertyUnit> units)
    {
        _properties[PropertyLabel::PADDING_LEFT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_LEFT].value = values.x;

        _properties[PropertyLabel::PADDING_RIGHT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_RIGHT].value = values.x;

        _properties[PropertyLabel::PADDING_TOP].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_TOP].value = values.y;

        _properties[PropertyLabel::PADDING_BOTTOM].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_BOTTOM].value = values.y;
    }

    void ADynamicComponent::setPadding(sf::FloatRect values, std::vector<PropertyUnit> units)
    {
        _properties[PropertyLabel::PADDING_LEFT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_LEFT].value = values.left;

        _properties[PropertyLabel::PADDING_TOP].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_TOP].value = values.top;

        _properties[PropertyLabel::PADDING_RIGHT].unit = units.size() >= 3 ? units[2] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_RIGHT].value = values.width;

        _properties[PropertyLabel::PADDING_BOTTOM].unit = units.size() >= 4 ? units[3] : PropertyUnit::PX;
        _properties[PropertyLabel::PADDING_BOTTOM].value = values.height;
    }

    void ADynamicComponent::setMargin(float value, PropertyUnit unit)
    {
        std::initializer_list<PropertyLabel> labels = {
            PropertyLabel::MARGIN_LEFT,
            PropertyLabel::MARGIN_TOP,
            PropertyLabel::MARGIN_RIGHT,
            PropertyLabel::MARGIN_BOTTOM
        };

        for (auto &label : labels) {
            _properties[label].unit = unit;
            _properties[label].value = value;
        }
    }

    void ADynamicComponent::setMargin(sf::Vector2f values, std::vector<PropertyUnit> units)
    {
        _properties[PropertyLabel::MARGIN_LEFT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_LEFT].value = values.x;

        _properties[PropertyLabel::MARGIN_RIGHT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_RIGHT].value = values.x;

        _properties[PropertyLabel::MARGIN_TOP].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_TOP].value = values.y;

        _properties[PropertyLabel::MARGIN_BOTTOM].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_BOTTOM].value = values.y;
    }

    void ADynamicComponent::setMargin(sf::FloatRect values, std::vector<PropertyUnit> units)
    {
        _properties[PropertyLabel::MARGIN_LEFT].unit = units.size() >= 1 ? units[0] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_LEFT].value = values.left;

        _properties[PropertyLabel::MARGIN_TOP].unit = units.size() >= 2 ? units[1] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_TOP].value = values.top;

        _properties[PropertyLabel::MARGIN_RIGHT].unit = units.size() >= 3 ? units[2] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_RIGHT].value = values.width;

        _properties[PropertyLabel::MARGIN_BOTTOM].unit = units.size() >= 4 ? units[3] : PropertyUnit::PX;
        _properties[PropertyLabel::MARGIN_BOTTOM].value = values.height;
    }

    const sf::Vector2f &ADynamicComponent::getSize() const
    {
        return _size;
    }

    const sf::Vector2f &ADynamicComponent::getPosition() const
    {
        return _position;
    }

    const sf::Vector2f &ADynamicComponent::getRequestedSize() const
    {
        return _requestedSize;
    }

    const sf::Vector2f &ADynamicComponent::getRequestedPosition() const
    {
        return _requestedPosition;
    }

    sf::FloatRect ADynamicComponent::getRect() const
    {
        return { _position.x, _position.y, _size.x, _size.y };
    }

    void ADynamicComponent::setLayoutMode(LayoutMode mode)
    {
        _properties[PropertyLabel::LAYOUT_MODE].layoutMode = mode;
    }

    void ADynamicComponent::setXTranslation(float offset, PropertyUnit unit)
    {
        _properties[PropertyLabel::TRANSLATE_X].value = offset;
        _properties[PropertyLabel::TRANSLATE_X].unit = unit;
    }

    void ADynamicComponent::setYTranslation(float offset, PropertyUnit unit)
    {
        _properties[PropertyLabel::TRANSLATE_Y].value = offset;
        _properties[PropertyLabel::TRANSLATE_Y].unit = unit;
    }

    void ADynamicComponent::setTranslation(sf::Vector2f offset, std::vector<PropertyUnit> units)
    {
        setXTranslation(offset.x, units.size() >= 1 ? units[0] : PropertyUnit::PX);
        setYTranslation(offset.y, units.size() >= 2 ? units[1] : PropertyUnit::PX);
    }

    void ADynamicComponent::setLayoutDirection(LayoutDirection direction)
    {
        _properties[PropertyLabel::LAYOUT_DIRECTION].value = static_cast<float>(direction);
    }

    void ADynamicComponent::setHorizontalAlign(LayoutAlignMode mode)
    {
        _properties[PropertyLabel::HORIZONTAL_ALIGN].value = static_cast<float>(mode);
    }

    void ADynamicComponent::setVerticalAlign(LayoutAlignMode mode)
    {
        _properties[PropertyLabel::VERTICAL_ALIGN].value = static_cast<float>(mode);
    }
}