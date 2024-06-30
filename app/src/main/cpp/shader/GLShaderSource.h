//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once

#define VERTEX_SHADER(...) "#version 300 es\n" #__VA_ARGS__
#define FRAGMENT_SHADER(...) "#version 300 es\n" #__VA_ARGS__

const char* vertexShader = VERTEX_SHADER(
       layout(location = 0) in vec3 a_Position;
       layout(location = 1) in vec2 a_Texture;
       out vec2 TexCoord;
       void main() {
           gl_Position = vec4(a_Position, 1.0);
           TexCoord = a_Texture;
       }
);

const char* fragmentShader = FRAGMENT_SHADER(
         precision mediump float;
         in vec2 TexCoord;
         uniform sampler2D texture_load;
         out vec4 outColor;
         void main() {
             outColor = texture(texture_load, TexCoord);
         }
 );