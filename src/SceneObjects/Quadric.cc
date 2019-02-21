#include <cmath>

#include "Quadric.h"
using namespace std;

bool Quadric::intersectLocal(ray& r, isect& i) const {
    glm::dvec3 p = r.getPosition();
    glm::dvec3 d = r.getDirection();

    double xo = p[0];
    double yo = p[1];
    double zo = p[2];

    double xd = d[0];
    double yd = d[1];
    double zd = d[2];

    double Aq = A * xd * xd + B * yd * yd + C * zd * zd + D * xd * yd +
                E * xd * zd + F * yd * zd;
    double Bq = 2 * A * xo * xd + 2 * B * yo * yd + 2 * C * zo * zd +
                D * (xo * yd + yo * xd) + E * (xo * zd + zo * xd) +
                F * (yo * zd + yd * zo) + G * xd + H * yd + I * zd;
    double Cq = A * xo * xo + B * yo * yo + C * zo * zo + D * xo * yo +
                E * xo * zo + F * yo * zo + G * xo + H * yo + I * zo + J;

    double t = 0.0;
    double disc = Bq * Bq - 4 * Aq * Cq;

    if (Aq == 0) {
        t = -Cq / Bq;
    } else {
        if (disc < 0.0) {
            return false;
        } else {
            double t0 = (-Bq - sqrt(disc)) / (2 * Aq);
            if (t0 > RAY_EPSILON) {
                t = t0;
            } else {
                double t1 = (-Bq + sqrt(disc)) / (2 * Aq);
                if (t1 > RAY_EPSILON) {
                    t = t1;
                } else {
                    return false;
                }
            }
        }
    }

    i.setObject(this);
    i.setMaterial(this->getMaterial());
    i.setT(t);

    glm::dvec3 P = r.at(t);

    double xn = 2 * A * P[0] + D * P[1] + E * P[2] + G;
    double yn = 2 * B * P[1] + D * P[0] + F * P[2] + H;
    double zn = 2 * C * P[2] + E * P[0] + F * P[1] + I;

    glm::dvec3 normal = glm::normalize(glm::dvec3(xn, yn, zn));

    if (glm::dot(normal, d) > 0) {
        i.setN(-normal);
    } else {
        i.setN(normal);
    }

    return true;
}