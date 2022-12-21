/*
  ==============================================================================

    ImageEffectComponent.h
    Created: 20 Dec 2022 8:31:20pm
    Author:  migizo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ImageEffectComponent  : public juce::Component, public juce::OpenGLRenderer
{
public:
    ImageEffectComponent();
    ~ImageEffectComponent() override;
    
    // juce::OpenGLRenderer継承
    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing()override;

    // スナップショットをシェーダーに渡すための関数
    void setSnapshot(const juce::Image& img);

private:
    juce::OpenGLContext openGLContext;
    std::unique_ptr<juce::OpenGLGraphicsContextCustomShader> shader;
    
    juce::OpenGLTexture texture;    // シェーダーで扱うテクスチャ
    juce::Image srcImage;           // setSnapshot()経由でテクスチャに設定されるイメージ

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageEffectComponent)
};
