#include <cmath>

#include "Torus.h"
#include "solve.h"
#include <glm/gtx/io.hpp>
#include <iostream>
#include <algorithm>

using namespace std;


bool Torus::intersectLocal(ray& r, isect& i) const
{
    glm::dvec3 p = r.getPosition();
    glm::dvec3 d = r.getDirection();

    double mr = minor_r;
    double R = major_r;

    double pp = glm::dot(p, p);
    double dd = glm::dot(d, d);
    double pd = glm::dot(p, d);

    
    double c_0 = pow(pp - (mr*mr + R*R), 2.0) - 4.0*R*R*(mr*mr - p[1]*p[1]);
    double c_1 = 4.0*(pp - (mr*mr + R*R))*pd + 8.0*R*R*p[1]*d[1];
    double c_2 = 2.0*dd*(pp - (mr*mr + R*R)) + 4.0*(pd*pd) + 4.0*R*R*d[1]*d[1];
    double c_3 = 4.0*dd*pd;
    double c_4 = dd*dd;
    

    double c[5];
    c[0] = c_0;
    c[1] = c_1;
    c[2] = c_2;
    c[3] = c_3;
    c[4] = c_4; 

    double s[4];

    int roots = SolveQuartic(c, s);
    if(roots == 0) {
        return false;
    }   

	i.setObject(this);
	i.setMaterial(this->getMaterial());

    //for(int i = 0; i<4; i++) {
    //    cout << s[i] << endl;
    //}

    double min_root = 1000.0;
    for(int j = 0; j<4; j++) {
        if(s[j] > RAY_EPSILON && s[j] < min_root) {
            min_root = s[j];
        }
    }

    i.setT(min_root);

    // Compute the normal
    /*
    double alpha = R / sqrt(p[0]*p[0] + p[1]*p[1]);
    glm::vec3 N = glm::dvec3((1-alpha)*p[0], (1-alpha)*p[1], p[2]);
    i.setN(glm::normalize(N));
    */
   i.setN(glm::normalize(r.at(min_root)));
   /*
    double paramSquared = R*R + mr*mr;

    double x = p[0];
    double y = p[1];
    double z = p[2];
    double sumSquared = x * x + y * y + z * z;
    glm::dvec3 tmp = glm::dvec3(4.0*x*(sumSquared - paramSquared), 
        4.0*y*(sumSquared - paramSquared) + 2.0*R*R, 
        4.0*z*(sumSquared - paramSquared));
    i.setN(glm::normalize(tmp));
    */
    return true;
}

