#pragma once

#include <memory>
#include "SFML/Graphics.hpp"

namespace sp {
    enum class PropertyUnit {
        PX,
        PERC,
        EM
    };

    enum class LayoutMode {
        DEFAULT,
        AUTO
    };

    enum class LayoutDirection {
        ROW,
        COLUMN
    };

    enum class PositionMode {
        RELATIVE,
        ABSOLUTE
    };

    enum class AlignMode {
        NONE,
        SPACE_BETWEEN,
        SPACE_EVENLY,
        SPACE_AROUND
    };

    enum class PropertyLabel {
        WIDTH,
        HEIGHT,
        LEFT,
        TOP,
        POSITION,
        PADDING_LEFT,
        PADDING_TOP,
        PADDING_RIGHT,
        PADDING_BOTTOM,
        MARGIN_LEFT,
        MARGIN_TOP,
        MARGIN_RIGHT,
        MARGIN_BOTTOM,
        LAYOUT_MODE,
        TRANSLATE_X,
        TRANSLATE_Y,
        LAYOUT_DIRECTION,
        HORIZONTAL_ALIGN,
        VERTICAL_ALIGN
    };

    struct PropertySettings {
        PropertyUnit unit;
        float value;
        LayoutMode layoutMode;
    };

    class IDynamicComponent {
        friend class Container;
        friend class Engine;
        friend class LayoutManager;

        public:
            virtual ~IDynamicComponent() = default;
            virtual void render(std::shared_ptr<sf::RenderWindow> window) = 0;
            virtual void render(std::unique_ptr<sf::RenderTexture> &texture) = 0;
            virtual int getZIndex() const = 0;
            virtual const std::string &getId() const = 0;
            virtual bool isRootComponent() const = 0;
            virtual sp::IDynamicComponent *getParent() = 0;
            virtual const sf::Vector2f &getRootWindowSize() const = 0;
            virtual const sf::Vector2f &getSize() const = 0;
            virtual const sf::Vector2f &getPosition() const = 0;
            virtual const sf::Vector2f &getRequestedSize() const = 0;
            virtual const sf::Vector2f &getRequestedPosition() const = 0;
            virtual sf::FloatRect getRect() const = 0;
            virtual void resize(sf::Vector2f size, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX }) = 0;
            virtual void move(sf::Vector2f position, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX }) = 0;
            virtual void setPositionMode(PositionMode mode) = 0;
            virtual PropertySettings &getPropertySettings(PropertyLabel label) = 0;
            virtual void setLayoutMode(LayoutMode mode) = 0;
            virtual void setTranslation(sf::Vector2f offset, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX }) = 0;
            virtual void setXTranslation(float offset, PropertyUnit unit = PropertyUnit::PX) = 0;
            virtual void setYTranslation(float offset, PropertyUnit unit = PropertyUnit::PX) = 0;
            virtual void setLayoutDirection(LayoutDirection direction) = 0;
            virtual void setHorizontalAlign(AlignMode mode) = 0;
            virtual void setVerticalAlign(AlignMode mode) = 0;

        public:
            virtual void setPadding(float value, PropertyUnit unit = PropertyUnit::PX) = 0;
            virtual void setPadding(sf::Vector2f values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX }) = 0;
            virtual void setPadding(sf::FloatRect values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX }) = 0;

        public:
            virtual void setMargin(float value, PropertyUnit unit = PropertyUnit::PX) = 0;
            virtual void setMargin(sf::Vector2f values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX }) = 0;
            virtual void setMargin(sf::FloatRect values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX }) = 0;

        protected:
            virtual void setZIndex(int zIndex) = 0;
            virtual void setRootComponent(bool state) = 0;
            virtual void setParent(sp::IDynamicComponent *parent) = 0;
            virtual void setRootWindowSize(sf::Vector2f size) = 0;
            virtual void updateProperties() = 0;
            virtual void updateProperty(PropertyLabel label, PropertyUnit unit, float value) = 0;
            virtual void setAbsoluteSize(sf::Vector2f size) = 0;
            virtual void setAbsolutePosition(sf::Vector2f position) = 0;
    };
}