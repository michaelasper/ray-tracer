#include <cmath>

#include "Torus.h"
#include "solve.h"
#include <glm/gtx/io.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

/*
References:
Pg. 93 of "An Introduction to Ray Tracing", Glassner.
Computing Normals - http://cosinekitty.com/raytrace/chapter13_torus.html 

*/
bool Torus::intersectLocal(ray& r, isect& i) const
{
    glm::dvec3 p = r.getPosition();
    glm::dvec3 d = glm::normalize(r.getDirection());

    double pp = glm::dot(p, p);
    double dd = glm::dot(d, d);
    double pd = glm::dot(p, d);

    double a = major_r;
    double b = minor_r;

    double c_4 = dd*dd;
    double c_3 = 4.0*pd*dd;
    double c_2 = 2.0*dd*(pp - (a*a + b*b)) + 4.0*pd*pd + 4.0*a*a*d[2]*d[2];
    
    double c_1 = 4.0 * pd * ((pp)-(a*a+b*b))
           + 8*a*a*p[2] * d[2];
    double c_0 = (pp-(a*a+b*b))*(pp-(a*a+b*b)) - 4.0*a*a*(b*b-p[2]* p[2]);

    double c[5] = {0.0,0.0,0.0,0.0,0.0};
    c[0] = c_0;
    c[1] = c_1;
    c[2] = c_2;
    c[3] = c_3;
    c[4] = c_4; 

    double s[4] = {0.0,0.0,0.0,0.0};

    int roots = SolveQuartic(c, s);
    if(roots == 0) {
        return false;
    }   

    double min_root = 1000000.0;
    for(int j = 0; j<4; j++) {
        if(s[j] > RAY_EPSILON && s[j] < min_root) {
            min_root = s[j];
        }
    }

    if(min_root == 1000000.0) {
        return false;
    }
    
    i.setT(min_root);

    // Compute the normal
    glm::dvec3 P = r.at(min_root);
    double alpha = a / sqrt(P[0]*P[0] + P[1]*P[1]);
    glm::vec3 N = glm::dvec3((1-alpha)*P[0], (1-alpha)*P[1], P[2]);
    i.setN(glm::normalize(N));
	i.setObject(this);
	i.setMaterial(this->getMaterial());

    return true;
}

