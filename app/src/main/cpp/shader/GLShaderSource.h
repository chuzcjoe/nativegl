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
         out vec4 FragColor;
         void main() {
             FragColor = texture(texture_load, TexCoord);
         }
 );

const char* sdr2hdrFragmentShader = FRAGMENT_SHADER(
      precision mediump float;
      in vec2 TexCoord;
      uniform sampler2D texture_load;
      out vec4 FragColor;

      // EOTF (Electro-Optical Transfer Function) for SDR to linear conversion
      vec3 sdrToLinear(vec3 sdr) {
          return pow(sdr, vec3(2.2));
      }

      // Color space transformation matrix (sRGB to BT.2020)
      const mat3 sRGBToBT2020 = mat3(
              0.6274, 0.0691, 0.0164,
              0.3293, 0.9195, 0.0880,
              0.0433, 0.0114, 0.8956
      );

      // Simple tone mapping function (Reinhard)
      vec3 toneMap(vec3 hdr) {
          return hdr / (hdr + vec3(1.0));
      }

      // OETF (Opto-Electronic Transfer Function) for linear to PQ HDR
      vec3 linearToPQ(vec3 linear) {
          float m1 = 0.1593017578125;
          float m2 = 78.84375;
          float c1 = 0.8359375;
          float c2 = 18.8515625;
          float c3 = 18.6875;

          vec3 Y = pow(linear, vec3(0.1593017578125));
          return pow((c1 + c2 * Y) / (1.0 + c3 * Y), vec3(78.84375));
      }

      void main()
      {
          // Sample the SDR texture
          vec3 sdrColor = texture(texture_load, TexCoord).rgb;

          // Convert SDR to linear space
          vec3 linearColor = sdrToLinear(sdrColor);

          // Apply color space transformation
          vec3 bt2020Color = sRGBToBT2020 * linearColor;

          // Apply HDR expansion (you can adjust this factor)
          float hdrFactor = 4.0;
          vec3 hdrColor = bt2020Color * hdrFactor;

          // Apply tone mapping
          vec3 toneMappedColor = toneMap(hdrColor);

          // Convert to PQ HDR space
          vec3 pqColor = linearToPQ(toneMappedColor);

          FragColor = vec4(pqColor, 1.0);
      }
);