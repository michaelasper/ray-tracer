#ifndef __QUADRIC_H__
#define __QUADRIC_H__

#include <vector>
#include "../scene/scene.h"

class Quadric : public MaterialSceneObject {
   public:
    Quadric(Scene* scene, Material* mat) : MaterialSceneObject(scene, mat) {}

    virtual bool intersectLocal(ray& r, isect& i) const;
    virtual bool hasBoundingBoxCapability() const { return true; }

    virtual BoundingBox ComputeLocalBoundingBox() {
        BoundingBox localbounds;
        localbounds.setMin(glm::dvec3(-1.0f, -1.0f, -1.0f));
        localbounds.setMax(glm::dvec3(1.0f, 1.0f, 1.0f));
        return localbounds;
    }

    void addVertex(glm::dvec2 v) {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
    }

    void update() {
        A = vertices[0];
        B = vertices[1];
        C = vertices[2];
        D = vertices[3];
        E = vertices[4];
        F = vertices[5];
        G = vertices[6];
        H = vertices[7];
        I = vertices[8];
        J = vertices[9];
    }

   protected:
    double A, B, C, D, E, F, G, H, I, J;
    void glDrawLocal(int quality, bool actualMaterials,
                     bool actualTextures) const;
    std::vector<double> vertices;
};
#endif  // __QUADRIC_H__
