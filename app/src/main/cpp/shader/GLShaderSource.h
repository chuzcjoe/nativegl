//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once

#define VERTEX_SHADER(...)  #__VA_ARGS__
#define FRAGMENT_SHADER(...)  #__VA_ARGS__

const char* vertexShader = VERTEX_SHADER(
        attribute vec3 a_Position;
        attribute vec2 a_Texture;
        varying vec2 TexCoord;
        void main() {
           gl_Position = vec4(a_Position, 1.0);
           TexCoord = a_Texture;
        }
);

const char* fragmentShader = FRAGMENT_SHADER(
        precision mediump float;
        varying vec2 TexCoord;
        uniform sampler2D texture_load;
        void main() {
           gl_FragColor = texture2D(texture_load, TexCoord);
        }
);