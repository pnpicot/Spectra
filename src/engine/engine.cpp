#include "engine.hpp"

namespace sp {
    Engine::Engine()
    {
        _resolution = DEFAULT_RESOLUTION;

        defaultInit();
    }

    Engine::Engine(int width, int height)
    {
        _resolution = { width, height };

        defaultInit();
    }

    Engine::~Engine()
    {
        // ...
    }

    void Engine::defaultInit()
    {
        _targetFramerate = DEFAULT_FRAMERATE;
        _window = std::make_shared<sf::RenderWindow>();
        _eventHandler = std::make_shared<sp::EventHandler>(this);
        _windowSize = { (float) _resolution.x, (float) _resolution.y };

        createBaseContainer();
    }

    void Engine::init()
    {
        sf::VideoMode videoMode(_resolution.x, _resolution.y);
        sf::ContextSettings ctxSettings;

        ctxSettings.antialiasingLevel = 4;

        _window->create(
            videoMode,
            "Window",
            sf::Style::Default,
            ctxSettings
        );

        _window->setFramerateLimit(DEFAULT_FRAMERATE);
    }

    void Engine::run()
    {
        if (!_window) {
            Logger::log(Logger::ERROR, "Cannot run engine before initialization");
            return;
        }

        updateLayout();
        _clock.restart();

        while (_window->isOpen()) {
            sf::Event event;

            while (_window->pollEvent(event)) {
                _eventHandler->registerEvent(event);
            }

            double elapsedSeconds = _clock.getElapsedTime().asSeconds();
            double targetInterval = 1.0f / _targetFramerate;

            if (elapsedSeconds >= targetInterval) {
                update();
                render();
                _clock.restart();
            }
        }
    }

    void Engine::close()
    {
        _window->close();
    }

    void Engine::update()
    {
        // ...
    }

    void Engine::render()
    {
        _window->clear(sf::Color::Black);
        renderContainers();
        _window->display();
    }

    std::shared_ptr<sp::EventHandler> Engine::getEventHandler()
    {
        return _eventHandler;
    }

    std::shared_ptr<sp::Container> Engine::createContainer(const std::string &containerId, bool fitWindow)
    {
        if (containerId.rfind("#reserved.", 0) == 0) {
            Logger::log(Logger::WARNING, "Attempted to create container with reserved id");
            return nullptr;
        }

        return createContainerSafe(containerId, fitWindow);
    }

    std::shared_ptr<sp::Container> Engine::createContainerSafe(const std::string &containerId, bool fitWindow)
    {
        std::shared_ptr<sp::Container> newContainer = std::make_shared<sp::Container>(containerId);

        if (!newContainer) {
            Logger::log(Logger::WARNING, "Could not allocate memory for new container");
            return nullptr;
        }

        if (!newContainer->create(_resolution.x * 2, _resolution.y * 2))
            return nullptr;

        if (fitWindow)
            newContainer->resize(static_cast<sf::Vector2f>(_resolution));

        newContainer->setRootComponent(true);
        newContainer->setRootWindowSize(_windowSize);
        _containers.emplace_back(newContainer);
        sortContainers();

        return newContainer;
    }

    void Engine::createBaseContainer()
    {
        std::shared_ptr<sp::Container> baseContainer = createContainerSafe(BASE_CONTAINER_ID, true);

        if (!baseContainer)
            return;

        baseContainer->setBackgroundColor(sf::Color::Black);
        baseContainer->setZIndex(-9999);
        baseContainer->resize({ 100.0f, 100.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    }

    void Engine::sortContainers()
    {
        ComponentHelper::sortComponents(_containers);
    }

    void Engine::renderContainers()
    {
        for (auto &container : _containers) {
            container->render(_window);
        }
    }

    std::shared_ptr<sp::Container> Engine::getContainer(const std::string &containerId)
    {
        for (auto &container : _containers) {
            if (container->getId() == containerId)
                return std::dynamic_pointer_cast<sp::Container>(container);
        }

        return nullptr;
    }

    void Engine::setContainerIndex(const std::string &containerId, int zIndex)
    {
        std::shared_ptr<sp::Container> container = getContainer(containerId);

        if (container == nullptr) {
            Logger::log(Logger::WARNING, "Could not set container index (unknown id)");
            return;
        }

        bool changed = container->getZIndex() != zIndex;

        container->setZIndex(zIndex);

        if (changed)
            sortContainers();
    }

    const sf::Vector2i &Engine::getResolution() const
    {
        return _resolution;
    }

    std::shared_ptr<sf::RenderWindow> &Engine::getWindow()
    {
        return _window;
    }
    
    void Engine::updateLayout()
    {
        for (auto &container : _containers) {
            LayoutManager::updateDynamicComponent(container, true);
        }

        for (auto &container : _containers) {
            LayoutManager::updateComponentChildren(container, true);
        }
    }

    void Engine::updateWindowSize(sf::Vector2f newSize)
    {
        _windowSize = newSize;

        for (auto &container : _containers) {
            container->setRootWindowSize(_windowSize);
        }
    }
}