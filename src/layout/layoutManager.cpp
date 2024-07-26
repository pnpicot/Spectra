#include "layoutManager.hpp"
#include <iostream>

namespace sp {
    void LayoutManager::updateDynamicComponent(std::shared_ptr<sp::IDynamicComponent> component, bool recursive)
    {
        updateComponentPosition(component);
        updateComponentSize(component);
        updateComponentTranslation(component);

        if (!recursive)
            return;

        if (auto container = std::dynamic_pointer_cast<sp::Container>(component)) {
            for (auto &child : container->_children) {
                updateDynamicComponent(child, true);
            }
        }
    }

    void LayoutManager::updateComponentChildren(std::shared_ptr<sp::IDynamicComponent> component, bool recursive)
    {
        std::shared_ptr<sp::Container> container = std::dynamic_pointer_cast<sp::Container>(component);

        if (container == nullptr || container->_children.empty())
            return;

        rearrangeComponentChildren(container);

        if (!recursive)
            return;

        for (auto &child : container->_children) {
            updateComponentChildren(child, true);
        }
    }

    void LayoutManager::rearrangeComponentChildren(std::shared_ptr<sp::Container> component)
    {
        LayoutMode layoutMode = component->getPropertySettings(PropertyLabel::LAYOUT_MODE).layoutMode;

        switch (layoutMode) {
            case LayoutMode::AUTO:
                automaticChildrenOrdering(component);
                break;
            default:
                break;
        }
    }

    sf::FloatRect LayoutManager::getComponentMargin(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f parentSize)
    {
        PropertySettings leftMarginSettings = component->getPropertySettings(PropertyLabel::MARGIN_LEFT);
        float leftMargin = computeSettingsUnitValue(leftMarginSettings, parentSize.x, leftMarginSettings.value);

        PropertySettings topMarginSettings = component->getPropertySettings(PropertyLabel::MARGIN_TOP);
        float topMargin = computeSettingsUnitValue(topMarginSettings, parentSize.y, topMarginSettings.value);

        PropertySettings rightMarginSettings = component->getPropertySettings(PropertyLabel::MARGIN_RIGHT);
        float rightMargin = computeSettingsUnitValue(rightMarginSettings, parentSize.x, rightMarginSettings.value);

        PropertySettings bottomMarginSettings = component->getPropertySettings(PropertyLabel::MARGIN_BOTTOM);
        float bottomMargin = computeSettingsUnitValue(bottomMarginSettings, parentSize.y, bottomMarginSettings.value);

        return { leftMargin, topMargin, rightMargin, bottomMargin };
    }

    sf::FloatRect LayoutManager::getComponentPadding(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f parentSize)
    {
        PropertySettings leftPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_LEFT);
        float leftPadding = computeSettingsUnitValue(leftPaddingSettings, parentSize.x, leftPaddingSettings.value);

