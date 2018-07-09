#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "camera.h"
#include "hitableList.h"
#include "myRandom.h"
#include "sphere.h"
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
vec3 color(const ray& r, const hitable* world, const float minDistance, const float maxDistance)
{
    hitRecord rec;
    bool isHit = world->hit(r, minDistance, maxDistance, rec);
    if (isHit) {
        return 0.5f * (rec.normal + 1);
    }
    return backgroundColor(r);
}
int main()
{
    myRandom random;
    float x = random.next();

    const int width = 400;
    const int height = 200;
    const int sampling = 100;
    const float minDistance = 0.f;
    const float maxDistance = 10000.f;
    // Output image data
    const int channels = 4; // STBI_rgb_alpha
    unsigned char* data = new unsigned char[width * height * channels];

    hitable* list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5f);
    list[1] = new sphere(vec3(0, -100.5f, -1), 100);
    hitable* world = new hitableList(list, 2);
    camera cam;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            vec3 col;
            for (int s = 0; s < sampling; ++s) {
                float u = float(i + random.next()) / float(width);
                float v = float(j + random.next()) / float(height);
                ray r = cam.getRay(u, v);
                col += color(r, world, minDistance, maxDistance);
            }
            col /= sampling;

            int index = ((j * width) + i) * channels;
            data[index + 0] = (unsigned char)(col.x * 255.99f);
            data[index + 1] = (unsigned char)(col.y * 255.99f);
            data[index + 2] = (unsigned char)(col.z * 255.99f);
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

    return 0;
}