#include "componentHelper.hpp"

namespace sp {
    std::shared_ptr<sp::Container> ComponentHelper::createContainer(const std::string &id, sf::Vector2i size, sf::Vector2i resolution)
    {
        std::shared_ptr<sp::Container> container = std::make_shared<sp::Container>(id);

        if (!container || !container->create(resolution.x, resolution.y)) {
            Logger::log(Logger::WARNING, "Could not allocate memory for new container");
            return nullptr;
        }

        container->resize(static_cast<sf::Vector2f>(size));

        return container;
    }

    void ComponentHelper::sortComponents(std::vector<std::shared_ptr<sp::IDynamicComponent>> &list)
    {
        std::sort(
            list.begin(),
            list.end(),
            [](
                const std::shared_ptr<sp::IDynamicComponent> &a,
                const std::shared_ptr<sp::IDynamicComponent> &b
            ) {
            return a->getZIndex() < b->getZIndex();
        });
    }
}