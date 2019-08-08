#include <Origin.h>
#include <wks2mat.h>
#include "RayShapeIntersect.c"

void TestRectangleIntersectionCode()
{
	// build ray
	Point ray_origin;
	ray_origin.x = 50;
	ray_origin.y = -3;
	
	double ray_angle = 4 *pi/180;
	
	
	// build rectangle
	double X[5], Y[5];
	X[0] = -110; 	X[1] = 0; 		X[2] = 110; 	X[3] = 0;
	Y[0] = 0; 		Y[1] = -110; 	Y[2] = 0; 		Y[3] = 110;
	Point rect[5];
	GetPolygonFromPoints(&rect, &X, &Y, 4);
	
	
	// get intersection
	Point return_point;
	bool does_intersect = GetRay2PolygonIntersectionPoint(return_point, ray_origin, ray_angle, rect, 4);
	
	return;
}