#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#define MAX_THREADS 32

// The main ray tracer.

#include <time.h>
#include <glm/vec3.hpp>
#include <queue>
#include <random>
#include <thread>
#include "scene/cubeMap.h"
#include "scene/ray.h"

class Scene;
class Pixel {
   public:
    Pixel(int i, int j, unsigned char* ptr) : ix(i), jy(j), value(ptr) {}

    int ix;
    int jy;
    unsigned char* value;
};

class RayTracer {
   public:
    RayTracer();
    ~RayTracer();

    glm::dvec3 tracePixel(int i, int j);
    glm::dvec3 traceRay(ray& r, double thresh, int depth, int monte,
                        double& length);

    glm::dvec3 getPixel(int i, int j);
    void setPixel(int i, int j, glm::dvec3 color);
    void getBuffer(unsigned char*& buf, int& w, int& h);
    double aspectRatio();

    void traceImage(int w, int h);
    int aaImage();
    bool checkRender();
    void waitRender();

    // void handleThings(int i, int j) {
    //     std::thread myThread([this]() { tracePixel(i, j); });
    // }

    void traceSetup(int w, int h);
    void threadImage(int w, int h, int index);
    bool loadScene(const char* fn);
    bool sceneLoaded() { return scene != 0; }

    void setReady(bool ready) { m_bBufferReady = ready; }
    bool isReady() const { return m_bBufferReady; }

    const Scene& getScene() { return *scene; }

    bool stopTrace;

   private:
    glm::dvec3 trace(double x, double y);
    std::mt19937 eng;
    std::random_device rd;
    std::vector<unsigned char> buffer;
    int buffer_width, buffer_height;
    int bufferSize;
    unsigned int threads;
    int block_size;
    double thresh;
    double aaThresh;
    int samples;
    std::unique_ptr<Scene> scene;
    CubeMap* cubemap;

    bool m_bBufferReady;
};

#endif  // __RAYTRACER_H__
