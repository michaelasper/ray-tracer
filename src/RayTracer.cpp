// The main ray tracer.

#pragma warning(disable : 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Parser.h"
#include "parser/Tokenizer.h"

#include <string.h>  // for memset
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "ui/TraceUI.h"

#include <fstream>
#include <iostream>
#include <random>
#include <thread>

using namespace std;
extern TraceUI* traceUI;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through pixel(i,j), i.e. normalized window coordinates
// (x,y), through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of
// 0.

glm::dvec3 RayTracer::trace(double x, double y) {
    // Clear out the ray cache in the scene for debugging purposes,
    if (TraceUI::m_debug) scene->intersectCache.clear();

    ray r(glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(1, 1, 1),
          ray::VISIBILITY);
    scene->getCamera().rayThrough(x, y, r);
    double dummy;
    glm::dvec3 ret;
    if (traceUI->isThreshold()) {
        // std::cout << "ran" << std::endl;
        if (traceUI->pathTrace()) {
            ret = traceRay(r, traceUI->getThreshold(), traceUI->getDepth(),
                           (int)traceUI->getMC(), dummy);
        } else {
            ret = traceRay(r, traceUI->getThreshold(), traceUI->getDepth(), 0,
                           dummy);
        }
    } else {
        if (traceUI->pathTrace()) {
            ret = traceRay(r, 0.0, traceUI->getDepth(), (int)traceUI->getMC(),
                           dummy);
        } else {
            ret = traceRay(r, 0.0, traceUI->getDepth(), 0, dummy);
        }
    }
    // http://cg.skeelogy.com/depth-of-field-using-raytracing/
    // https://stackoverflow.com/questions/13532947/references-for-depth-of-field-implementation-in-a-raytracer
    if (traceUI->dSwitch()) {
        ray depthRay(r);
        int raySamples = traceUI->getDivs();

        // First, define an aperture: a planar area center on your eye and
        // parallel to the retina/frame. The bigger the aperture, the more DOF
        // effect will be evident. Aperture's are typically just circles, in
        // which case it is easily defined by its radius. Other shapes can lead
        // to different lighting effects.

        // Also define a "focal distance". I don't think that's actually the
        // correct term for it, but it's the distance from the eye at which
        // things will be perfectly in focus.

        double apertureSize = traceUI->getApSize();
        double focalDistance = traceUI->getFocalD();

        // Start by casting a ray like normal from the eye through the pixel out
        // into the scene. Instead of intersecting it with objects in the scene,
        // though, you just want to find the point on the ray for which the
        // distance from the eye is equal to the selected focal distance. Call
        // this point the focal point for the pixel.

        glm::dvec3 focalPointN = -r.getDirection();
        glm::dvec3 focalPointPos = r.at(focalDistance);

        // focal point plane
        double t = glm::dot(focalPointN, r.getDirection());
        t = glm::dot(focalPointPos - r.getPosition(), focalPointN) / t;
        glm::dvec3 dest = r.at(t);

        std::uniform_real_distribution<double> distRadius(0, apertureSize / 2);
        std::uniform_real_distribution<double> distPhi(0,
                                                       2 * glm::pi<double>());
        for (int i = 0; i < raySamples; ++i) {
            // std::cout << "ran" << std::endl;

            // Now select a random starting point on the aperture. For a
            // circular aperture, it's pretty easy, you can select a random
            // polar angle and a random radius (no greater than the radius of
            // the aperture). You want a uniform distribution over the entire
            // aperture, don't try to bias it towards the center or anything.

            double phi = distPhi(eng);
            double radius = distRadius(eng);

            glm::dvec3 offVec = (glm::cos(phi) * scene->getCamera().getV() +
                                 glm::sin(phi) * scene->getCamera().getU()) *
                                radius;

            r.setPosition(scene->getCamera().getEye() + offVec);
            r.setDirection(glm::normalize(dest - r.getPosition()));

            // Cast a ray from your selected point on the aperture through the
            // focal point. Note that it will not necessarily pass through the
            // same pixel, that's ok.

            ret += traceRay(r, traceUI->getThreshold(), traceUI->getDepth(), 0,
                            dummy);
        }

        ret *= 1.0 / double(raySamples + 1.0);
    }
    ret = glm::clamp(ret, 0.0, 1.0);
    return ret;
}

