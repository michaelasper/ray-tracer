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
    isect i;
    ray shadow(r);
    auto total = glm::dvec3(1.0, 1.0, 1.0);
    auto pos = glm::dvec3(0.0, 0.0, 0.0);
   
    if (this->scene->intersect(shadow, i)) {
        if (i.getMaterial().Trans()) {
            
            if (glm::dot(shadow.getDirection(), i.getN()) > 0) {
                
                double d = i.getT();
                auto trans = i.getMaterial().kt(i);
                glm::dvec3 atten(std::pow(trans[0], d), std::pow(trans[1], d),
                                 std::pow(trans[2], d));

                pos = i.getN() * RAY_EPSILON;
                shadow.setPosition(shadow.at(i) + pos);
                atten *= this->shadowAttenuation(shadow, shadow.getPosition());
                return atten;
            } else {
                pos = i.getN() * -RAY_EPSILON;
                shadow.setPosition(shadow.at(i) + pos);
                //return this->shadowAttenuation(shadow, shadow.getPosition());
            }
        } else {
            return glm::dvec3(0.0, 0.0, 0.0);
        }
    }
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
    double d = glm::distance(position, P);
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
    auto pos = glm::dvec3(0.0, 0.0, 0.0);

    if (this->scene->intersect(shadow, i) && i.getT() < diff) {
        if (i.getMaterial().Trans()) {
            isect i2;
            shadow.setPosition(shadow.at(i.getT() + 0.0001));
            if (glm::dot(shadow.getDirection(), i.getN()) > 0) {
                double d = i.getT();
                auto trans = i.getMaterial().kt(i);
                glm::dvec3 atten(std::pow(trans[0], d), std::pow(trans[1], d),
                                 std::pow(trans[2], d));

                pos = i.getN() * RAY_EPSILON;
                shadow.setPosition(shadow.at(i) + pos);
                atten *= this->shadowAttenuation(shadow, shadow.getPosition());
                return atten;
            } else {
                pos = i.getN() * -RAY_EPSILON;
                shadow.setPosition(shadow.at(i) + pos);
                return this->shadowAttenuation(shadow, shadow.getPosition());
            }
        } else {
            return glm::dvec3(0.0, 0.0, 0.0);
        }
    } else {
        return glm::dvec3(1.0, 1.0, 1.0);
    }
}

#define VERBOSE 0
