#include "event.hpp"

namespace sp {
    EventHandler::EventHandler(sp::Engine *engine) : _engine(engine)
    {
        _callback = nullptr;
    }

    void EventHandler::registerEvent(const sf::Event &event)
    {
        Event callbackEvent;

        switch (event.type) {
            case sf::Event::Closed:
                callbackEvent.type = CLOSED;
                _engine->close();
                break;
            case sf::Event::KeyPressed:
                callbackEvent.type = KEYPRESSED;
                callbackEvent.key = event.key.code;
                break;
            case sf::Event::KeyReleased:
                callbackEvent.type = KEYRELEASED;
                callbackEvent.key = event.key.code;
                break;
            case sf::Event::Resized:
                {
                    sf::Vector2f size = { 
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                    };

                    callbackEvent.type = RESIZED;
                    callbackEvent.size = size;

                    handleResize(size);
                }
                break;
            default:
                callbackEvent.type = NONE;
        }

        if (_callback != nullptr)
            (_callback)(callbackEvent, _engine);
    }

    void EventHandler::setCallback(void (*callback)(const Event &event, sp::Engine *app))
    {
        _callback = callback;
    }

    void EventHandler::handleResize(sf::Vector2f size)
    {
        std::shared_ptr<sf::RenderWindow> &window = _engine->getWindow();
        sf::FloatRect visibleArea(0, 0, size.x, size.y);

        window->setView(sf::View(visibleArea));
        _engine->updateWindowSize(size);
        _engine->updateLayout();
    }
}