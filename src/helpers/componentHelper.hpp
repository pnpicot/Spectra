#pragma once

#include "../component/container/container.hpp"
#include "../logger/logger.hpp"
#include "../component/ADynamicComponent.hpp"

namespace sp {
    class Container;

    class ComponentHelper {
        public:
            static std::shared_ptr<sp::Container> createContainer(const std::string &id, sf::Vector2i size, sf::Vector2i resolution);
            static void sortComponents(std::vector<std::shared_ptr<sp::IDynamicComponent>> &list);
    };
}