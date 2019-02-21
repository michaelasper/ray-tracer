#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "../scene/scene.h"

class Circle : public MaterialSceneObject {
   public:
    Circle(Scene* scene, Material* mat, double r = 0.5)
        : MaterialSceneObject(scene, mat) {
        radius = r;
    }

    virtual bool intersectLocal(ray& r, isect& i) const;
    virtual bool hasBoundingBoxCapability() const { return true; }

    virtual BoundingBox ComputeLocalBoundingBox() {
        BoundingBox localbounds;
        localbounds.setMin(glm::dvec3(-0.5f, -0.5f, -RAY_EPSILON));
        localbounds.setMax(glm::dvec3(0.5f, 0.5f, RAY_EPSILON));
        return localbounds;
    }

   protected:
    void glDrawLocal(int quality, bool actualMaterials,
                     bool actualTextures) const;
    double radius;
};

#endif  // __Circle_H__
