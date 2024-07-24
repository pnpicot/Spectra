#pragma once

#include "../engine/engine.hpp"
#include "SFML/Graphics.hpp"

namespace sp {
    class Engine;

    class EventHandler {
        public:
            enum EventType {
                NONE,
                CLOSED,
                KEYPRESSED,
                KEYRELEASED,
                RESIZED
            };

            struct Event {
                EventType type;
                sf::Keyboard::Key key;
                sf::Vector2f size;
            };

        public:
            EventHandler(sp::Engine *engine);
            ~EventHandler() = default;

        public:
            void registerEvent(const sf::Event &event);
            void setCallback(void (*callback)(const Event &event, sp::Engine *app));

        private:
            void handleResize(sf::Vector2f size);

        private:
            sp::Engine *_engine;
            void (*_callback)(const Event &event, sp::Engine *app);
    };
}