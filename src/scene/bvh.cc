#include "bvh.h"
#include <stack>

bool BVH::getIntersection(ray& r, isect& i) const {
    if (size == 0) false;

    double threshold = EPSILON;
    std::stack<BVHTraversalItem> stack;

    BVHTraversalItem item{0, -EPSILON};
    stack.push(item);

    while (!stack.empty()) {
        BVHTraversalItem& currentNode(stack.top());
        stack.pop();

        int index = currentNode.i;
        double t = currentNode.min_t;

        const BVHNode& node = flatTree[index];
        if (t > threshold) continue;

        if (node.offset == 0) {
            for (int j = 0; j < node.prims; j++) {
                const Geometry* obj = objects[node.start + j];
                isect i2;
                if (obj->intersect(r, i2) && i2.getT() < t) {
                    i = i2;
                    threshold = i2.getT();
                }
            }
        } else {
            double t_min_right, t_max_right, t_min_left, t_max_left;
            bool hitR = flatTree[index + node.offset].box.intersect(
                r, t_min_right, t_max_right);
            bool hitL =
                flatTree[index + 1].box.intersect(r, t_min_left, t_max_left);

            if (hitR && hitL) {
                if (t_min_right < t_min_left) {
                    BVHTraversalItem leftNode{index + 1, t_min_left};
                    BVHTraversalItem rightNode{index + node.offset,
                                               t_min_right};
                    stack.push(leftNode);
                    stack.push(rightNode);

                } else {
                    BVHTraversalItem leftNode{index + 1, t_min_left};
                    BVHTraversalItem rightNode{index + node.offset,
                                               t_min_right};
                    stack.push(rightNode);
                    stack.push(leftNode);
                }
            } else if (hitR) {
                BVHTraversalItem rightNode{index + node.offset, t_min_right};
                stack.push(rightNode);

            } else if (hitL) {
                BVHTraversalItem leftNode{index + 1, t_min_left};
                stack.push(leftNode);
            }
        }
    }

    return threshold < EPSILON - 0.1;
}

void BVH::construct() {
    for (auto iter = scene->beginObjects(); iter != scene->endObjects();
         iter++) {
        Geometry* geo = iter->get();
        objects.push_back(geo);
        // }
    }

    // std::cout << "objects: " << objects.size() << std::endl;

    // initialize variables
    this->size = 0;
    this->leafs = 0;

    // error checking
    if (objects.size() == 0) return;

    // creating stack
    std::stack<BVHStackItem> stack;

    // create first object
    BVHStackItem item{-1, 0, static_cast<int>(objects.size())};
    stack.push(item);

    std::vector<BVHNode> nodes;
    nodes.resize(objects.size() * 2);

    while (!stack.empty()) {
        BVHStackItem& currentNode(stack.top());
        stack.pop();

        // local variables
        int start = currentNode.start;
        int end = currentNode.end;
        int prims = end - start;

        BVHNode node;
        node.start = start;
        node.prims = prims;
        node.offset = -1;

        BoundingBox bb = objects[start]->getBoundingBox();
        BoundingBox bc(bb.getCenter());

        for (int p = start + 1; p < end; p++) {
            BoundingBox tempbb = objects[p]->getBoundingBox();
            BoundingBox tempbc(tempbb.getCenter());
            bb.merge(tempbb);
            bc.merge(tempbc);
        }

        node.box = bb;

        std::cout << bb.volume() << std::endl;

        if (prims <= 1) {
            node.offset = 0;
            leafs++;
        }

        nodes[size++] = node;

        if (currentNode.parent != -1) {
            nodes[currentNode.parent].offset--;
            if (nodes[currentNode.parent].offset == -3) {
                nodes[currentNode.parent].offset =
                    size - 1 - currentNode.parent;
            }
        }

        if (node.offset == 0) continue;

        int splitDimension = bc.getMaxD();
        double split =
            0.5 * (bc.getMax()[splitDimension] + bc.getMin()[splitDimension]);

        int mid = start;
        for (int i = start; i < end; i++) {
            BoundingBox temp = objects[i]->getBoundingBox();
            if (temp.getCenter()[splitDimension] < split) {
                std::swap(objects[i], objects[mid]);
                mid++;
            }
        }

        if (mid == start || mid == end) {
            mid = start + (end - start) / 2;
        }

        BVHStackItem right;
        BVHStackItem left;

        right.start = mid;
        right.end = end;
        right.parent = size - 1;

        stack.push(right);

        left.start = start;
        left.end = mid;
        left.parent = size - 1;
        stack.push(left);
    }

    flatTree.resize(size);
    for (int i = 0; i < size; i++) flatTree[i] = nodes[i];

    std::cout << "objects: " << flatTree.size() << std::endl;
}