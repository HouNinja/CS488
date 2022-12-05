#include <glm/glm.hpp>

using namespace glm;

#define TEXTURE




class Texture {
    public:
    int texture_id;
    Texture();
    virtual ~Texture();
    virtual glm::vec3 get_color(float u, float v) = 0;
    virtual glm::vec3 get_color_3D(float u, float v, float w) = 0;
};

class CheckerTexture : public Texture {
    public:
    vec3 color1;
    vec3 color2;
    CheckerTexture(vec3 c1, vec3 c2);
    virtual ~CheckerTexture();
    vec3 get_color(float u, float v) override;
    vec3 get_color_3D(float u, float v, float w) override;
};
