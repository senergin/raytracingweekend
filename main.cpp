#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "camera.h"
#include "external\OBJ_Loader.h"
#include "external\stb_image_write.h"
#include "hitable.h"
#include "materials.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

vec3 backgroundColor(const ray& r)
{
    vec3 unit = r.direction;
    float t1 = 0.5f - (0.5f * unit.y());
    float t2 = 0.5f + (0.5f * unit.y());
    return t1 * vec3(0.5f, 1.f, 1.f) + t2 * vec3(0.5f, 0.7f, 1.f);
}
vec3 color(const ray& r, const hitable* hitable, const float minDistance, const float maxDistance,
           const unsigned int depth, const unsigned int maxDepth)
{
    hitRecord rec;
    bool isHit = hitable->hit(r, minDistance, maxDistance, rec);
    if (isHit) {
        ray scattered;
        vec3 attenuation;
        if (depth < maxDepth && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation *
                   color(scattered, hitable, minDistance, maxDistance, depth + 1, maxDepth);
        }
        return vec3(0, 0, 0);
    }
    return backgroundColor(r);
}
hitable* randomScene()
{
    std::vector<hitable*> list;

    // OBJ
    vec3 objTranslate(0, 0, 1);
    float objScale = 0.75;
    objl::Loader loader;
    bool isLoaded = loader.LoadFile("resources\\teapot.obj");
    if (isLoaded) {
        const auto indices = loader.LoadedIndices;
        const auto vertices = loader.LoadedVertices;
        for (int i = 0; i < indices.size(); i += 3) {
            auto p1 = vertices[indices[i]].Position;
            auto p2 = vertices[indices[i + 1]].Position;
            auto p3 = vertices[indices[i + 2]].Position;
            vec3 pp1 = vec3(p1.X, p1.Y, p1.Z) * objScale + objTranslate;
            vec3 pp2 = vec3(p2.X, p2.Y, p2.Z) * objScale + objTranslate;
            vec3 pp3 = vec3(p3.X, p3.Y, p3.Z) * objScale + objTranslate;
            list.push_back(new triangle(pp1, pp2, pp3, new metal(vec3(0.7, 0.6, 0.5), 0.7)));
        }
    }

    // Sphere-world
    list.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f))));
    for (int a = -22; a < 22; a++) {
        for (int b = -22; b < 22; b++) {
            float chooseMat = myRandom::next();
            vec3 center(a + 0.9 * myRandom::next(), 0.2f, b + 0.9f * myRandom::next());
            if ((center - vec3(4, 0.2f, 0)).length() > 0.9f) {
                if (chooseMat < 0.8f) { // diffuse
                    list.push_back(
                        new sphere(center, 0.2f,
                                   new lambertian(vec3(myRandom::next() * myRandom::next(),
                                                       myRandom::next() * myRandom::next(),
                                                       myRandom::next() * myRandom::next()))));
                } else if (chooseMat < 0.95f) { // metal
                    list.push_back(new sphere(
                        center, 0.2f,
                        new metal(vec3(0.5f * (1 + myRandom::next()), 0.5f * (1 + myRandom::next()),
                                       0.5f * (1 + myRandom::next())),
                                  0.5f * myRandom::next())));
                } else { // glass
                    list.push_back(
                        new sphere(center, 0.2, new dielectric(vec3(1.f, 1.f, 1.f), 1.5f)));
                }
            }
        }
    }
    list.push_back(new sphere(vec3(-6, 1.5f, -4), 1.5f, new lambertian(vec3(0.4, 0.2, 0.1))));
    list.push_back(new sphere(vec3(-2, 1.5f, -4), 1.5f, new dielectric(vec3(1.f, 1.f, 1.f), 1.5)));
    list.push_back(new sphere(vec3(2, 1.5f, -4), 1.5f, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

    hitable** listArr = new hitable*[list.size()];
    std::copy(list.begin(), list.end(), listArr);
    return new hitableList(listArr, list.size());
}
struct raycastWorldParameters {
    const float minDistance;
    const float maxDistance;
    const unsigned int maxDepth;
    const unsigned int sampling;
    const unsigned int width;
    const unsigned int height;
    const unsigned int startWidth;
    const unsigned int endWidth;
    const unsigned int startHeight;
    const unsigned int endHeight;
    const unsigned int channels;
};
void raycastWorld(const raycastWorldParameters& params, const hitable* world, const camera& cam,
                  unsigned char* out)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::thread::id threadId = std::this_thread::get_id();

    for (unsigned int j = params.startHeight; j < params.endHeight; ++j) {
        std::printf("- threadId: 0x%08x %u/%u\n", threadId, (j - params.startHeight),
                    (params.endHeight - params.startHeight));
        for (unsigned int i = params.startWidth; i < params.endWidth; ++i) {
            vec3 col(0.f, 0.f, 0.f);
            for (unsigned int s = 0; s < params.sampling; ++s) {
                float u = float(i + myRandom::next()) / float(params.width);
                float v = float(j + myRandom::next()) / float(params.height);
                ray r = cam.getRay(u, v);
                col += color(r, world, params.minDistance, params.maxDistance,
                             /* depth */ 0, params.maxDepth);
            }
            col /= params.sampling;

            // Gamma correction
            int r = sqrtf(col.x()) * 255.99f;
            int g = sqrtf(col.y()) * 255.99f;
            int b = sqrtf(col.z()) * 255.99f;

            int index = ((j * params.width) + i) * params.channels;

            out[index + 0] = (unsigned char)(r);
            out[index + 1] = (unsigned char)(g);
            out[index + 2] = (unsigned char)(b);
            out[index + 3] = (unsigned char)255;
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::printf("--------------------------\n"
                "raycastWorld duration for:\n"
                " startWidth: %u\n"
                " endWidth: %u\n"
                " startHeight: %u\n"
                " endHeight: %u\n"
                " maxDepth: %u\n"
                " sampling: %u\n"
                " threadId: 0x%08x\n"
                "duration: %u seconds.\n",
                params.startWidth, params.endWidth, params.startHeight, params.endHeight,
                params.maxDepth, params.sampling, threadId, duration);
}
void singlethreadRaycast(const float minDistance, const float maxDistance,
                         const unsigned int maxDepth, const unsigned int sampling,
                         const unsigned int width, const unsigned int height,
                         const unsigned int channels, const hitable* world, const camera& cam,
                         unsigned char* const data)
{
    const raycastWorldParameters parameters{.minDistance = minDistance,
                                            .maxDistance = maxDistance,
                                            .maxDepth = maxDepth,
                                            .sampling = sampling,
                                            .width = width,
                                            .height = height,
                                            .startWidth = 0,
                                            .endWidth = width,
                                            .startHeight = 0,
                                            .endHeight = height,
                                            .channels = channels};
    raycastWorld(parameters, world, cam, data);
}
void multithreadRaycast(const float minDistance, const float maxDistance,
                        const unsigned int maxDepth, const unsigned int sampling,
                        const unsigned int width, const unsigned int height,
                        const unsigned int channels, const hitable* world, const camera& cam,
                        unsigned char* const data, unsigned int threadCount)
{
    std::vector<std::thread> workers;

    unsigned int startHeight = 0u;
    for (int i = 0; i < threadCount; i++) {
        const unsigned int startWidth = 0u;
        const unsigned int endWidth = width;
        const unsigned int minHeightPerThread = height / threadCount;
        const unsigned int threadHeight =
            i < (height % threadCount) ? minHeightPerThread + 1 : minHeightPerThread;
        const unsigned int endHeight = startHeight + threadHeight;
        workers.push_back(
            std::thread([minDistance, maxDistance, maxDepth, sampling, width, height, startWidth,
                         endWidth, startHeight, endHeight, channels, world, cam, data]() {
                const raycastWorldParameters parameters{.minDistance = minDistance,
                                                        .maxDistance = maxDistance,
                                                        .maxDepth = maxDepth,
                                                        .sampling = sampling,
                                                        .width = width,
                                                        .height = height,
                                                        .startWidth = startWidth,
                                                        .endWidth = endWidth,
                                                        .startHeight = startHeight,
                                                        .endHeight = endHeight,
                                                        .channels = channels};
                raycastWorld(parameters, world, cam, data);
            }));
        startHeight += threadHeight;
    }
    for (int i = 0; i < threadCount; i++) {
        workers[i].join();
    }
}
int main()
{
    const float minDistance = 0.001f;
    const float maxDistance = 10000.f;
    const unsigned int maxDepth = 40u;
    const unsigned int sampling = 100u;

    // Output image data
    const unsigned int width = 1000u;
    const unsigned int height = 600u;
    const unsigned int channels = 4u; // STBI_rgb_alpha

    // Multi-threading
    const unsigned int threadCount = std::thread::hardware_concurrency();

    const unsigned int outputSize = width * height * channels;

    // Camera
    // vec3 lookFrom(15, 6, 12);
    vec3 lookFrom(26, 4, 6);
    vec3 lookAt(0, 0, 0);
    float distanceToFocus = 20.0;
    float aperture = 0.1;

    camera cam(lookFrom, lookAt, /* up */ vec3(0, 1, 0), /* fov */ 20, (float)width / height,
               aperture, distanceToFocus);

    // Scene
    hitable* world = randomScene();

    unsigned char* const data = new unsigned char[outputSize];

    auto t1 = std::chrono::high_resolution_clock::now();

    if (threadCount == 1) {
        singlethreadRaycast(minDistance, maxDistance, maxDepth, sampling, width, height, channels,
                            world, cam, data);
    } else {
        multithreadRaycast(minDistance, maxDistance, maxDepth, sampling, width, height, channels,
                           world, cam, data, threadCount);
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::printf("---------------------\n"
                "Raycast duration for:\n"
                " width: %u\n"
                " height: %u\n"
                " maxDepth: %u\n"
                " sampling: %u\n"
                " threadCount: %u\n"
                "duration: %u seconds.\n",
                width, height, maxDepth, sampling, threadCount, duration);

    int ret = stbi_write_png("out.png", width, height, channels, data, channels * width);
    if (ret == 0) {
        std::cout << "problem at stbi_write_png" << std::endl;
    }

    delete world;
    delete[] data;

    return 0;
}