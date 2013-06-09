#include "iOSContentComponent.h"

#include "TriangleComponent.h"
#include "SquareComponent.h"
#include "TexturedComponent.h"
#include "TexturedCubeComponent.h"
#include "TextToTextureComponent.h"
#include "TextWithFreetypeFontAtlasComponent.h"
#include "TextWithJuceFontAtlasComponent.h"

iOSContentComponent::iOSContentComponent()
: activeMode( (ApplicationMode::ID) -1)
  , activeScreen()
{
    addAndMakeVisible(toolbar = new iOSModeSelectorToolbar());
    toolbar->addListener(this);

    modeSelected(ApplicationMode::Triangle);
}

iOSContentComponent::~iOSContentComponent()
{
}

void iOSContentComponent::modeSelected(ApplicationMode::ID mode)
{
    if (mode == activeMode)
        return;

    activeMode = mode;

    removeChildComponent(activeScreen);

    switch (mode)
    {
    case ApplicationMode::Triangle:
        activeScreen = new TriangleComponent();
        break;

    case ApplicationMode::Square:
        activeScreen = new SquareComponent();
        break;

    case ApplicationMode::Texture:
        activeScreen = new TexturedComponent();
        break;

    case ApplicationMode::SlowText:
        activeScreen = new TextToTextureComponent();
        break;

    case ApplicationMode::FastText:
        activeScreen = new TextWithJuceFontAtlasComponent();
        break;

    case ApplicationMode::Cube:
        activeScreen = new TexturedCubeComponent();
        break;

    default:
        break;
    }

    addAndMakeVisible(activeScreen);
    resized();
}

void iOSContentComponent::resized()
{
    bool isToolbarVisible = (toolbar != NULL && toolbar->isVisible());

    if (activeScreen != NULL)
    {
        int width = getWidth();
        int height = getHeight();
        if (isToolbarVisible)
            height -= iOSModeSelectorToolbar::ToolbarHeight;
        int x = 0;
        int y = 0;
        activeScreen->setBounds(x, y, width, height);
    }

    if (isToolbarVisible)
    {
        int width = getWidth();
        int height = iOSModeSelectorToolbar::ToolbarHeight;
        int x = 0;
        int y = getHeight() - height;
        toolbar->setBounds(x, y, width, height);
    }
}

void iOSContentComponent::paint(Graphics& g)
{
    ColourGradient gradient(Colour(0xffe6e6e6), 0, 0, Colour(0xffeaeaea), 0, getWidth(), false);
    gradient.addColour(0.5f, Colour(0xffd5d5d5));
    g.setGradientFill(gradient);
    g.fillAll();
}


