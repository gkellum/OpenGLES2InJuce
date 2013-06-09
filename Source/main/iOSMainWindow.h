/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#ifndef __MAINWINDOW_H_207F95EE__
#define __MAINWINDOW_H_207F95EE__

#include "JuceHeader.h"

//==============================================================================
class iOSMainWindow : public DocumentWindow
{
public:
    //==============================================================================
    iOSMainWindow();
    ~iOSMainWindow();

    void resized();

    void closeButtonPressed();

    Component* getContentComponent();

    /* Note: Be careful when overriding DocumentWindow methods - the base class
       uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in you content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       implementation calls the superclass's method.
    */

    static const int StatusBarHeight = 20;

    static bool isPortraitOrientation();

private:
    void createUI();

    ScopedPointer<Component> contentComponent;

    bool resizingWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (iOSMainWindow)
};


#endif  // __MAINWINDOW_H_207F95EE__
