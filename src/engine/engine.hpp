#pragma once

#include "../logger/logger.hpp"
#include "../event/event.hpp"
#include "../container/container.hpp"
#include "../helpers/componentHelper.hpp"
#include "../component/ADynamicComponent.hpp"
#include "../layout/layoutManager.hpp"
#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

#define DEFAULT_FRAMERATE 144
#define DEFAULT_RESOLUTION { 1080, 1920 }
#define BASE_CONTAINER_ID "#reserved.baseContainer"

namespace sp {
    class EventHandler;

    class Engine {
        friend class EventHandler;

        public:
            Engine();
            Engine(int width, int height);
            ~Engine();

        public:
            void init();
            void run();
            void close();
            std::shared_ptr<sp::EventHandler> getEventHandler();
            const sf::Vector2i &getResolution() const;
            std::shared_ptr<sf::RenderWindow> &getWindow();

        public:
            std::shared_ptr<sp::Container> createContainer(const std::string &containerId, bool fitWindow = true);
            std::shared_ptr<sp::Container> getContainer(const std::string &containerId);
            void setContainerIndex(const std::string &containerId, int zIndex);

        protected:
            void updateLayout();
            void updateWindowSize(sf::Vector2f newSize);

        private:
            void defaultInit();
            void update();
            void render();
            void createBaseContainer();
            void sortContainers();
            void renderContainers();
            std::shared_ptr<sp::Container> createContainerSafe(const std::string &containerId, bool fitWindow = true);

        private:
            sf::Vector2i _resolution;
            std::shared_ptr<sf::RenderWindow> _window;
            std::shared_ptr<sp::EventHandler> _eventHandler;
            sf::Clock _clock;
            double _targetFramerate;
            std::vector<std::shared_ptr<sp::IDynamicComponent>> _containers;
            sf::Vector2f _windowSize;
    };
}