        PropertySettings topPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_TOP);
        float topPadding = computeSettingsUnitValue(topPaddingSettings, parentSize.y, topPaddingSettings.value);

        PropertySettings rightPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_RIGHT);
        float rightPadding = computeSettingsUnitValue(rightPaddingSettings, parentSize.x, rightPaddingSettings.value);

        PropertySettings bottomPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_BOTTOM);
        float bottomPadding = computeSettingsUnitValue(bottomPaddingSettings, parentSize.y, bottomPaddingSettings.value);

        return { leftPadding, topPadding, rightPadding, bottomPadding };
    }

    sf::FloatRect LayoutManager::calculateChildBounds(std::shared_ptr<sp::IDynamicComponent> child, const sf::Vector2f &parentSize)
    {
        sf::FloatRect bounds = child->getRect();
        sf::FloatRect margin = getComponentMargin(child, parentSize);

        bounds.width += margin.left + margin.width;
        bounds.height += margin.top + margin.height;

        return bounds;
    }

    void LayoutManager::positionChildInRow(float &currentX, float &currentY, const sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, const sf::FloatRect &margin, float &maxHeight, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, std::vector<std::shared_ptr<sp::IDynamicComponent>> &currentRow, std::shared_ptr<sp::IDynamicComponent> child)
    {
        if (currentX + bounds.width > parentSize.x + parentPadding.left) {
            currentX = parentPadding.left + margin.left;
            currentY += maxHeight;
            maxHeight = 0;
            grid.emplace_back(currentRow);
            currentRow.clear();
        }

        child->setAbsolutePosition({ currentX, currentY });
        currentRow.emplace_back(child);

        currentX += bounds.width;

        if (bounds.height > maxHeight)
            maxHeight = bounds.height;
    }

    void LayoutManager::positionChildInColumn(float &currentX, float &currentY, const sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, const sf::FloatRect &margin, float &maxWidth, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, std::vector<std::shared_ptr<sp::IDynamicComponent>> &currentRow, std::shared_ptr<sp::IDynamicComponent> child)
    {
        if (currentY + bounds.height > parentSize.y + parentPadding.top) {
            currentY = parentPadding.top + margin.top;
            currentX += maxWidth;
            maxWidth = 0;
            grid.emplace_back(currentRow);
            currentRow.clear();
        }

        child->setAbsolutePosition({ currentX, currentY });
        currentRow.emplace_back(child);

        currentY += bounds.height;

        if (bounds.width > maxWidth)
            maxWidth = bounds.width;
    }

    void LayoutManager::getDimensionsForGridAlignment(bool isRow, std::vector<float> &totalWidths, std::vector<float> &totalHeights, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const sf::Vector2f &parentSize)
    {
        if (isRow) {
            totalWidths.reserve(grid.size());
            totalHeights.push_back(0);

            for (std::size_t i = 0; i < grid.size(); ++i) {
                totalWidths.push_back(0);
            }
        } else {
            totalWidths.push_back(0);
            totalHeights.reserve(grid.size());

            for (std::size_t i = 0; i < grid.size(); ++i) {
                totalHeights.push_back(0);
            }
        }

        unsigned int y = 0;
        std::vector<float> maxHeightIte;
        std::vector<float> maxWidthIte;

        for (auto &row : grid) {
            float maxColumnWidth = 0;
            float maxRowHeight = 0;

            for (auto &child : row) {
                sf::FloatRect bounds = calculateChildBounds(child, parentSize);

                if (isRow) {
                    totalWidths[y] += bounds.width;
                    maxRowHeight = std::max(maxRowHeight, bounds.height);
                } else {
                    totalHeights[y] += bounds.height;
                    maxColumnWidth = std::max(maxColumnWidth, bounds.width);
                }
            }

            maxWidthIte.push_back(maxColumnWidth);
            maxHeightIte.push_back(maxRowHeight);
            y++;
        }

        if (isRow) {
            totalHeights[0] = std::accumulate(maxHeightIte.begin(), maxHeightIte.end(), 0.0f);
        } else {
            totalWidths[0] = std::accumulate(maxWidthIte.begin(), maxWidthIte.end(), 0.0f);
        }
    }

    void LayoutManager::applyHorizontalGridAlignment(bool isRow, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const std::vector<float> &totalWidths, const sf::Vector2f &parentSize, AlignMode horizontalAlign)
    {
        if (isRow) {
            for (std::size_t rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
                auto &row = grid[rowIndex];
                float totalWidth = totalWidths[rowIndex];
                float extraSpace = parentSize.x - totalWidth;
                float offset = 0;
                float currentOffset = 0;

                switch (horizontalAlign) {
                    case AlignMode::SPACE_AROUND:
                        offset = extraSpace / (row.size() * 2);
                        currentOffset = offset;
                        break;
                    case AlignMode::SPACE_EVENLY:
                        offset = extraSpace / (row.size() + 1);
                        currentOffset = offset;
                        break;
                    case AlignMode::SPACE_BETWEEN:
                        offset = extraSpace / (row.size() - 1);
                        break;
                    default:
                        break;
                }

                for (auto &child : row) {
                    sf::Vector2f position = child->getPosition();

                    position.x += currentOffset;
                    child->setAbsolutePosition(position);
                    currentOffset += horizontalAlign == AlignMode::SPACE_AROUND ? offset * 2 : offset;
                }
            }
        } else {
            float totalWidth = totalWidths[0];
            float extraSpace = parentSize.x - totalWidth;
            float offset = 0;
            float currentOffset = 0;

            switch (horizontalAlign) {
                case AlignMode::SPACE_AROUND:
                    offset = extraSpace / (grid.size() * 2);
                    currentOffset = offset;
                    break;
                case AlignMode::SPACE_EVENLY:
                    offset = extraSpace / (grid.size() + 1);
                    currentOffset = offset;
                    break;
                case AlignMode::SPACE_BETWEEN:
                    offset = extraSpace / (grid.size() - 1);
                    break;
                default:
                    break;
            }

            for (auto &row : grid) {
                for (auto &child : row) {
                    sf::Vector2f position = child->getPosition();

                    position.x += currentOffset;
                    child->setAbsolutePosition(position);
                }

                currentOffset += horizontalAlign == AlignMode::SPACE_AROUND ? offset * 2 : offset;
            }
        }
    }

    void LayoutManager::applyVerticalGridAlignment(bool isRow, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const std::vector<float> &totalHeights, const sf::Vector2f &parentSize, AlignMode verticalAlign)
    {
        if (isRow) {
            float totalHeight = totalHeights[0];
            float extraSpace = parentSize.y - totalHeight;
            float offset = 0;
            float currentOffset = 0;

            switch (verticalAlign) {
                case AlignMode::SPACE_AROUND:
                    offset = extraSpace / (grid.size() * 2);
                    currentOffset = offset;
                    break;
                case AlignMode::SPACE_EVENLY:
                    offset = extraSpace / (grid.size() + 1);
                    currentOffset = offset;
                    break;
                case AlignMode::SPACE_BETWEEN:
                    offset = extraSpace / (grid.size() - 1);
                    break;
                default:
                    break;
            }

            for (auto &row : grid) {
                for (auto &child : row) {
                    sf::Vector2f position = child->getPosition();

                    position.y += currentOffset;
                    child->setAbsolutePosition(position);
                }

                currentOffset += verticalAlign == AlignMode::SPACE_AROUND ? offset * 2 : offset;
            }
        } else {
            for (std::size_t rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
                auto &column = grid[rowIndex];
                float totalHeight = totalHeights[rowIndex];
                float extraSpace = parentSize.y - totalHeight;
                float offset = 0;
                float currentOffset = 0;

                switch (verticalAlign) {
                    case AlignMode::SPACE_AROUND:
                        offset = extraSpace / (column.size() * 2);
                        currentOffset = offset;
                        break;
                    case AlignMode::SPACE_EVENLY:
                        offset = extraSpace / (column.size() + 1);
                        currentOffset = offset;
                        break;
                    case AlignMode::SPACE_BETWEEN:
                        offset = extraSpace / (column.size() - 1);
                        break;
                    default:
                        break;
                }

                for (auto &child : column) {
                    sf::Vector2f position = child->getPosition();

                    position.y += currentOffset;
                    child->setAbsolutePosition(position);
                    currentOffset += verticalAlign == AlignMode::SPACE_AROUND ? offset * 2 : offset;
                }
            }
        }
    }

    void LayoutManager::applyGridAlignment(LayoutDirection direction, std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> &grid, const sf::Vector2f &parentSize, AlignMode horizontalAlign, AlignMode verticalAlign)
    {
        bool isRow = direction == LayoutDirection::ROW;
        std::vector<float> totalWidths;
        std::vector<float> totalHeights;

        getDimensionsForGridAlignment(isRow, totalWidths, totalHeights, grid, parentSize);
        applyHorizontalGridAlignment(isRow, grid, totalWidths, parentSize, horizontalAlign);
        applyVerticalGridAlignment(isRow, grid, totalHeights, parentSize, verticalAlign);
    }

    void LayoutManager::automaticChildrenOrdering(std::shared_ptr<sp::Container> container)
    {
        if (container->_children.size() < 2)
            return;

        sf::Vector2f parentSize = getComponentAvailableSize(container);
        LayoutDirection direction = static_cast<LayoutDirection>(container->getPropertySettings(PropertyLabel::LAYOUT_DIRECTION).value);
        sf::FloatRect parentPadding = getComponentPadding(container, container->getSize());
        AlignMode horizontalAlign = static_cast<AlignMode>(container->getPropertySettings(PropertyLabel::HORIZONTAL_ALIGN).value);
        AlignMode verticalAlign = static_cast<AlignMode>(container->getPropertySettings(PropertyLabel::VERTICAL_ALIGN).value);

        float baseX = container->_children[0]->getPosition().x;
        float baseY = container->_children[0]->getPosition().y;
        float currentX = baseX;
        float currentY = baseY;
        float maxHeight = 0;
        float maxWidth = 0;

        std::vector<std::vector<std::shared_ptr<sp::IDynamicComponent>>> grid;
        std::vector<std::shared_ptr<sp::IDynamicComponent>> currentRow;

        for (auto &child : container->_children) {
            sf::FloatRect bounds = calculateChildBounds(child, parentSize);
            sf::FloatRect margin = getComponentMargin(child, parentSize);

            if (direction == LayoutDirection::ROW) {
                positionChildInRow(currentX, currentY, bounds, parentSize, parentPadding, margin, maxHeight, grid, currentRow, child);
            } else {
                positionChildInColumn(currentX, currentY, bounds, parentSize, parentPadding, margin, maxWidth, grid, currentRow, child);
            }
        }

        grid.emplace_back(currentRow);

        if (horizontalAlign == AlignMode::NONE && verticalAlign == AlignMode::NONE)
            return;

        applyGridAlignment(direction, grid, parentSize, horizontalAlign, verticalAlign);
    }

    sf::FloatRect LayoutManager::getComponentParentRect(std::shared_ptr<sp::IDynamicComponent> component)
    {
        if (component->isRootComponent()) {
            sf::Vector2f windowSize = component->getRootWindowSize();
            sf::FloatRect result = { 0, 0, windowSize.x, windowSize.y };

            return result;
        }

        sp::IDynamicComponent *parent = component->getParent();

        if (parent == nullptr)
            return { 0, 0, 0, 0 };

        return parent->getRect();
    }

    float LayoutManager::computeSettingsUnitValue(PropertySettings settings, float parentValue, float current)
    {
        float result = 0;

        switch (settings.unit) {
            case PropertyUnit::PERC:
                result = parentValue * (settings.value / 100.0f);
                break;
            case PropertyUnit::EM:
                result = current * 15.0f;
                break;
            default:
                result = current;
        }

        return result;
    }

    void LayoutManager::updateComponentPosition(std::shared_ptr<sp::IDynamicComponent> component)
    {
        PositionMode positionMode = static_cast<PositionMode>(component->getPropertySettings(PropertyLabel::POSITION).value);
        sf::Vector2f position = component->getRequestedPosition();

        if (positionMode == PositionMode::ABSOLUTE) {
            component->setAbsolutePosition(position);
            return;
        }

        sf::FloatRect parentRect = getComponentParentRect(component);
        sf::Vector2f finalPosition = { 0, 0 };
        PropertySettings leftSettings = component->getPropertySettings(PropertyLabel::LEFT);
        PropertySettings topSettings = component->getPropertySettings(PropertyLabel::TOP);

        finalPosition.x += computeSettingsUnitValue(leftSettings, parentRect.width, position.x);
        finalPosition.y += computeSettingsUnitValue(topSettings, parentRect.height, position.y);

        updateComponentPositionOffsets(component, finalPosition, parentRect);
        component->setAbsolutePosition(finalPosition);
    }

    void LayoutManager::updateComponentPositionOffsets(std::shared_ptr<sp::IDynamicComponent> component, sf::Vector2f &finalPosition, const sf::FloatRect &parentRect)
    {
        sp::IDynamicComponent *parent = component->getParent();
        float parentLeftPadding = 0;
        float parentTopPadding = 0;
        float parentRightPadding = 0;
        float parentBottomPadding = 0;

        if (parent != nullptr) {
            PropertySettings leftPadding = parent->getPropertySettings(PropertyLabel::PADDING_LEFT);
            PropertySettings topPadding = parent->getPropertySettings(PropertyLabel::PADDING_TOP);
            PropertySettings rightPadding = parent->getPropertySettings(PropertyLabel::PADDING_RIGHT);
            PropertySettings bottomPadding = parent->getPropertySettings(PropertyLabel::PADDING_BOTTOM);
            parentLeftPadding = computeSettingsUnitValue(leftPadding, parentRect.width, leftPadding.value);
            parentTopPadding = computeSettingsUnitValue(topPadding, parentRect.height, topPadding.value);
            parentRightPadding = computeSettingsUnitValue(rightPadding, parentRect.width, rightPadding.value);
            parentBottomPadding = computeSettingsUnitValue(bottomPadding, parentRect.height, bottomPadding.value);

            finalPosition.x += parentLeftPadding;
            finalPosition.y += parentTopPadding;
        }

        PropertySettings leftMargin = component->getPropertySettings(PropertyLabel::MARGIN_LEFT);
        PropertySettings topMargin = component->getPropertySettings(PropertyLabel::MARGIN_TOP);

        finalPosition.x += computeSettingsUnitValue(leftMargin, parentRect.width - parentLeftPadding - parentRightPadding, leftMargin.value);
        finalPosition.y += computeSettingsUnitValue(topMargin, parentRect.height - parentTopPadding - parentBottomPadding, topMargin.value);
    }

    sf::Vector2f LayoutManager::getComponentAvailableSize(sp::IDynamicComponent *component)
    {
        if (component == nullptr)
            return { 0, 0 };

        sf::FloatRect componentRect = component->getRect();
        sf::Vector2f availableSpace = { componentRect.width, componentRect.height };

        PropertySettings leftPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_LEFT);
        float leftPadding = computeSettingsUnitValue(leftPaddingSettings, componentRect.width, leftPaddingSettings.value);

        PropertySettings topPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_TOP);
        float topPadding = computeSettingsUnitValue(topPaddingSettings, componentRect.height, topPaddingSettings.value);

        PropertySettings rightPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_RIGHT);
        float rightPadding = computeSettingsUnitValue(rightPaddingSettings, componentRect.width, rightPaddingSettings.value);

        PropertySettings bottomPaddingSettings = component->getPropertySettings(PropertyLabel::PADDING_BOTTOM);
        float bottomPadding = computeSettingsUnitValue(bottomPaddingSettings, componentRect.height, bottomPaddingSettings.value);

        availableSpace.x -= leftPadding + rightPadding;
        availableSpace.y -= topPadding + bottomPadding;

        return availableSpace;
    }

    sf::Vector2f LayoutManager::getComponentAvailableSize(std::shared_ptr<sp::IDynamicComponent> component)
    {
        if (component == nullptr)
            return { 0, 0 };

        sf::FloatRect componentRect = component->getRect();
        sf::Vector2f availableSpace = { componentRect.width, componentRect.height };
        sf::FloatRect padding = getComponentPadding(component, { componentRect.width, componentRect.height });

        availableSpace.x -= padding.left + padding.width;
        availableSpace.y -= padding.top + padding.height;

        return availableSpace;
    }

    void LayoutManager::updateComponentSize(std::shared_ptr<sp::IDynamicComponent> component)
    {
        PositionMode positionMode = static_cast<PositionMode>(component->getPropertySettings(PropertyLabel::POSITION).value);
        sf::Vector2f size = component->getRequestedSize();

        if (positionMode == PositionMode::ABSOLUTE) {
            component->setAbsoluteSize(size);
            return;
        }

        sf::FloatRect parentRect = getComponentParentRect(component);
        sf::Vector2f finalSize;
        PropertySettings widthSettings = component->getPropertySettings(PropertyLabel::WIDTH);
        PropertySettings heightSettings = component->getPropertySettings(PropertyLabel::HEIGHT);
        sp::IDynamicComponent *parent = component->getParent();
        sf::Vector2f availableParentSpace = { parentRect.width, parentRect.height };

        if (parent != nullptr)
            availableParentSpace = getComponentAvailableSize(parent);

        finalSize.x = computeSettingsUnitValue(widthSettings, availableParentSpace.x, size.x);
        finalSize.y = computeSettingsUnitValue(heightSettings, availableParentSpace.y, size.y);

        {
            sf::FloatRect padding = getComponentPadding(component, finalSize);

            finalSize.x += padding.left + padding.width;
            finalSize.y += padding.top + padding.height;
        }

        component->setAbsoluteSize(finalSize);
    }

    void LayoutManager::updateComponentTranslation(std::shared_ptr<sp::IDynamicComponent> component)
    {
        sf::Vector2f position = component->getPosition();
        sf::Vector2f size = component->getSize();
        PropertySettings xTranslation = component->getPropertySettings(PropertyLabel::TRANSLATE_X);
        PropertySettings yTranslation = component->getPropertySettings(PropertyLabel::TRANSLATE_Y);

        position.x += computeSettingsUnitValue(xTranslation, size.x, xTranslation.value);
        position.y += computeSettingsUnitValue(yTranslation, size.y, yTranslation.value);

        component->setAbsolutePosition(position);
    }
}