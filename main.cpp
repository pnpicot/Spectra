#include "src/engine/engine.hpp"
#include "src/helpers/componentHelper.hpp"
#include "src/helpers/randomHelper.hpp"
#include "src/component/rectangle/rectangle.hpp"
#include "src/helpers/debugHelper.hpp"

void eventHandler(const sp::EventHandler::Event &event, sp::Engine *app)
{
    if (event.type == sp::EventHandler::KEYRELEASED && event.key == sf::Keyboard::Escape) {
        app->close();
        return;
    }
}

void testComponents(std::unique_ptr<sp::Engine> &app)
{
    using namespace sp;

    sf::Vector2i resolution = app->getResolution();
    std::shared_ptr<Container> mainContainer = app->getContainer(BASE_CONTAINER_ID);
    std::shared_ptr<Container> subContainer = ComponentHelper::createContainer("sub-container", resolution, resolution * 2);

    subContainer->resize({ 80.0f, 90.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    subContainer->setBackgroundColor(sf::Color(40, 40, 40));
    subContainer->setLayoutMode(LayoutMode::AUTO);
    subContainer->setPadding(20.0f);
    subContainer->setMargin({ 50.0f, 50.0f, 0, 0 }, { PropertyUnit::PERC, PropertyUnit::PERC, PropertyUnit::PX, PropertyUnit::PX });
    subContainer->setTranslation({ -50.0f, -50.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    subContainer->setLayoutDirection(LayoutDirection::ROW);
    subContainer->setHorizontalAlign(AlignMode::SPACE_AROUND);
    subContainer->setVerticalAlign(AlignMode::SPACE_AROUND);

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (x == 5 && y == 18) {
                std::shared_ptr<Rectangle> newRectangle = std::make_shared<Rectangle>("rectangle" + std::to_string(x) + std::to_string(y));

                newRectangle->resize({ 5.0f, 5.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });

                newRectangle->setBackgroundColor(sf::Color(
                    RandomHelper::randInt(100, 255),
                    RandomHelper::randInt(100, 255),
                    RandomHelper::randInt(100, 255),
                    200
                ));

                newRectangle->setBorderRadius(25.0f);

                subContainer->appendComponent(newRectangle);

                continue;
            }

            std::shared_ptr<Container> newContainer = ComponentHelper::createContainer(
                "container-" + std::to_string(x) + std::to_string(y),
                { 50, 50 },
                { 50, 50 }
            );

            newContainer->resize({ 5.0f, 5.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });

            newContainer->setBackgroundColor(sf::Color(
                RandomHelper::randInt(100, 255),
                RandomHelper::randInt(100, 255),
                RandomHelper::randInt(100, 255),
                200
            ));

            subContainer->appendComponent(newContainer);
        }
    }

    mainContainer->appendComponent(subContainer);
}

int main(int argc, char **argv)
{
    srand(time(nullptr));

    std::unique_ptr<sp::Engine> app = std::make_unique<sp::Engine>(1920, 1080);

    app->init();
    app->getEventHandler()->setCallback(&eventHandler);
    testComponents(app);
    app->run();

    return 0;
}