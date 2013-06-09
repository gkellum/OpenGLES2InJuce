#include "iOSModeSelectorToolbar.h"

iOSModeSelectorToolbar::iOSModeSelectorToolbar()
{
    ApplicationMode::ID currentApplicationModeID = ApplicationMode::Triangle;
    int numberOfButtons = ApplicationMode::NumberOfApplicationModes;
    int radioGroupId = Random::getSystemRandom().nextInt();
    for (int counter = 0; counter < numberOfButtons; counter++)
    {
        ApplicationMode::ID applicationModeID = (ApplicationMode::ID) counter;
        String displayText = ApplicationMode::getName( applicationModeID );
        ReferenceCountedTextButton::Ptr button( new ReferenceCountedTextButton(displayText) );

        button->setRadioGroupId(radioGroupId);
        button->setClickingTogglesState(true);
        if (applicationModeID == currentApplicationModeID)
            button->setToggleState(true, false);
        button->addListener(this);

        buttons.push_back(button);
        addAndMakeVisible(button);
    }    
}

iOSModeSelectorToolbar::~iOSModeSelectorToolbar()
{
}

void iOSModeSelectorToolbar::modeSelected(ApplicationMode::ID mode)
{
    if (mode < buttons.size())
        buttons[mode]->setToggleState(true, false);
}

void iOSModeSelectorToolbar::buttonClicked(Button* button)
{
    for (int counter = 0; counter < buttons.size(); counter++)
    {
        if (button == buttons[counter])
        {
            ApplicationMode::ID applicationModeID = (ApplicationMode::ID) counter;

            Component::BailOutChecker checker(this);
            if (! checker.shouldBailOut())
                modeListeners.callChecked(checker, &Listener::modeSelected, applicationModeID);  
        }
    }
}

void iOSModeSelectorToolbar::resized()
{
    int numberOfButtons = buttons.size();
    int buttonWidth = float(getWidth()) / numberOfButtons;
    int buttonHeight = getHeight() > 44 ? 44 : getHeight();

    //float widthAdjustments[] = {0, 0, 0, 0, 0};
    int x = 0;
    int y = 5;
    for (int counter = 0; counter < numberOfButtons; counter++)
    {
        int width = buttonWidth; // + widthAdjustments[counter];
        buttons[counter]->setBounds(x,
                                    y,
                                    width,
                                    buttonHeight);
        x += width;
    }
}

void iOSModeSelectorToolbar::paint(Graphics& g)
{
    ColourGradient gradient(Colour(0xff555555), 0, 0, Colour(0xff000000), 0, getHeight(), false);    
    gradient.addColour(0.4999, Colour(0xff1B1B1B));
    gradient.addColour(0.5, Colour(0xff000000));
    g.setGradientFill(gradient);
    g.fillAll();
    g.setColour(Colour(0xff676667));
    g.drawLine(0, 0, getWidth(), 0, 1.0f);
}


