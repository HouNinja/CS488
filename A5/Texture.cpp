#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include "Texture.hpp"

using namespace glm;

Texture::Texture()
    :texture_id(-1) {}

Texture::~Texture() {}


CheckerTexture::CheckerTexture(vec3 c1, vec3 c2)
    : color1(c1), color2(c2) { texture_id = 1; }

CheckerTexture::~CheckerTexture() {}

vec3 CheckerTexture::get_color(float u, float v) {
    int sum = round(floor(u)) + round(floor(v));
    if ( sum % 2 == 0 ) {
        return color1;
    } else {
        return color2;
    }
}

vec3 CheckerTexture::get_color_3D(float u, float v, float w) {
    int sum = round(floor(u)) + round(floor(v)) + round(floor(w));
    if ( sum % 2 == 0 ) {
        return color1;
    } else {
        return color2;
    }
}

PerlinNoise::PerlinNoise(vec3 c1, vec3 c2)
    : color1(c1), color2(c2) { texture_id = 2; }

PerlinNoise::~PerlinNoise() {}

vec3 PerlinNoise::get_color(float u, float v) {
    return vec3(1.0, 1.0, 1.0);
}

vec3 PerlinNoise::get_color_3D(float u, float v, float w) {
    //glm::vec3 result;
    
    double freq = 1.0;
    
    float val = 0.0;
    
    glm::vec3 p2(u, v, w);
    val = glm::sin(std::abs(glm::perlin(p2)) + 
             std::abs(glm::perlin(2.0f * p2)) +
             1 / 2.0 * std::abs(glm::perlin(3.0f * p2)) +
             1 / 2.0 * std::abs(glm::perlin(4.0f * p2)));

//    val = (val + 1.0f) / 2.0;                // val is between 0 and 2 at this point, divided by 2 to get into [0, 1]
    if (val <= 0 ) std::cout << " warning " << std::endl;

    return val * color1 + (1 - val) * color2;
}
