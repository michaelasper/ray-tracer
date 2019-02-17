#include "material.h"
#include "../ui/TraceUI.h"
#include "light.h"
#include "ray.h"
extern TraceUI* traceUI;

#include <glm/gtx/io.hpp>
#include <iostream>
#include "../fileio/images.h"

using namespace std;
extern bool debugMode;

Material::~Material() {}

glm::dvec3 Material::shade(Scene* scene, const ray& r, const isect& i) const {
    glm::dvec3 normal = i.getN();
    glm::dvec3 v = glm::normalize(-r.getDirection());

    glm::dvec3 Ia = scene->ambient();
    glm::dvec3 intensity = ke(i) + ka(i) * Ia;
    glm::dvec3 pos = r.at(i);
    double alpha = shininess(i);

    // iterate through all the lights
    for (const auto& pLight : scene->getAllLights()) {
        glm::dvec3 l = pLight->getDirection(pos);
        glm::dvec3 reflect = 2 * glm::dot(l, normal) * normal - l;

        glm::dvec3 atten = pLight->getColor();
        atten *= glm::clamp(1.0, 0.0, pLight->distanceAttenuation(pos));
        ray shadow(pos + 1e-7 * normal, l, glm::dvec3(1.0, 1.0, 1.0),
                   ray::SHADOW);
        atten *= pLight->shadowAttenuation(shadow, pos + 1e-7 * normal);

        // Phong Model
        auto diffuse =
            kd(i) * (i.getMaterial().Trans() ? abs(glm::dot(l, normal))
                                             : max(glm::dot(l, normal), 0.0));
        auto spec = ks(i) * pow(max(glm::dot(v, reflect), 0.0), alpha);

        intensity += atten * (diffuse + spec);
    }

    return intensity;
}

TextureMap::TextureMap(string filename) {
    data = readImage(filename.c_str(), width, height);
    if (data.empty()) {
        width = 0;
        height = 0;
        string error("Unable to load texture map '");
        error.append(filename);
        error.append("'.");
        throw TextureMapException(error);
    }
}

glm::dvec3 TextureMap::getMappedValue(const glm::dvec2& coord) const {
    double x = (coord[0] * (this->getWidth() - 1));
    double y = (coord[1] * (this->getHeight() - 1));

    double x1 = int(x);
    double x2 = x1 - floor(x1);
    double y1 = int(y);
    double y2 = y1 - floor(y1);
    auto q11 = getPixelAt(x1, y1);
    auto q21 = getPixelAt(x2, y1);
    auto q12 = getPixelAt(x1, y2);
    auto q22 = getPixelAt(x1, y2);

    // Bilinear interpolation from wikipedia
    auto color = double(1 / ((x2 - x1) * (y2 - y1))) *
                 (q11 * (x2 - x) * (y2 - y) + q21 * (x - x1) * (y2 - y) +
                  q12 * (x2 - x) * (y - y1) + q22 * (x - x1) * (y - y2));

    return color;
}

glm::dvec3 TextureMap::getPixelAt(int x, int y) const {
    glm::dvec3 pixelColor(0.0, 0.0, 0.0);

    if (x < 0 || x >= this->getWidth()) return pixelColor;
    if (y < 0 || y >= this->getHeight()) return pixelColor;

    int offset = x + y * width;

    for (int i = 0; i < 3; i++) {
        pixelColor[i] = double(data[i + offset * 3]);
    }

    return pixelColor / 255.0;
}

glm::dvec3 MaterialParameter::value(const isect& is) const {
    if (0 != _textureMap)
        return _textureMap->getMappedValue(is.getUVCoordinates());
    else
        return _value;
}

double MaterialParameter::intensityValue(const isect& is) const {
    if (0 != _textureMap) {
        glm::dvec3 value(_textureMap->getMappedValue(is.getUVCoordinates()));
        return (0.299 * value[0]) + (0.587 * value[1]) + (0.114 * value[2]);
    } else
        return (0.299 * _value[0]) + (0.587 * _value[1]) + (0.114 * _value[2]);
}
