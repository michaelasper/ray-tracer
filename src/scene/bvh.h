
#pragma once

#include <iostream>
#include <vector>
#include "bbox.h"
#include "ray.h"
#include "scene.h"

#define EPSILON 99999999.0

class Geometry;
class Scene;
class MaterialSceneObject;
class Trimesh;
class TrimeshFace;

struct BVHNode {
    BoundingBox box;
    int start, prims, offset;
};

struct BVHStackItem {
    int parent, start, end;
};

struct BVHTraversalItem {
    int i;
    double min_t;
};

class BVH {
   public:
    BVH(const Scene* scene) { this->scene = scene; }
    void construct();
    bool getIntersection(ray& r, isect& i) const;

   private:
    int size;
    int leafs;
    const Scene* scene;
    std::vector<Geometry*> objects;
    std::vector<BVHNode> flatTree;
};