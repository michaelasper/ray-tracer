#pragma once

#include "bbox.h"
#include "ray.h"
#include "scene.h"

class Geometry;
class Scene;
class MaterialSceneObject;
class Trimesh;
class TrimeshFace;

class BVH {
   private:
    struct node {
        Geometry* box;
        node* left;
        node* right;
    };

    struct BVHBuildEntry {
        // If non-zero then this is the index of the parent. (used in offsets)
        uint32_t parent;
        // The range of objects in the object list covered by this node.
        uint32_t start, end;
    };

    struct BVHTraversal {
        uint32_t i;    // Node
        double min_t;  // Minimum hit time for this node.
    };

    struct BVHFlatNode {
        BoundingBox* bbox;
        uint32_t start, nPrims, rightOffset;
    };

   public:
    node* root;
    uint32_t size, leaf_size, leafs;
    BVHFlatNode* flatTree;
    std::vector<Geometry*> objects;

    Scene* scene;
    BVH(Scene* scene) : scene(scene), size(0), leaf_size(4), leafs(0) {
        this->root = NULL;
        construct();
    };

    void construct();

    bool getIntersection(const ray& _r, const isect& _i);
};