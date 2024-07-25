#pragma once

#include "IDynamicComponent.hpp"
#include "../logger/logger.hpp"

namespace sp {
    class ADynamicComponent: public IDynamicComponent {
        friend class Engine;
        friend class Container;

        public:
            ADynamicComponent(const std::string &id);
            ~ADynamicComponent() = default;

        public:
            int getZIndex() const;
            const std::string &getId() const;
            void resize(sf::Vector2f size, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX });
            void move(sf::Vector2f position, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX });
            bool isRootComponent() const;
            sp::IDynamicComponent *getParent();
            const sf::Vector2f &getRootWindowSize() const;
            void setPositionMode(PositionMode mode);
            PropertySettings &getPropertySettings(PropertyLabel label);
            const sf::Vector2f &getSize() const;
            const sf::Vector2f &getPosition() const;
            const sf::Vector2f &getRequestedSize() const;
            const sf::Vector2f &getRequestedPosition() const;
            sf::FloatRect getRect() const;
            void setLayoutMode(LayoutMode mode);
            void setTranslation(sf::Vector2f offset, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX });
            void setXTranslation(float offset, PropertyUnit unit = PropertyUnit::PX);
            void setYTranslation(float offset, PropertyUnit unit = PropertyUnit::PX);
            void setLayoutDirection(LayoutDirection direction);
            void setHorizontalAlign(AlignMode mode);
            void setVerticalAlign(AlignMode mode);

        public:
            void setPadding(float value, PropertyUnit unit = PropertyUnit::PX);
            void setPadding(sf::Vector2f values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX });
            void setPadding(sf::FloatRect values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX });

        public:
            void setMargin(float value, PropertyUnit unit = PropertyUnit::PX);
            void setMargin(sf::Vector2f values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX });
            void setMargin(sf::FloatRect values, std::vector<PropertyUnit> units = { PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX, PropertyUnit::PX });

        protected:
            void setZIndex(int zIndex);
            void setRootComponent(bool state);
            void setParent(sp::IDynamicComponent *parent);
            void setRootWindowSize(sf::Vector2f size);
            void updateProperties();
            void updateProperty(PropertyLabel label, PropertyUnit unit, float value);

        private:
            void initProperties();

        protected:
            sf::Vector2f _size;
            sf::Vector2f _position;
            sf::Vector2f _requestedSize;
            sf::Vector2f _requestedPosition;

        private:
            int _zIndex;
            std::string _id;
            sp::IDynamicComponent *_parent;
            sf::Vector2f _rootWindowSize;
            bool _rootComponent;
            std::map<PropertyLabel, PropertySettings> _properties;
    };
}