#include <glm/glm.hpp>
#include "Texture.hpp"

using namespace glm;
using namespace std;

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
