/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"

#include "ContentComponent.h"

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

//==============================================================================
MainWindow::MainWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
    , resizingWindow(false)
{
    setUsingNativeTitleBar (true);

    int width = 800;
    int height = 600;
    
    #if (OS_MACIOS)
    {
        Rectangle<int> monitorArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
        width = monitorArea.getWidth();
        height = monitorArea.getHeight();
    }
    #endif

    contentComponent = new ContentComponent();
    contentComponent->setSize(width, height);
    setContentOwned(contentComponent, true);
    setResizable(true, true);

    centreWithSize(width, height);
    setVisible(true);
}

MainWindow::~MainWindow()
{
}

ContentComponent* MainWindow::getContentComponent()
{
    return contentComponent;
}

void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
