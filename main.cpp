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
hitable* randomScene()
{
    unsigned int n = 500;
    unsigned int i = 0;
    hitable** list = new hitable*[n + 1];
    list[i++] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = myRandom::next();
            vec3 center(a + 0.9 * myRandom::next(), 0.2, b + 0.9 * myRandom::next());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (chooseMat < 0.8) { // diffuse
                    list[i++] =
                        new sphere(center, 0.2,
                                   new lambertian(vec3(myRandom::next() * myRandom::next(),
                                                       myRandom::next() * myRandom::next(),
                                                       myRandom::next() * myRandom::next())));
                } else if (chooseMat < 0.95) { // metal
                    list[i++] = new sphere(
                        center, 0.2,
                        new metal(vec3(0.5 * (1 + myRandom::next()), 0.5 * (1 + myRandom::next()),
                                       0.5 * (1 + myRandom::next())),
                                  0.5 * myRandom::next()));
                } else { // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(vec3(1.f, 1.f, 1.f), 1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(vec3(1.f, 1.f, 1.f), 1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitableList(list, i);
}
int main()
{
    float x = myRandom::next();

    const unsigned int width = 1000;
    const unsigned int height = 600;
    const unsigned int sampling = 20;
    const unsigned int maxDepth = 40;
    const float minDistance = 0.001f;
    const float maxDistance = 10000.f;

    // Output image data
    const int channels = 4; // STBI_rgb_alpha
    unsigned char* data = new unsigned char[width * height * channels];

    // Camera
    vec3 lookFrom(13, 2, 3);
    vec3 lookAt(0, 0, 0);
    float distanceToFocus = 10.0;
    float aperture = 0.1;

    camera cam(lookFrom, lookAt, /* up */ vec3(0, 1, 0), /* fov */ 20, (float)width / height,
               aperture, distanceToFocus);

    // Scene
    hitable* world = randomScene();

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
            int r = sqrtf(col.x) * 255.99f;
            int g = sqrtf(col.y) * 255.99f;
            int b = sqrtf(col.z) * 255.99f;

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

    return 0;
}