glm::dvec3 RayTracer::tracePixel(int i, int j) {
    glm::dvec3 col(0, 0, 0);
    glm::dvec3 col1(0, 0, 0);
    glm::dvec3 col2(0, 0, 0);

    if (!sceneLoaded()) return col;

    double x = double(i) / double(buffer_width);
    double y = double(j) / double(buffer_height);

    // std::cout << view - offset << std::endl;
    // std::cout << view + offset << std::endl;
    // std::cout << up << std::endl;
    unsigned char* pixel = buffer.data() + (i + j * buffer_width) * 3;

    if (traceUI->dddSwitch()) {
        glm::dvec3 eye(scene->getCamera().getEye());
        glm::dvec3 offset(0.2, 0.00, 0.00);
        glm::mat3 red, blue;
        col1 = trace(x, y);
        scene->getCamera().setEye(eye + offset);
        col2 = trace(x, y);
        scene->getCamera().setEye(eye);

        switch (traceUI->get3dMode()) {
            case 0:
                red = glm::mat3(glm::dvec3(.299, 0.0, 0.0),
                                glm::dvec3(.587, 0.0, 0.0),
                                glm::dvec3(.114, 0.0, 0.0));
                blue = glm::mat3(glm::dvec3(0.0, .299, .299),
                                 glm::dvec3(0.0, .587, .587),
                                 glm::dvec3(0.0, .114, .114));
                break;
            case 1:
                red = glm::mat3(glm::dvec3(.299, 0.0, 0.0),
                                glm::dvec3(.587, 0.0, 0.0),
                                glm::dvec3(.114, 0.0, 0.0));
                blue = glm::mat3(glm::dvec3(0.0, 0.0, 0.0),
                                 glm::dvec3(0.0, 1.0, 0.0),
                                 glm::dvec3(0.0, 0.0, 1.0));
                break;
            default:
                break;
        }

        col = red * col1 + blue * col2;
    } else {
        col = trace(x, y);
    }
    // col = red * col1;

    pixel[0] = (int)(255.0 * col[0]);
    pixel[1] = (int)(255.0 * col[1]);
    pixel[2] = (int)(255.0 * col[2]);
    return col;
}

#define VERBOSE 0

ray reflectDirection(ray& r, const isect& i) {
    glm::dvec3 w_in = r.getDirection();
    auto w_ref = w_in - 2 * glm::dot(w_in, i.getN()) * i.getN();
    return ray(r.at(i.getT()) + i.getN() * RAY_EPSILON, w_ref, r.getAtten(),
               ray::REFLECTION);
}

