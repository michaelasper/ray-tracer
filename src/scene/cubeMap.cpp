#include "cubeMap.h"
#include "ray.h"
#include "../ui/TraceUI.h"
#include "../scene/material.h"
extern TraceUI *traceUI;

glm::dvec3 CubeMap::getColor(ray r) const
{
	// YOUR CODE HERE
	// FIXME: Implement Cube Map here
	glm::dvec3 d = r.getDirection();
	double x = d[0];
	double y = d[1];
	double z = d[2];

	double absX = fabs(x);
	double absY = fabs(y);
	double absZ = fabs(z);

	double u = 0.0;
	double v = 0.0;
	double axis;
	int index = -1;


	// POSITIVE X
	if (x > 0 && absX >= absY && absX >= absZ)
	{
		axis = absX;
		u = -z;
		v = y;
		index = 0;
	}
	// NEGATIVE X
	if (x <= 0 && absX >= absY && absX >= absZ)
	{
		axis = absX;
		u = z;
		v = y;
		index = 1;
	}
	// POSITIVE Y
	if (y > 0 && absY >= absX && absY >= absZ)
	{
		axis = absY;
		u = x;
		v = -z;
		index = 2;
	}
	// NEGATIVE Y
	if (y <= 0 && absY >= absX && absY >= absZ)
	{
		axis = absY;
		u = x;
		v = z;
		index = 3;
	}
	// POSITIVE Z
	if (z > 0 && absZ >= absX && absZ >= absY)
	{
		axis = absZ;
		u = x;
		v = y;
		index = 4;
	}
	// NEGATIVE Z
	if (z <= 0 && absZ >= absX && absZ >= absY)
	{
		axis = absZ;
		u = -x;
		v = y;
		index = 5;
	}

	// Convert range from -1 to 1 to 0 to 1
	u = 0.5f * (u/axis + 1.0f);
	v = 0.5f * (v/axis + 1.0f);
	return tMap[index]->getMappedValue(glm::dvec2(u, v));
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::setNthMap(int n, TextureMap *m)
{
	if (m != tMap[n].get())
		tMap[n].reset(m);
}
