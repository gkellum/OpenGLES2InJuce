#include "ContentComponent.h"

#include "Settings.h"
#include "TriangleComponent.h"
#include "SquareComponent.h"
#include "PointSpriteComponent.h"
#include "TexturedComponent.h"
#include "TextToTextureComponent.h"
#include "TextWithJuceFontAtlasComponent.h"

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

ContentComponent::ContentComponent()
  : activeMode( (ApplicationMode::ID) -1)
  , activeScreen()
{
    OpenGLContext& openGLContext(Settings::getInstance().getOpenGLContext());
    openGLContext.setComponentPaintingEnabled(true);
    openGLContext.attachTo(*this);

    addAndMakeVisible(toolbar = new ModeSelectorToolbar());
    toolbar->addListener(this);

    modeSelected(ApplicationMode::Triangle);
}

ContentComponent::~ContentComponent()
{
    Settings::getInstance().getOpenGLContext().detach();
}

void ContentComponent::modeSelected(ApplicationMode::ID mode)
{
    if (mode == activeMode)
        return;

    activeMode = mode;

    removeChildComponent(activeScreen);
    activeScreen = NULL;

    switch (mode)
    {
    case ApplicationMode::Triangle:
        activeScreen = new TriangleComponent();
        break;

    case ApplicationMode::Square:
        activeScreen = new SquareComponent();
        break;

    case ApplicationMode::Points:
        activeScreen = new PointSpriteComponent();
        break;

    case ApplicationMode::Texture:
        activeScreen = new TexturedComponent();
        break;

    case ApplicationMode::TextRenderedToImage:
        activeScreen = new TextToTextureComponent();
        break;

    case ApplicationMode::TextRenderedFromFontAtlas:
       activeScreen = new TextWithJuceFontAtlasComponent();
       break;

    default:
        break;
    }

    addAndMakeVisible(activeScreen);
    resized();

    Settings::getInstance().getOpenGLContext().triggerRepaint();
}

void ContentComponent::resized()
{
    bool isToolbarVisible = (toolbar != NULL && toolbar->isVisible());

    if (activeScreen != NULL)
    {
        int width = getWidth();
        int height = getHeight();
        if (isToolbarVisible)
            height -= ModeSelectorToolbar::ToolbarHeight;
        int x = 0;
        int y = 0;
        activeScreen->setBounds(x, y, width, height);
    }

    if (isToolbarVisible)
    {
        int width = getWidth();
        int height = ModeSelectorToolbar::ToolbarHeight;
        int x = 0;
        int y = getHeight() - height;
        toolbar->setBounds(x, y, width, height);
    }
}



