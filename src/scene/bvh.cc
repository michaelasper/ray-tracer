#include "bvh.h"

bool BVH::getIntersection(ray& r, isect& i) const { return false; }

void BVH::construct() {
    for (auto iter = scene->beginObjects(); iter != scene->endObjects();
         iter++) {
        Geometry* geo = iter->get();
        // if (geo->isTrimesh()) {
        //     Trimesh* trimesh = (Trimesh*)geo;
        //     for (auto face = trimesh->beginFace(); face !=
        //     trimesh->endFace();
        //          face++) {
        //         (*face)->ComputeBoundingBox();
        //         objects.push_back((Geometry*)(*face));
        //     }
        // } else {
        // geo->ComputeBoundingBox();
        objects.push_back(geo);
        // }
    }
}