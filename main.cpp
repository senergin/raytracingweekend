#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "camera.h"
#include "hitable.h"
#include "materials.h"
#include "stb_image_write.h"
#include <cstdint>
#include <iostream>

vec3 backgroundColor(const ray& r)
{
    vec3 unit = r.direction;
    float t1 = 0.5f - (0.5f * unit.y);
    float t2 = 0.5f + (0.5f * unit.y);
    return t1 * vec3(1.f, 1.f, 1.f) + t2 * vec3(0.5f, 0.7f, 1.f);
}
vec3 color(const ray& r, const hitable* world, const float minDistance, const float maxDistance,
           const unsigned int depth, const unsigned int maxDepth)
{
    hitRecord rec;
    bool isHit = world->hit(r, minDistance, maxDistance, rec);
    if (isHit) {
        ray scattered;
        vec3 attenuation;
        if (depth < maxDepth && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation *
                   color(scattered, world, minDistance, maxDistance, depth + 1, maxDepth);
        }
        return vec3(0.f, 0.f, 0.f);
    }
    return backgroundColor(r);
}
int main()
{
    float x = myRandom::next();

    const unsigned int width = 960;
    const unsigned int height = 540;
    const unsigned int sampling = 20;
    const unsigned int maxDepth = 40;
    const float minDistance = 0.001f;
    const float maxDistance = 10000.f;
    // Output image data
    const int channels = 4; // STBI_rgb_alpha
    unsigned char* data = new unsigned char[width * height * channels];

    // Camera
    camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 45.f, (float)width / height);

    // Scene
    hitable* list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.f)));
    list[2] = new sphere(vec3(1, -0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.1));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(vec3(1.f, 1.f, 1.f), 1.5f));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(vec3(1.f, 1.f, 1.f), 1.5f));
    hitable* world = new hitableList(list, 5);
    // hitable* list[2];
    // float r = cos(mathx::pi / 4);
    // list[0] = new sphere(vec3(-r, 0, -1), r, new lambertian(vec3(0, 0, 1)));
    // list[1] = new sphere(vec3(r, 0, -1), r, new lambertian(vec3(1, 0, 0)));
    // hitable* world = new hitableList(list, 2);

    for (unsigned int j = 0; j < height; ++j) {
        for (unsigned int i = 0; i < width; ++i) {
            vec3 col(0.f, 0.f, 0.f);
            for (unsigned int s = 0; s < sampling; ++s) {
                float u = float(i + myRandom::next()) / float(width);
                float v = float(j + myRandom::next()) / float(height);
                ray r = cam.getRay(u, v);
                col += color(r, world, minDistance, maxDistance, /* depth */ 0, maxDepth);
            }
            col /= sampling;
            // Gamma correction
            int r = std::sqrt(col.x) * 255.99f;
            int g = std::sqrt(col.y) * 255.99f;
            int b = std::sqrt(col.z) * 255.99f;

            int index = ((j * width) + i) * channels;

            data[index + 0] = (unsigned char)(r);
            data[index + 1] = (unsigned char)(g);
            data[index + 2] = (unsigned char)(b);
            data[index + 3] = (unsigned char)255;
        }
    }

    int ret = stbi_write_png("out.png", width, height, channels, data, channels * width);
    if (ret == 0) {
        std::cout << "problem at stbi_write_png" << std::endl;
    }

    delete world;
    delete list[0];
    delete list[1];
    delete list[2];
    delete list[3];
    delete list[4];

    return 0;
}