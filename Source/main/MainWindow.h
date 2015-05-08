/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#ifndef __MAINWINDOW_H_207F95EE__
#define __MAINWINDOW_H_207F95EE__

#include "JuceHeader.h"

class ContentComponent;

//==============================================================================
class MainWindow : public DocumentWindow
{
public:
    //==============================================================================
    MainWindow();
    ~MainWindow();

    ContentComponent* getContentComponent();

    void closeButtonPressed();

private:
    SafePointer<ContentComponent> contentComponent;

    bool resizingWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};


#endif  // __MAINWINDOW_H_207F95EE__
