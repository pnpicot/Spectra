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

        std::shared_ptr<sp::Container> container = std::dynamic_pointer_cast<sp::Container>(component);

        if (container != nullptr) {
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
                // ...
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

    void LayoutManager::positionChildInRow(std::shared_ptr<sp::IDynamicComponent> child, sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, float &currentX, float &currentY, float &maxHeight, const sf::FloatRect &margin)
    {
        if (currentX + bounds.width > parentSize.x + parentPadding.left) {
            currentX = parentPadding.left + margin.left;
            currentY += maxHeight;
            maxHeight = 0;
        }

        child->setAbsolutePosition({ currentX, currentY });

        currentX += bounds.width;

        if (bounds.height > maxHeight)
            maxHeight = bounds.height;
    }

    void LayoutManager::positionChildInColumn(std::shared_ptr<sp::IDynamicComponent> child, sf::FloatRect &bounds, const sf::Vector2f &parentSize, const sf::FloatRect &parentPadding, float &currentX, float &currentY, float &maxWidth, const sf::FloatRect &margin)
    {
        if (currentY + bounds.height > parentSize.y + parentPadding.top) {
            currentY = parentPadding.top + margin.top;
            currentX += maxWidth;
            maxWidth = 0;
        }

        child->setAbsolutePosition({ currentX, currentY });

        currentY += bounds.height;

        if (bounds.width > maxWidth)
            maxWidth = bounds.width;
    }

    void LayoutManager::automaticChildrenOrdering(std::shared_ptr<sp::Container> container)
    {
        if (container->_children.size() < 2)
            return;

        sf::Vector2f parentSize = getComponentAvailableSize(container);
        float baseX = container->_children[0]->getPosition().x;
        float baseY = container->_children[0]->getPosition().y;
        float currentX = baseX;
        float currentY = baseY;
        float maxHeight = 0;
        float maxWidth = 0;
        LayoutDirection direction = static_cast<LayoutDirection>(container->getPropertySettings(PropertyLabel::LAYOUT_DIRECTION).value);
        sf::FloatRect parentPadding = getComponentPadding(container, container->getSize());

        for (auto &child : container->_children) {
            sf::FloatRect bounds = calculateChildBounds(child, parentSize);
            sf::FloatRect margin = getComponentMargin(child, parentSize);

            if (direction == LayoutDirection::ROW) {
                positionChildInRow(child, bounds, parentSize, parentPadding, currentX, currentY, maxHeight, margin);
            } else {
                positionChildInColumn(child, bounds, parentSize, parentPadding, currentX, currentY, maxWidth, margin);
            }
        }
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