#ifndef __TORUS_H__
#define __TORUS_H__

#include "../scene/scene.h"

class Torus
	: public MaterialSceneObject
{
public:
	Torus( Scene *scene, Material *mat, double mr = 0.3, double mR = 1 )
		: MaterialSceneObject( scene, mat )
	{
        minor_r = mr;
        major_r = mR;
	}

	virtual bool intersectLocal(ray& r, isect& i ) const;
	virtual bool hasBoundingBoxCapability() const { return true; }

    virtual BoundingBox ComputeLocalBoundingBox()
    {
        BoundingBox localbounds;
		localbounds.setMin(glm::dvec3(-major_r, -major_r, -major_r));
		localbounds.setMax(glm::dvec3(major_r, major_r, major_r));
        return localbounds;
    }

protected:
    double minor_r;
    double major_r;
protected:
	void glDrawLocal(int quality, bool actualMaterials, bool actualTextures) const;

};

#endif // __TORUS_H__
