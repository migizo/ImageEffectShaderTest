/*
  ==============================================================================

    GameboyShader.h
    Created: 20 Dec 2022 8:41:25pm
    Author:  migizo

  ==============================================================================
*/

#pragma once

const static std::string fragmentShader = R"(

uniform sampler2D uTexture;
uniform vec2 uResolution;
varying vec3 pos;

vec3 color1 = vec3(0.784313725, 0.788235294, 0.262745098); // brigher
vec3 color2 = vec3(0.490196078, 0.521568627, 0.152941176);
vec3 color3 = vec3(0.000000000, 0.415686275, 0.000000000);
vec3 color4 = vec3(0.015686275, 0.243137255, 0.000000000); // darker

float gridSize = 5.0;
float padding = 1.0;
bool isInverted = false;

float getGrayscale(vec2 coord)
{
    vec2 uv = coord.xy / uResolution.xy;
    vec3 srcColor = texture2D(uTexture, uv).rgb;
    return (srcColor.r + srcColor.g + srcColor.b) / 3.0;
}

vec3 grayToColor(float gray)
{
    vec3 color = color1;
    if      (gray < 0.3) color = mix(color4, color3, smoothstep(0.0, 0.3, gray));
    else if (gray < 0.6) color = mix(color3, color2, smoothstep(0.3, 0.6, gray));
    else if (gray < 1.0) color = mix(color2, color1, smoothstep(0.6, 1.0, gray));
    return color;
}

void main (void)
{
    vec2 gridCoord = floor(gl_FragCoord.xy / gridSize);

    // [A] calculate average gray
    float gray = 0.0;
    vec2 originInGrid = gridCoord.xy * gridSize;
    for (int i = 0; i < int(gridSize*gridSize); i++)
    {
        float x = mod(float(i), gridSize);
        float y = floor(float(i) / gridSize);
        gray = gray + getGrayscale(originInGrid.xy + vec2(x, y));
    }
    gray /= gridSize*gridSize;

    // [B] simple gray
//    float gray = getGrayscale(gridCoord.xy * gridSize);
    
    vec2 coordInGrid = fract(gl_FragCoord.xy / gridSize) * gridSize;
    float gridW = 1.0;
    gridW = gridW * step(padding, coordInGrid.x) * step(coordInGrid.x, gridSize - padding);
    gridW = gridW * step(padding, coordInGrid.y) * step(coordInGrid.y, gridSize - padding);
    gray = gray + (1.0 - gridW) * 0.4;
    
    // invert
    if (isInverted)
    {
        gray = 1.0 - gray;
    }
    
    // gray to converted Color
    vec4 color = vec4(grayToColor(gray), 1.0);
    
    gl_FragColor.rgba = color.rgba;
}


)";
