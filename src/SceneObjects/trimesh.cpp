#include "trimesh.h"
#include <assert.h>
#include <float.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

using namespace std;

Trimesh::~Trimesh() {
    for (auto m : materials) delete m;
    for (auto f : faces) delete f;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex(const glm::dvec3& v) { vertices.emplace_back(v); }

void Trimesh::addMaterial(Material* m) { materials.emplace_back(m); }

void Trimesh::addNormal(const glm::dvec3& n) { normals.emplace_back(n); }

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace(int a, int b, int c) {
    int vcnt = vertices.size();

    if (a >= vcnt || b >= vcnt || c >= vcnt) return false;

    TrimeshFace* newFace =
        new TrimeshFace(scene, new Material(*this->material), this, a, b, c);
    newFace->setTransform(this->transform);
    if (!newFace->degen)
        faces.push_back(newFace);
    else
        delete newFace;

    // Don't add faces to the scene's object list so we can cull by bounding
    // box
    return true;
}

// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
const char* Trimesh::doubleCheck() {
    if (!materials.empty() && materials.size() != vertices.size())
        return "Bad Trimesh: Wrong number of materials.";
    if (!normals.empty() && normals.size() != vertices.size())
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

bool Trimesh::intersectLocal(ray& r, isect& i) const {
    bool have_one = false;
    for (auto face : faces) {
        isect cur;
        if (face->intersectLocal(r, cur)) {
            if (!have_one || (cur.getT() < i.getT())) {
                i = cur;
                have_one = true;
            }
        }
    }
    if (!have_one) i.setT(1000.0);
    return have_one;
}

bool TrimeshFace::intersect(ray& r, isect& i) const {
    return intersectLocal(r, i);
}

// Intersect ray r with the triangle abc.  If it hits returns true,
// and put the parameter in t and the barycentric coordinates of the
// intersection in u (alpha) and v (beta).

bool check_cond(glm::dvec3 a, glm::dvec3 b, glm::dvec3 c, glm::dvec3 q,
                glm::dvec3 normal) {
    bool cond1 = glm::dot(glm::cross(b - a, q - a), normal) >= 0;
    bool cond2 = glm::dot(glm::cross(c - b, q - b), normal) >= 0;
    bool cond3 = glm::dot(glm::cross(a - c, q - c), normal) >= 0;

    return cond1 && cond2 && cond3;
}

bool TrimeshFace::intersectLocal(ray& r, isect& i) const {
    // YOUR CODE HERE
    //
    // FIXME: Add ray-trimesh intersection

    glm::dvec3 a_coords = parent->vertices[ids[0]];
    glm::dvec3 b_coords = parent->vertices[ids[1]];
    glm::dvec3 c_coords = parent->vertices[ids[2]];

    glm::dvec3 p = r.getPosition();
    glm::dvec3 d = r.getDirection();

    // The vector is parallel to the plane it doesn't intersect the plane.
    if (glm::dot(normal, d) == 0) {
        return false;
    }

    double dval = glm::dot(normal, a_coords);

    double t = (dval - glm::dot(normal, p)) / glm::dot(normal, d);
    if (t <= RAY_EPSILON) return false;
    glm::dvec3 q = r.at(t);

    // Figure out if the ray is inside the triangle

    if (check_cond(a_coords, b_coords, c_coords, q, normal)) {
        // Compute areas
        double alpha =
            glm::dot(glm::cross(c_coords - b_coords, q - b_coords), normal) /
            glm::dot(glm::cross(b_coords - a_coords, c_coords - a_coords),
                     normal);
        double beta =
            glm::dot(glm::cross(a_coords - c_coords, q - c_coords), normal) /
            glm::dot(glm::cross(b_coords - a_coords, c_coords - a_coords),
                     normal);
        double gamma =
            glm::dot(glm::cross(b_coords - a_coords, q - a_coords), normal) /
            glm::dot(glm::cross(b_coords - a_coords, c_coords - a_coords),
                     normal);

        // if (!parent->vertNorms) parent->generateNormals();
        glm::dvec3 interp;
        if (parent->vertNorms) {
            interp = glm::normalize(alpha * parent->normals[ids[0]] +
                                    beta * parent->normals[ids[1]] +
                                    gamma * parent->normals[ids[2]]);
            if (parent->materials.size() > 0) {
                Material accumulate = (alpha * *(parent->materials[ids[0]]));
                accumulate += (beta * *(parent->materials[ids[1]]));
                accumulate += (gamma * *(parent->materials[ids[2]]));
                i.setMaterial(accumulate);
            } else {
                i.setMaterial(this->getMaterial());
            }
        } else {
            interp = glm::normalize(normal);
            i.setMaterial(this->getMaterial());
        }

        i.setT(t);
        i.setN(interp);
        i.setObject(this);
        i.setBary(alpha, beta, gamma);
        i.setUVCoordinates(glm::dvec2(alpha, beta));
        return true;
    }

    return false;
}

// Once all the verts and faces are loaded, per vertex normals can be
// generated by averaging the normals of the neighboring faces.
void Trimesh::generateNormals() {
    int cnt = vertices.size();
    normals.resize(cnt);
    std::vector<int> numFaces(cnt, 0);

    for (auto face : faces) {
        glm::dvec3 faceNormal = face->getNormal();

        for (int i = 0; i < 3; ++i) {
            normals[(*face)[i]] += faceNormal;
            ++numFaces[(*face)[i]];
        }
    }

    for (int i = 0; i < cnt; ++i) {
        if (numFaces[i]) normals[i] /= numFaces[i];
    }

    vertNorms = true;
}
