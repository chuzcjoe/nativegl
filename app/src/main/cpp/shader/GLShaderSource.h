//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once

#define VERTEX_SHADER(...)  #__VA_ARGS__
#define FRAGMENT_SHADER(...)  #__VA_ARGS__

const char* vertexShader = VERTEX_SHADER(
        attribute vec3 a_Position;
        void main() {
           gl_Position = vec4(a_Position, 1.0);
        }
);

const char* fragmentShader = FRAGMENT_SHADER(
        precision mediump float;
        vec4 color_temp;
        void main() {
           color_temp = vec4(0, 1, 0, 1);
           gl_FragColor = color_temp;
        }
);