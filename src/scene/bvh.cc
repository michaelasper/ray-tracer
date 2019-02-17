#include "bvh.h"
#include <stack>

#include <fstream>
#include <iostream>
#define EPSILON -9e9

bool BVH::getIntersection(const ray& _r, const isect& _i) {
    ray r(_r);
    isect i(_i);
    uint32_t close, other;

    double hit1 = 0;
    double hit2 = 0;
    double hit3 = 0;
    double hit4 = 0;

    std::stack<BVHTraversal> s;
    s.push(BVHTraversal{0, EPSILON});

    while (!s.empty()) {
        BVHTraversal& bnode = s.top();
        s.pop();

        int node_index = bnode.i;
        double t = bnode.min_t;

        BVHFlatNode node(flatTree[node_index]);

        if (t > i.getT()) continue;

        if (node.rightOffset == 0) {
            for (int k = 0; k < node.nPrims; ++k) {
                isect i2;
                auto obj = objects[node.start + k];
                if (obj->intersect(r, i2) && i2.getT() < t) {
                    i = i2;
                }
            }
        } else {
            bool hitL = flatTree[node_index + 1].bbox.intersect(r, hit1, hit2);

            bool hitR = flatTree[node_index + node.rightOffset].bbox.intersect(
                r, hit3, hit4);

            if (hitL && hitR) {
                close = node_index + 1;
                other = node_index + node.rightOffset;
                if (hit3 < hit1) {
                    std::swap(hit1, hit3);
                    std::swap(hit2, hit4);
                    std::swap(close, other);
                }
                s.push(BVHTraversal{other, hit3});
                s.push(BVHTraversal{close, hit1});
            } else if (hitL) {
                s.push(
                    BVHTraversal{static_cast<uint32_t>(node_index + 1), hit1});
            } else if (hitR) {
                s.push(BVHTraversal{node_index + node.rightOffset, hit3});
            }
        }
    }

    return i.getT() < EPSILON - 0.1;
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
        BVHBuildEntry& bnode = BVHstack.top();
        BVHstack.pop();

        this->size++;

        node.start = bnode.start;
        node.nPrims = bnode.end - bnode.start;
        ;
        node.rightOffset = Untouched;

        BoundingBox bb(objects[node.start]->getBoundingBox());
        BoundingBox bc(bb.getCenter(), bb.getCenter());

        for (uint32_t p = node.start + 1; p < bnode.end; ++p) {
            auto temp = objects[p]->getBoundingBox();
            bb.merge(temp);
            bc.merge(BoundingBox(temp.getCenter(), temp.getCenter()));
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

        uint32_t mid = bnode.start;
        for (uint32_t i = bnode.start; i < bnode.end; ++i) {
            if (objects[i]->getBoundingBox().getCenter()[d] < splitAxis) {
                std::swap(objects[i], objects[mid]);
                ++mid;
            }
        }

        if (mid == bnode.start || mid == bnode.end) {
            mid = bnode.start + (bnode.end - bnode.start) / 2;
        }

        BVHstack.push(BVHBuildEntry{mid, bnode.end, size - 1});
        BVHstack.push(BVHBuildEntry{bnode.start, mid, size - 1});
    }

    this->flatTree = new BVHFlatNode[size];
    for (uint32_t n = 0; n < size; ++n) flatTree[n] = nodes[n];
}