#ifndef __LIGHT_H__
#define __LIGHT_H__

#ifndef _WIN32
#include <algorithm>
#include <random>
using std::max;
using std::min;
#endif

#include <FL/gl.h>
#include "../ui/TraceUI.h"
#include "scene.h"

class Light : public SceneElement {
   public:
    virtual glm::dvec3 shadowAttenuation(const ray& r,
                                         const glm::dvec3& pos) const = 0;
    virtual double distanceAttenuation(const glm::dvec3& P) const = 0;
    virtual glm::dvec3 getColor() const = 0;
    virtual glm::dvec3 getDirection(const glm::dvec3& P) const = 0;

   protected:
    Light(Scene* scene, const glm::dvec3& col)
        : SceneElement(scene), color(col) {
        // std::mt19937 eng(rd());
    }

    glm::dvec3 color;
    // std::random_device rd;

    // std::mt19937 eng;

   public:
    virtual void glDraw(GLenum lightID) const {}
    virtual void glDraw() const {}
};

class DirectionalLight : public Light {
   public:
    DirectionalLight(Scene* scene, const glm::dvec3& orien,
                     const glm::dvec3& color)
        : Light(scene, color), orientation(glm::normalize(orien)) {}
    virtual glm::dvec3 shadowAttenuation(const ray& r,
                                         const glm::dvec3& pos) const;
    virtual double distanceAttenuation(const glm::dvec3& P) const;
    virtual glm::dvec3 getColor() const;
    virtual glm::dvec3 getDirection(const glm::dvec3& P) const;

   protected:
    glm::dvec3 orientation;

   public:
    void glDraw(GLenum lightID) const;
    void glDraw() const;
};

class PointLight : public Light {
   public:
    PointLight(Scene* scene, const glm::dvec3& pos, const glm::dvec3& color,
               float constantAttenuationTerm, float linearAttenuationTerm,
               float quadraticAttenuationTerm)
        : Light(scene, color),
          position(pos),
          constantTerm(constantAttenuationTerm),
          linearTerm(linearAttenuationTerm),
          quadraticTerm(quadraticAttenuationTerm) {}

    virtual glm::dvec3 shadowAttenuation(const ray& r,
                                         const glm::dvec3& pos) const;
    virtual double distanceAttenuation(const glm::dvec3& P) const;
    virtual glm::dvec3 getColor() const;
    virtual glm::dvec3 getDirection(const glm::dvec3& P) const;

    void setAttenuationConstants(float a, float b, float c) {
        constantTerm = a;
        linearTerm = b;
        quadraticTerm = c;
    }

   protected:
    glm::dvec3 position;

    // These three values are the a, b, and c in the distance
    // attenuation function (from the slide labelled
    // "Intensity drop-off with distance"):
    //    f(d) = min( 1, 1/( a + b d + c d^2 ) )
    float constantTerm;   // a
    float linearTerm;     // b
    float quadraticTerm;  // c

   public:
    void glDraw(GLenum lightID) const;
    void glDraw() const;

   protected:
};

// Disk
class AreaLight : public Light {
   public:
    AreaLight(Scene* scene, const glm::dvec3& pos, float r,
              const glm::dvec3& color, float constantAttenuationTerm,
              float linearAttenuationTerm, float quadraticAttenuationTerm)
        : Light(scene, color),
          position(pos),
          radius(r),
          constantTerm(constantAttenuationTerm),
          linearTerm(linearAttenuationTerm),
          quadraticTerm(quadraticAttenuationTerm) {}

    virtual glm::dvec3 shadowAttenTail(const ray& r,
                                       const glm::dvec3& pos) const;
    virtual glm::dvec3 shadowAttenuation(const ray& r,
                                         const glm::dvec3& pos) const;
    virtual double distanceAttenuation(const glm::dvec3& P) const;
    virtual glm::dvec3 getColor() const;
    virtual glm::dvec3 getDirection(const glm::dvec3& P) const;

    void setAttenuationConstants(float a, float b, float c) {
        constantTerm = a;
        linearTerm = b;
        quadraticTerm = c;
    }

   protected:
    glm::dvec3 position;
    float radius;
    // These three values are the a, b, and c in the distance
    // attenuation function (from the slide labelled
    // "Intensity drop-off with distance"):
    //    f(d) = min( 1, 1/( a + b d + c d^2 ) )
    float constantTerm;   // a
    float linearTerm;     // b
    float quadraticTerm;  // c

   public:
    void glDraw(GLenum lightID) const;
    void glDraw() const;

   protected:
};

#endif  // __LIGHT_H__
