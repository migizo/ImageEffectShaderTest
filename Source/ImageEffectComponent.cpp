/*
  ==============================================================================

    ImageEffectComponent.cpp
    Created: 20 Dec 2022 8:31:20pm
    Author:  migizo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ImageEffectComponent.h"
#include "GameboyShader.h"

//==============================================================================
ImageEffectComponent::ImageEffectComponent()
{
    // ソースとなるコンポーネントの上に表示させるための設定
    setInterceptsMouseClicks(false, false);
    
    // openGL設定
    openGLContext.setRenderer(this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting(true);
    openGLContext.setComponentPaintingEnabled (false);
    
    setSize (400, 400);
}

ImageEffectComponent::~ImageEffectComponent()
{
    openGLContext.detach();
}

void ImageEffectComponent::newOpenGLContextCreated()
{
    // texture
    auto imgW = getWidth() != 0 ? getWidth() : 1024;
    auto imgH = getHeight() != 0 ? getHeight() : 1024;
    srcImage = juce::Image(juce::Image::PixelFormat::ARGB, imgW, imgH, true);
    juce::Graphics g(srcImage);
    g.fillAll(juce::Colours::transparentBlack);
    
    // shader
    shader.reset (new juce::OpenGLGraphicsContextCustomShader (fragmentShader));
}

void ImageEffectComponent::renderOpenGL()
{
    if (shader.get() == nullptr) return;

    if (srcImage != juce::Image())
    {
        texture.loadImage(srcImage);
        srcImage = juce::Image();
    }
    
    // 描画クリア
    juce::OpenGLHelpers::clear (juce::Colours::black);

    // コンテキスト取得
    auto desktopScale = openGLContext.getRenderingScale();
    int w = juce::roundToInt (desktopScale * getWidth());
    int h = juce::roundToInt (desktopScale * getHeight());
    
    std::unique_ptr<juce::LowLevelGraphicsContext> glContext(createOpenGLGraphicsContext(openGLContext, w, h));
    if (glContext.get() == nullptr) return;
        
    shader->onShaderActivated = [this, w, h](juce::OpenGLShaderProgram& p)
    {
        using namespace juce::gl;
        glActiveTexture (GL_TEXTURE0);
        glEnable (GL_TEXTURE_2D);
        texture.bind();
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        p.setUniform("uTexture", (GLint)0);
        p.setUniform("uResolution", w, h);
    };
    shader->fillRect (*glContext.get(), glContext->getClipBounds());
}

void ImageEffectComponent::openGLContextClosing()
{
    texture.release();
}

void ImageEffectComponent::setSnapshot(const juce::Image& img)
{
    srcImage = img;
}