glm::dvec3 RayTracer::traceRay(ray& r, double thresh, int depth, int monte,
                               double& t) {
    isect i;
    glm::dvec3 colorC;
    if (depth < 0) return colorC = glm::dvec3(0.0, 0.0, 0.0);

    if (scene->intersect(r, i)) {
        const Material& m = i.getMaterial();
        colorC = m.shade(scene.get(), r, i);
        if (traceUI->isThreshold() && glm::dot(colorC, colorC) < thresh)
            return colorC;

        t = i.getT();
        bool inside = glm::dot(-r.getDirection(), i.getN()) < 0;

        if (traceUI->pathTrace()) {
            // enableRTX(Speed::REALTIME);
            // do stuff
            if (monte > 0 && r.type() == ray::VISIBILITY) {
                // do actual stuff
                glm::dvec3 ptColor(0.0);
                double numSamples = traceUI->getPathSamples() /
                                    std::max(traceUI->getMC() - monte, 1.0);
                std::uniform_real_distribution<double> dist(0.0, 1.0);
                for (int count = 0; count < numSamples; count++) {
                    // Shoot random  rays

                    // MAKE A RANDOM RAY
                    double r1 = dist(eng);
                    double r2 = dist(eng);

                    double rad = sqrt(r1);
                    double theta = 2 * r2 * glm::pi<double>();

                    double x = rad * cos(theta);
                    double y = rad * sin(theta);

                    glm::dvec3 newDir(x, y, sqrt(max(0.0, 1 - r1)));
                    glm::dvec3 newPt =
                        (r.getPosition() + i.getT() * r.getDirection()) -
                        RAY_EPSILON * r.getDirection();

                    ray newRay(newPt, newDir, glm::dvec3(1.0), ray::VISIBILITY);
                    glm::dvec3 rayColor =
                        traceRay(newRay, thresh, depth - 1, monte - 1, t);

                    ptColor += glm::dot(i.getN(), newDir) * rayColor;
                }

                double albedo = 1.0;
                colorC += ptColor * (1.0 / traceUI->getPathSamples()) * albedo;
            }
        }

        // Reflections
        if (m.Refl() && !inside) {
            auto reflect = reflectDirection(r, i);
            auto new_depth = depth - 1;
            double newT = 0;
            colorC +=
                traceRay(reflect, thresh, new_depth, monte, newT) * m.kr(i);
        }

        // Refractions
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        if (m.Trans()) {
            auto N = i.getN();

            double n1 = 1;
            double n2 = m.index(i);

            if (inside) {
                std::swap(n1, n2);
                N = -N;
            }

            auto T = glm::refract(r.getDirection(), N, n1 / n2);
            auto new_depth = depth - 1;

            // handle TIR here

            if (!isnan(T[0])) {
                glm::dvec3 pos = r.at(i) - RAY_EPSILON * N;

                ray refract = ray(pos, T, r.getAtten(),
                                  (inside ? ray::VISIBILITY : ray::REFRACTION));
                double newT = 0;
                auto temp = traceRay(refract, thresh, new_depth, monte, newT);

                if (!inside)
                    temp *= glm::dvec3(std::pow(m.kt(i)[0], newT),
                                       std::pow(m.kt(i)[1], newT),
                                       std::pow(m.kt(i)[2], newT));
                colorC += temp;
            } else {
                // ray reflect = reflectDirection(r, i);
                glm::dvec3 w_in = r.getDirection();
                auto w_ref = w_in - 2 * glm::dot(w_in, -i.getN()) * -i.getN();
                ray reflect(r.at(i.getT()) - i.getN() * RAY_EPSILON, w_ref,
                            r.getAtten(), ray::REFLECTION);
                reflect.setPosition(r.at(i) - RAY_EPSILON * N);
                double newT = 0;
                auto temp = traceRay(reflect, thresh, new_depth, monte, newT);

                if (!inside)
                    temp *= glm::dvec3(std::pow(m.kt(i)[0], newT),
                                       std::pow(m.kt(i)[1], newT),
                                       std::pow(m.kt(i)[2], newT));

                colorC += temp;
            }
        }

    } else {
        if (traceUI->cubeMap()) {
            colorC = cubemap->getColor(r);
        } else
            colorC = glm::dvec3(0.0, 0.0, 0.0);
    }
    return colorC;
}

RayTracer::RayTracer()
    : scene(nullptr),
      buffer(0),
      thresh(0),
      buffer_width(0),
      buffer_height(0),
      m_bBufferReady(false) {
    std::mt19937 eng(rd());  // seed the generator
}

RayTracer::~RayTracer() {}

void RayTracer::getBuffer(unsigned char*& buf, int& w, int& h) {
    buf = buffer.data();
    w = buffer_width;
    h = buffer_height;
}

double RayTracer::aspectRatio() {
    return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene(const char* fn) {
    ifstream ifs(fn);
    if (!ifs) {
        string msg("Error: couldn't read scene file ");
        msg.append(fn);
        traceUI->alert(msg);
        return false;
    }

    // Strip off filename, leaving only the path:
    string path(fn);
    if (path.find_last_of("\\/") == string::npos)
        path = ".";
    else
        path = path.substr(0, path.find_last_of("\\/"));

    // Call this with 'true' for debug output from the tokenizer
    Tokenizer tokenizer(ifs, false);
    Parser parser(tokenizer, path);
    try {
        scene.reset(parser.parseScene());
    } catch (SyntaxErrorException& pe) {
        traceUI->alert(pe.formattedMessage());
        return false;
    } catch (ParserException& pe) {
        string msg("Parser: fatal exception ");
        msg.append(pe.message());
        traceUI->alert(msg);
        return false;
    } catch (TextureMapException e) {
        string msg("Texture mapping exception: ");
        msg.append(e.message());
        traceUI->alert(msg);
        return false;
    }

    if (!sceneLoaded()) return false;

    return true;
}

void RayTracer::traceSetup(int w, int h) {
    size_t newBufferSize = w * h * 3;
    if (newBufferSize != buffer.size()) {
        bufferSize = newBufferSize;
        buffer.resize(bufferSize);
    }
    buffer_width = w;
    buffer_height = h;
    std::fill(buffer.begin(), buffer.end(), 0);
    m_bBufferReady = true;

    /*
     * Sync with TraceUI
     */

    threads = traceUI->getThreads();
    block_size = traceUI->getBlockSize();
    thresh = traceUI->getThreshold();
    samples = traceUI->getSuperSamples();
    aaThresh = traceUI->getAaThreshold();
    if (traceUI->cubeMap()) cubemap = traceUI->getCubeMap();

    // YOUR CODE HERE
    // FIXME: Additional initializations
    scene->buildBVH();
}

/*
 * RayTracer::traceImage
 *
 *	Trace the image and store the pixel data in RayTracer::buffer.
 *
 *	Arguments:
 *		w:	width of the image buffer
 *		h:	height of the image buffer
 *
 */
void RayTracer::traceImage(int w, int h) {
    // Always call traceSetup before rendering anything.
    // traceSetup(w, h);

    traceSetup(w, h);
    std::vector<std::thread> threads;

    // for (int i = 0; i < w; i++) {
    //     for (int j = 0; j < h; j++) {
    //         // tracePixel(i, j);
    //         threads.push_back(std::thread(&RayTracer::tracePixel, this, i,
    //         j));
    //     }
    // }

    for (int i = 0; i < 100; ++i) {
        threads.push_back(std::thread(&RayTracer::threadImage, this, w, h, i));
    }

    for (auto& th : threads) th.join();
}

void RayTracer::threadImage(int w, int h, int index) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (j % 100 == index) {
                this->tracePixel(i, j);
            }
        }
    }
}

