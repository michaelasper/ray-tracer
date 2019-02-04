#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "light.h"

using namespace std;

double DirectionalLight::distanceAttenuation(const glm::dvec3& P) const {
    // distance to light is infinite, so f(di) goes to 0.  Return 1.
    return 1.0;
}

glm::dvec3 DirectionalLight::shadowAttenuation(const ray& r,
                                               const glm::dvec3& p) const {
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    glm::dvec3 l = glm::normalize(getDirection(p));
    ray shadow(p + l * 1e-4, l, r.getAtten(), ray::SHADOW);

    isect i;
    if (this->scene->intersect(shadow, i)) {
        return glm::dvec3(0, 0, 0);
    } else {
        return glm::dvec3(1, 1, 1);
    }
}

glm::dvec3 DirectionalLight::getColor() const { return color; }

glm::dvec3 DirectionalLight::getDirection(const glm::dvec3& P) const {
    return -orientation;
}
// YOUR CODE HERE

// You'll need to modify this method to attenuate the intensity
// of the light based on the distance between the source and the
// point P.  For now, we assume no attenuation and just return 1.0

// return 1.0;
double PointLight::distanceAttenuation(const glm::dvec3& P) const {
    double d = sqrt(glm::dot(position - P, position - P));
    return 1.0 / (constantTerm + linearTerm * d + quadraticTerm * d * d);
}

glm::dvec3 PointLight::getColor() const { return color; }

glm::dvec3 PointLight::getDirection(const glm::dvec3& P) const {
    return glm::normalize(position - P);
}

glm::dvec3 PointLight::shadowAttenuation(const ray& r,
                                         const glm::dvec3& p) const {
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    // auto d =
    glm::dvec3 l = glm::normalize(getDirection(p));
    ray shadow(p + l * 1e-4, l, r.getAtten(), ray::SHADOW);

    isect i;
    if (this->scene->intersect(shadow, i)) {
        return glm::dvec3(0, 0, 0);
    } else {
        return glm::dvec3(1, 1, 1);
    }
}

#define VERBOSE 0
