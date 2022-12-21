#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(&srcComponent);
    addAndMakeVisible(&fxComponent);
    
    setSize (400, 400);
    
    startTimerHz(30);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
}

//==============================================================================
void MainComponent::timerCallback()
{
    fxComponent.setSnapshot(srcComponent.createComponentSnapshot(srcComponent.getLocalBounds()));
}

