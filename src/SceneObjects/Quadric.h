#ifndef __QUADRIC_H__
#define __QUADRIC_H__

#include "../scene/scene.h"

class Quadric
	: public MaterialSceneObject
{
public:
	Quadric( Scene *scene, Material *mat )
		: MaterialSceneObject( scene, mat )
	{
		A = -1.0;
		B = 1.0;
		C = 1.0;
		D = 0.0;
		E = 0.0;
		F = 0.0;
		G = 0.0;
		H = 0.0;
		I = 0.0;
		J = -1.0;
	}
    
	virtual bool intersectLocal(ray& r, isect& i ) const;
	virtual bool hasBoundingBoxCapability() const { return true; }

    virtual BoundingBox ComputeLocalBoundingBox()
    {
        BoundingBox localbounds;
		localbounds.setMin(glm::dvec3(-1.0f, -1.0f, -1.0f));
		localbounds.setMax(glm::dvec3(1.0f, 1.0f, 1.0f));
        return localbounds;
    }

protected:
    double A, B, C, D, E, F, G, H, I, J;
	void glDrawLocal(int quality, bool actualMaterials, bool actualTextures) const;
};
#endif // __QUADRIC_H__

