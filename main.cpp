#include "src/engine/engine.hpp"
#include "src/helpers/componentHelper.hpp"
#include "src/helpers/randomHelper.hpp"
#include "src/component/rectangle/rectangle.hpp"
#include "src/component/text/text.hpp"
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
    std::shared_ptr<sp::AssetsManager> assetsManager = app->getAssetsManager();

    subContainer->resize({ 80.0f, 90.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    subContainer->setBackgroundColor(sf::Color(40, 40, 40));
    subContainer->setLayoutMode(LayoutMode::AUTO);
    subContainer->setPadding(20.0f);
    subContainer->setMargin({ 50.0f, 50.0f, 0, 0 }, { PropertyUnit::PERC, PropertyUnit::PERC, PropertyUnit::PX, PropertyUnit::PX });
    subContainer->setTranslation({ -50.0f, -50.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    subContainer->setLayoutDirection(LayoutDirection::ROW);
    subContainer->setHorizontalAlign(AlignMode::SPACE_AROUND);
    subContainer->setVerticalAlign(AlignMode::SPACE_AROUND);

    std::shared_ptr<sp::Text> text = std::make_shared<sp::Text>("mytext");

    assetsManager->loadFont("roboto", "assets/fonts/Roboto-Regular.ttf");
    text->setText("Hello World!");
    text->setFont(assetsManager->getFont("roboto"));
    text->setTextColor(sf::Color::White);
    text->resize({ 50.0f, 50.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    text->setMargin({ 50.0f, 50.0f, 0, 0 }, { PropertyUnit::PERC, PropertyUnit::PERC });
    text->setTranslation({ -50.0f, -50.0f }, { PropertyUnit::PERC, PropertyUnit::PERC });
    text->setFontSize(35);
    text->setBackgroundColor(sf::Color(255, 255, 255, 30));
    text->setHorizontalAlign(TextAlign::CENTER);
    text->setVerticalAlign(TextAlign::CENTER);

    subContainer->appendComponent(text);
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