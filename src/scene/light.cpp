#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <random>
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
    shadow.setPosition(p - RAY_EPSILON * r.getDirection());
    shadow.setDirection(this->getDirection(p));

    auto total = glm::dvec3(1.0, 1.0, 1.0);
    // check if inside
    if (this->scene->intersect(shadow, i)) {
        // check if material is translucent for refraction
        auto newPos = shadow.getPosition() + i.getT() * shadow.getDirection();
        if (i.getMaterial().Trans()) {
            // check if inside
            if (glm::dot(shadow.getDirection(), i.getN()) > 0) {
                double d = i.getT();
                auto trans = i.getMaterial().kt(i);
                glm::dvec3 atten(std::pow(trans[0], d), std::pow(trans[1], d),
                                 std::pow(trans[2], d));

                shadow.setPosition(newPos +
                                   2 * RAY_EPSILON * shadow.getDirection());
                atten *= this->shadowAttenuation(shadow, shadow.getPosition());
                if (traceUI->isThreshold() &&
                    glm::dot(atten, atten) < traceUI->getThreshold())
                    return glm::dvec3(0.0);

                return atten;
            } else {
                shadow.setPosition(newPos +
                                   RAY_EPSILON * shadow.getDirection());
                isect i2;
                if (scene->intersect(shadow, i2)) {
                    double d = i2.getT();
                    glm::dvec3 newPos2 =
                        shadow.getPosition() + d * shadow.getDirection();
                    glm::dvec3 posOff = 2 * RAY_EPSILON * shadow.getDirection();
                    auto trans = i.getMaterial().kt(i);

                    glm::dvec3 atten(std::pow(trans[0], d),
                                     std::pow(trans[1], d),
                                     std::pow(trans[2], d));
                    ray sRay2(posOff, shadow.getDirection(), glm::dvec3(1.0),
                              ray::SHADOW);
                    return atten * this->shadowAttenuation(sRay2, posOff);
                } else {
                    return glm::dvec3(1.0, 1.0, 1.0);
                }
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

double AreaLight::distanceAttenuation(const glm::dvec3& P) const {
    double d = glm::distance(position, P);
    return 1.0 / (constantTerm + linearTerm * d + quadraticTerm * d * d);
}

glm::dvec3 AreaLight::getColor() const { return color; }

glm::dvec3 AreaLight::getDirection(const glm::dvec3& P) const {
    return glm::normalize(position - P);
}

glm::dvec3 AreaLight::shadowAttenTail(const ray& r, const glm::dvec3& p) const {
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
                atten *= this->shadowAttenTail(shadow, shadow.getPosition());
                if (traceUI->isThreshold() &&
                    glm::dot(atten, atten) < traceUI->getThreshold())
                    return glm::dvec3(0.0);
                return atten;
            } else {
                shadow.setPosition(shadow.at(i) + i.getN() * -RAY_EPSILON);
                return this->shadowAttenTail(shadow, shadow.getPosition());
            }
        } else {
            return glm::dvec3(0.0, 0.0, 0.0);
        }
    } else {
        return glm::dvec3(1.0, 1.0, 1.0);
    }
}

glm::dvec3 AreaLight::shadowAttenuation(const ray& r,
                                        const glm::dvec3& p) const {
    isect i;
    ray shadow(r);
    // calculate length
    double diff = glm::length(p - position);

    glm::dvec3 ret = glm::dvec3(1.0);

    // Compute local coordinate system
    glm::dvec3 P = shadow.at(diff);
    double min_abs = -1;
    glm::dvec3 U, V;
    if (abs(P[0]) < abs(P[1]) && abs(P[0]) < abs(P[2])) {
        U = glm::dvec3(0, -P[2], P[1]);
        V = glm::cross(U, P);
    } else if (abs(P[1]) < abs(P[0]) && abs(P[1]) < abs(P[2])) {
        U = glm::dvec3(-P[2], 0, P[0]);
        V = glm::cross(U, P);
    } else {
        U = glm::dvec3(-P[1], P[0], 0);
        V = glm::cross(U, P);
    }

    U = glm::normalize(U);
    V = glm::normalize(V);
    P = glm::normalize(P);

    std::random_device rd1;
    std::mt19937 eng1(rd1());
    std::uniform_real_distribution<double> distRadius(0, radius);
    std::uniform_real_distribution<double> distPhi(0, 2 * glm::pi<double>());
    shadow.setPosition(p - r.getDirection() * 1e-7);

    int val = 150;
    for (int k = 0; k < val; k++) {
        auto phi = distPhi(eng1);
        auto rad = distRadius(eng1);

        glm::dvec3 offVec =
            position + (glm::cos(phi) * V + glm::sin(phi) * U) * rad;
        shadow.setDirection(glm::normalize(offVec));

        ret += this->shadowAttenTail(shadow, -offVec);
    }
    ret /= val;
    return ret;
}

#define VERBOSE 0
