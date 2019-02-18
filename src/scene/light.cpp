#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "../ui/TraceUI.h"
#include "light.h"

using namespace std;
extern TraceUI* traceUI;

double DirectionalLight::distanceAttenuation(const glm::dvec3& P) const {
    // distance to light is infinite, so f(di) goes to 0.  Return 1.
    return 1.0;
}

glm::dvec3 DirectionalLight::shadowAttenuation(const ray& r,
                                               const glm::dvec3& p) const {
    isect i;
    ray shadow(r);
    auto total = glm::dvec3(1.0, 1.0, 1.0);
    // check if inside
    if (this->scene->intersect(shadow, i)) {
        // check if material is translucent for refraction
        if (i.getMaterial().Trans()) {
            // check if inside
            if (glm::dot(shadow.getDirection(), i.getN()) > 0) {
                double d = i.getT();
                auto trans = i.getMaterial().kt(i);
                glm::dvec3 atten(std::pow(trans[0], d), std::pow(trans[1], d),
                                 std::pow(trans[2], d));

                shadow.setPosition(shadow.at(i) + i.getN() * RAY_EPSILON);
                atten *= this->shadowAttenuation(shadow, shadow.getPosition());
                if (traceUI->isThreshold() &&
                    glm::dot(atten, atten) < traceUI->getThreshold())
                    return glm::dvec3(0.0);

                return atten;
            } else {
                shadow.setPosition(shadow.at(i) + i.getN() * -RAY_EPSILON);

                return this->shadowAttenuation(shadow, shadow.getPosition());
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
    // calculate length
    double diff = glm::length(p - position);

    // have to check if t < t_light
    if (this->scene->intersect(shadow, i) && i.getT() < diff) {
        if (i.getMaterial().Trans()) {
            // classic bug avoidance
            shadow.setPosition(shadow.at(i.getT() + 0.0001));
            // check if inside
            if (glm::dot(shadow.getDirection(), i.getN()) > 0) {
                double d = i.getT();
                auto trans = i.getMaterial().kt(i);
                glm::dvec3 atten(std::pow(trans[0], d), std::pow(trans[1], d),
                                 std::pow(trans[2], d));
                shadow.setPosition(shadow.at(i) + i.getN() * RAY_EPSILON);
                atten *= this->shadowAttenuation(shadow, shadow.getPosition());
                if (traceUI->isThreshold() &&
                    glm::dot(atten, atten) < traceUI->getThreshold())
                    return glm::dvec3(0.0);
                return atten;
            } else {
                shadow.setPosition(shadow.at(i) + i.getN() * -RAY_EPSILON);
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
