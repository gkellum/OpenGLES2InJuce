#ifndef __iOSModeSelectorToolbar_H__
#define __iOSModeSelectorToolbar_H__

#include "JuceHeader.h"

#include "ApplicationMode.h"

#include <vector>

class iOSModeSelectorToolbar : public Component
                                  , public Button::Listener
{ 
public:
    iOSModeSelectorToolbar();
    ~iOSModeSelectorToolbar();
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void modeSelected(ApplicationMode::ID mode) =0;
    };

    void addListener(Listener* listener) { modeListeners.add(listener); }
    void removeListener(Listener* listener) { modeListeners.remove(listener); }

    void modeSelected(ApplicationMode::ID mode);
    
    void buttonClicked(Button* button);
    
    void resized();
    
    void paint(Graphics& g);

    static const int ToolbarHeight = 50;

private:
    ListenerList<Listener> modeListeners;

    class ReferenceCountedTextButton : public TextButton
                                     , public SingleThreadedReferenceCountedObject
    {
    public:
        typedef ReferenceCountedObjectPtr<ReferenceCountedTextButton> Ptr;

        ReferenceCountedTextButton(const String &buttonName)
          : TextButton(buttonName) {}
    };

    std::vector< ReferenceCountedTextButton::Ptr > buttons;
};

#endif