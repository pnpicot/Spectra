#pragma once

#include "../component/ADynamicComponent.hpp"
#include "../container/container.hpp"

namespace sp {
    class LayoutManager {
        public:
            static void updateDynamicComponent(std::shared_ptr<sp::IDynamicComponent> component, bool recursive = false);
            static void updateComponentChildren(std::shared_ptr<sp::IDynamicComponent> component, bool recursive);

        private:
            static sf::FloatRect getComponentParentRect(std::shared_ptr<sp::IDynamicComponent> component);
            static void updateComponentPosition(std::shared_ptr<sp::IDynamicComponent> component);
            static void updateComponentSize(std::shared_ptr<sp::IDynamicComponent> component);
            static void updateComponentPositionOffsets(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f &finalPosition, const sf::FloatRect &parentRect);
            static float computeSettingsUnitValue(PropertySettings settings, float parentValue, float current);
            static void rearrangeComponentChildren(std::shared_ptr<sp::Container> component);
            static void automaticChildrenOrdering(std::shared_ptr<sp::Container> container);
            static void updateComponentTranslation(std::shared_ptr<sp::IDynamicComponent> component);
            static sf::Vector2f getComponentAvailableSize(sp::IDynamicComponent *component);
            static sf::Vector2f getComponentAvailableSize(std::shared_ptr<sp::IDynamicComponent> component);
            static sf::FloatRect getComponentMargin(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f parentSize);
            static sf::FloatRect getComponentPadding(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f parentSize);
            static sf::FloatRect calculateChildBounds(std::shared_ptr<sp::IDynamicComponent> child, const sf::Vector2f &parentSize);
            static void positionChildInRow(std::shared_ptr<sp::IDynamicComponent> child, sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, float &currentX, float &currentY, float &maxHeight, const sf::FloatRect &margin);
            static void positionChildInColumn(std::shared_ptr<sp::IDynamicComponent> child, sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, float &currentX, float &currentY, float &maxWidth, const sf::FloatRect &margin);
    };
}