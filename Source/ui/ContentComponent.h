#ifndef __IPhoneContentComponent_H__
#define __IPhoneContentComponent_H__

#include "JuceHeader.h"

#include "ModeSelectorToolbar.h"
#include "ApplicationMode.h"

class ContentComponent : public Component
                       , public ModeSelectorToolbar::Listener
{
public:
    ContentComponent();
    ~ContentComponent();

    void modeSelected(ApplicationMode::ID mode);

    void resized();

private:
    ApplicationMode::ID activeMode;

    ScopedPointer<ModeSelectorToolbar> toolbar;
    ScopedPointer<Component> activeScreen;
};

#endif
