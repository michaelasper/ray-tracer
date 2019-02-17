#include "bvh.h"
#include <stack>

#include <fstream>
#include <iostream>
#define EPSILON 9e6

bool BVH::getIntersection(const ray& _r, const isect& _i) {
    ray r(_r);
    isect i(_i);
    uint32_t close, other;
    double threshold = EPSILON;
    double hit1 = 0;
    double hit2 = 0;
    double hit3 = 0;
    double hit4 = 0;

    std::stack<BVHTraversal> s;
    s.push(BVHTraversal{0, -EPSILON});
    int counter = 0;

    while (!s.empty()) {
        BVHTraversal& bnode(s.top());
        s.pop();

        uint32_t node_index = bnode.i;
        double t = bnode.min_t;

        const BVHFlatNode& node(flatTree[node_index]);

        if (t > threshold) continue;

        if (node.rightOffset == 0) {
            counter++;
            for (int k = 0; k < node.nPrims; ++k) {
                isect i2;
                auto obj = objects[node.start + k];
                if (obj->intersect(r, i2) && i2.getT() < threshold) {
                    i = i2;
                }
            }
        } else {
            double t_min_r, t_max_r;
            bool hitR = flatTree[node_index + node.rightOffset].bbox.intersect(
                r, t_min_r, t_max_r);

            double t_min_l, t_max_l;
            bool hitL = flatTree[node_index + 1].bbox.intersect(r, t_min_l, t_max_l);

            

            if (hitL && hitR) {
                if(t_min_r < t_min_l){
                    s.push(BVHTraversal{node_index + 1, t_min_l});
                    s.push(BVHTraversal{node_index + node.rightOffset, t_min_r});
                } else {
                    s.push(BVHTraversal{node_index + node.rightOffset, t_min_r});
                    s.push(BVHTraversal{node_index + 1, t_min_l});

                }
            } else if (hitR) {
                s.push(BVHTraversal{node_index + node.rightOffset, t_min_r});
            }
            else if (hitL) {
                s.push(
                    BVHTraversal{node_index + 1,  t_min_l});
            } 
        }
    }

    return threshold < EPSILON - 0.1;
}

void BVH::construct() {
    for (auto iter = scene->beginObjects(); iter != scene->endObjects();
         iter++) {
        auto obj = iter->get();
        objects.push_back(obj);
    }
    std::stack<BVHBuildEntry> BVHstack;

    const uint32_t Untouched = 0xffffffff;
    const uint32_t TouchedTwice = 0xfffffffd;

    BVHstack.push(
        BVHBuildEntry{0, static_cast<uint32_t>(objects.size()), 0xfffffffc});

    BVHFlatNode node;
    std::vector<BVHFlatNode> nodes;
    nodes.reserve(objects.size() * 2);

    while (!BVHstack.empty()) {
        BVHBuildEntry& bnode(BVHstack.top());
        BVHstack.pop();

        uint32_t start = bnode.start;
        uint32_t end = bnode.end;
        uint32_t nPrims = end - start;

        this->size++;

        node.start = start;
        node.nPrims = nPrims;
        node.rightOffset = Untouched;

        BoundingBox bb(objects[start]->getBoundingBox());
        BoundingBox bc(objects[start]->getBoundingBox().getCenter(),
                       objects[start]->getBoundingBox().getCenter());

        for (uint32_t p = start + 1; p < end; ++p) {
            auto temp = objects[p]->getBoundingBox();
            bb.merge(temp);
            auto tempbc = BoundingBox(temp.getCenter(), temp.getCenter());
            bc.merge(tempbc);
        }

        node.bbox = bb;

        if (node.nPrims <= leaf_size) {
            node.rightOffset = 0;
            leafs++;
        }

        nodes.push_back(node);

        if (bnode.parent != 0xfffffffc) {
            nodes[bnode.parent].rightOffset--;

            // When this is the second touch, this is the right child.
            // The right child sets up the offset for the flat tree.
            if (nodes[bnode.parent].rightOffset == TouchedTwice) {
                nodes[bnode.parent].rightOffset = size - 1 - bnode.parent;
            }
        }

        if (node.rightOffset == 0) continue;

        int d = bc.getMaxD();

        double splitAxis = .5 * (bc.getMin()[d] + bc.getMax()[d]);

        uint32_t mid = start;
        for (uint32_t i = start; i < end; ++i) {
            if (objects[i]->getBoundingBox().getCenter()[d] < splitAxis) {
                std::swap(objects[i], objects[mid]);
                ++mid;
            }
        }

        if (mid == start || mid == end) {
            mid = start + (end - start) / 2;
        }

        BVHstack.push(BVHBuildEntry{mid, end, size - 1});
        BVHstack.push(BVHBuildEntry{start, mid, size - 1});
    }

    flatTree = new BVHFlatNode[size];
    for (uint32_t n = 0; n < size; ++n) flatTree[n] = nodes[n];
}