#pragma once

#include "../component/ADynamicComponent.hpp"
#include "../component/container/container.hpp"
#include <numeric>

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
            static void positionChildInRow(float &currentX, float &currentY, const sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, const sf::FloatRect &margin, float &maxHeight, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, std::vector<std::shared_ptr<sp::IDynamicComponent>> &currentRow, std::shared_ptr<sp::IDynamicComponent> child);
            static void positionChildInColumn(float &currentX, float &currentY, const sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, const sf::FloatRect &margin, float &maxWidth, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, std::vector<std::shared_ptr<sp::IDynamicComponent>> &currentRow, std::shared_ptr<sp::IDynamicComponent> child);
            static void applyGridAlignment(LayoutDirection direction, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const sf::Vector2f &parentSize, AlignMode horizontalAlign, AlignMode verticalAlign);
            static void getDimensionsForGridAlignment(bool isRow, std::vector<float> &totalWidths, std::vector<float> &totalHeights, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const sf::Vector2f &parentSize);
            static void applyHorizontalGridAlignment(bool isRow, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const std::vector<float> &totalWidths, const sf::Vector2f &parentSize, AlignMode horizontalAlign);
            static void applyVerticalGridAlignment(bool isRow, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const std::vector<float> &totalHeights, const sf::Vector2f &parentSize, AlignMode verticalAlign);
    };
}