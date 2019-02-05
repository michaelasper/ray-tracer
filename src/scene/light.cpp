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
    isect i;
    ray shadow(r);
    // have to check if t < t_light
    auto total = glm::dvec3(1.0, 1.0, 1.0);
    auto pos = glm::dvec3(0.0, 0.0, 0.0);
    // std::cerr << "EEEE" << std::endl;
    // std::cout << "CCCC" << std::endl;
    if (this->scene->intersect(shadow, i)) {
        if (!i.getMaterial().Trans()) {
            return i.getMaterial().kt(i);
        }
    }
    // std::cerr << total << std::endl;
    return total;
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
    double d = glm::sqrt(glm::dot(position - P, position - P));
    return 1.0 / (constantTerm + linearTerm * d + quadraticTerm * d * d);
}

glm::dvec3 PointLight::getColor() const { return color; }

glm::dvec3 PointLight::getDirection(const glm::dvec3& P) const {
    return glm::normalize(position - P);
}

glm::dvec3 PointLight::shadowAttenuation(const ray& r,
                                         const glm::dvec3& p) const {
    isect i;
    ray shadow(r);
    double diff = glm::length(p - position);
    // have to check if t < t_light
    auto total = glm::dvec3(1.0, 1.0, 1.0);
    auto pos = glm::dvec3(0.0, 0.0, 0.0);
    while (this->scene->intersect(shadow, i)) {
        if (i.getT() < diff) return total;
        if (!i.getMaterial().Trans()) {
            return glm::dvec3(0.0, 0.0, 0.0);
        } else {
            for (int j = 0; j < 3; j++) {
                total[j] *= glm::pow(i.getMaterial().kt(i)[j], i.getT());
            }
            pos = i.getN() * .000001;
            shadow.setPosition(shadow.at(i) + pos);
        }
    }

    return total;
}

#define VERBOSE 0
