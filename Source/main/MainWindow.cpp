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
    setResizable (true, false);
    // setResizeLimits (300, 300, 10000, 10000);

    #if defined(OS_MACIOS) || defined(OS_ANDROID)
    setFullScreen (true);
    #endif

    startTimer(1);
}

MainWindow::~MainWindow()
{
    contentComponent = NULL;
}

void MainWindow::timerCallback()
{
    stopTimer();

    createUI();
}

ContentComponent* MainWindow::getContentComponent()
{
    return contentComponent;
}

void MainWindow::createUI()
{
    setUsingNativeTitleBar(true);

    Desktop::getInstance().setOrientationsEnabled(Desktop::allOrientations);

    contentComponent = new ContentComponent();

#if defined(OS_MACIOS) || defined(OS_ANDROID)
    Rectangle<int> monitorArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;

    //std::cerr << "Monitor area (" << monitorArea.getWidth() << ", " << monitorArea.getHeight() << ")" << std::endl; 

    centreWithSize(monitorArea.getWidth(), 
                   monitorArea.getHeight());
    contentComponent->setBounds(0,
                                0.5 * StatusBarHeight,
                                monitorArea.getWidth(), 
                                monitorArea.getHeight() - StatusBarHeight
                                );
    setMenuBar(NULL);
#else
    centreWithSize(480, 320);
    contentComponent->setBounds(0, 0, 480, 320);
#endif

    setContentNonOwned(contentComponent, true);

    setVisible(true);
}

bool MainWindow::isPortraitOrientation()
{
     Desktop::DisplayOrientation displayOrientation = Desktop::getInstance().getCurrentOrientation();

    if(displayOrientation == Desktop::upright || displayOrientation == Desktop::upsideDown)
        return true;
    else
        return false;
}

void MainWindow::resized()
{
    // We have to check if we're already resizing the window, because we get called here
    // when the iPad is rotated, and we have to resize the window to fit the rotation
    // from this method. This can cause an infinite loop...
    if (resizingWindow)
        return;

    resizingWindow = true;

    Rectangle<int> monitorArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;

#if defined(OS_MACIOS) || defined(OS_ANDROID)
    if (getWidth() != monitorArea.getWidth() || getHeight() != monitorArea.getHeight())
    {
        centreWithSize(monitorArea.getWidth(), 
                       monitorArea.getHeight());
    }
#else

    bool portraitOrientation = isPortraitOrientation();
    if (portraitOrientation)
    {
        if (getWidth() != 320 || getHeight() != (480-StatusBarHeight))
            centreWithSize(320, 480-StatusBarHeight);
    }
    else
    {
        if (getWidth() != 480 || getHeight() != 320)
            centreWithSize(480, 320);
    }
#endif
    
    if (contentComponent != NULL) 
    {
        // I don't know why offsetting the y position by half the status bar's height rather than its entire height
        // causes the content component to properly fill the available screen area.
#if defined(OS_MACIOS) || defined(OS_ANDROID)
        contentComponent->setBounds(  0,
                                      0.5 * StatusBarHeight,
                                      monitorArea.getWidth(), 
                                      monitorArea.getHeight() - StatusBarHeight);
#else
        if (portraitOrientation)
            contentComponent->setBounds(0, 0, 320, 480-StatusBarHeight);
        else
            contentComponent->setBounds(0, 0, 480, 320);
#endif
    }

    resizingWindow = false;
}

void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