int RayTracer::aaImage() {
    // bool ssSwitch = traceUI->ssSwitch();
    for (int i = 0; i < buffer_width; i++) {
        for (int j = 0; j < buffer_height; j++) {
            glm::dvec3 color(0.0, 0.0, 0.0);
            for (int m = 0; m < samples; m++) {
                for (int n = 0; n < samples; n++) {
                    if (traceUI->ssSwitch()) {
                        double x = double(i * samples + m) /
                                   double(buffer_width * samples);
                        double y = double(j * samples + n) /
                                   double(buffer_height * samples);
                        // std::cout << "ran" << std::endl;
                        // https://www.alanzucconi.com/2015/09/16/how-to-sample-from-a-gaussian-distribution/
                        double dist_min = min(1.0 / buffer_height * samples,
                                              1.0 / buffer_width * samples);
                        std::uniform_real_distribution<double> dist(-1.0, 1.0);

                        double u1, u2, s;
                        do {
                            u1 = dist(eng);
                            u2 = dist(eng);
                            s = u1 * u1 + u2 * u2;
                        } while (s >= 1.0 || s == 0.0);

                        s = sqrt((-2.0 * glm::log(s)) / s);

                        u1 *= s;

                        u1 *= dist_min;
                        u1 /= samples;
                        u2 *= dist_min;
                        u2 /= samples;
                        x += u1;
                        y += u2;
                        color += trace(x, y);
                    } else {
                        double x = double(i * samples + m) /
                                   double(buffer_width * samples);
                        double y = double(j * samples + n) /
                                   double(buffer_height * samples);
                        color += trace(x, y);
                    }
                }
            }
            color /= (samples * samples);
            setPixel(i, j, color);
        }
    }
    return 0;
}

bool RayTracer::checkRender() {
    // YOUR CODE HERE
    // FIXME: Return true if tracing is done.
    //        This is a helper routine for GUI.
    //
    // TIPS: Introduce an array to track the status of each worker thread.
    //       This array is maintained by the worker threads.
}

void RayTracer::waitRender() {
    // YOUR CODE HERE
    // FIXME: Wait until the rendering process is done.
    //        This function is essential if you are using an asynchronous
    //        traceImage implementation.
    //
    // TIPS: Join all worker threads here.
}

glm::dvec3 RayTracer::getPixel(int i, int j) {
    unsigned char* pixel = buffer.data() + (i + j * buffer_width) * 3;
    return glm::dvec3((double)pixel[0] / 255.0, (double)pixel[1] / 255.0,
                      (double)pixel[2] / 255.0);
}

void RayTracer::setPixel(int i, int j, glm::dvec3 color) {
    unsigned char* pixel = buffer.data() + (i + j * buffer_width) * 3;

    pixel[0] = (int)(255.0 * color[0]);
    pixel[1] = (int)(255.0 * color[1]);
    pixel[2] = (int)(255.0 * color[2]);
}
