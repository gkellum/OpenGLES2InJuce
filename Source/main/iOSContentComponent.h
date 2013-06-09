#ifndef __IPhoneContentComponent_H__
#define __IPhoneContentComponent_H__

#include "JuceHeader.h"

#include "iOSModeSelectorToolbar.h"
#include "ApplicationMode.h"

class iOSContentComponent : public Component
                          , public iOSModeSelectorToolbar::Listener
{
public:
    iOSContentComponent();
    ~iOSContentComponent();

    void modeSelected(ApplicationMode::ID mode);

    void resized();

    void paint(Graphics& g);

private:
    ScopedPointer<iOSModeSelectorToolbar> toolbar;
    ApplicationMode::ID activeMode;
    ScopedPointer<Component> activeScreen;
};

#endif
