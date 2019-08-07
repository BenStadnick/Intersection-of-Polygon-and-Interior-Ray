/************************************************************************************
Given a ray (point+direction) and polygon coordinates (e.g. a rotated rectangle ), 
this will calculate an intersection of the ray with the polygon. Note, if the origin is 
on the edge (within error) we don't count it as not an intersection. This way, when the 
user is positioned by the stage limits, they cannot move further into the limits

*originally written for OriginC compiler
************************************************************************************/
#include <Origin.h>


 struct Point
{
	double x, y;
};

// get rectangle in Point structures from x and y arrays
// size of rect, x and y must be num_edges+1
static void GetPolygonFromPoints(Point* rect, double*x, double* y, int num_edges )
{
	// build rectangle from points
	for(int ii = 0; ii < num_edges; ii++)
	{
		rect[ii].x = x[ii];
		rect[ii].y = y[ii];
	}
	// last point must match first
	rect[num_edges].x = x[0];
	rect[num_edges].y = y[0];
}

// find intersection of ray given its origin and angle and a rectangle (can be rotated) given the coordinates of its 4 corners
// ray_angle given in radians
static bool GetRay2PolygonIntersectionPoint(Point& return_point, const Point& ray_origin, double ray_angle, Point* rect, int num_edges)
{
	// create segment to represent the ray
	Point seg[2];
	seg[0] = ray_origin;
	const double big_length = 9999; // should be large so that the second point of 'seg' is guaranteed to be outside of rectangle
	seg[1].x = ray_origin.x + cos(ray_angle)*big_length;
	seg[1].y = ray_origin.y + sin(ray_angle)*big_length;
	
	return GetSegment2PolygonIntersectionPoint(return_point, seg, rect, num_edges)
}

// returns false if no intersection of segment and polygon
// else returns true and a valid intersection point, return_point
static bool GetSegment2PolygonIntersectionPoint(Point& return_point, Point* seg, Point* polygon, int num_edges)
{
	for(int ii=0; ii < num_edges; ii++)
	{
		// if the segment origin is on the edge (within error) we don't count it as not an intersection, 
		// this way when the user is positioned by the stage limits, they cannot move further into the limits
		if( !IsPointOnLine( seg[0], polygon[ii], polygon[ii+1] ) 
			&& GetSegment2SegmentIntersectionPoint(return_point, seg[0], seg[1], polygon[ii], polygon[ii+1]) )
			return true;
	}
	
	return false; // should only happen when user is near limits and attempts to move further or if stage coordinates are bugged
}

// test if the point p0 lays within error of the line p1-p2
static bool IsPointOnLine( Point& p0, Point& p1, Point& p2 )
{
	const double allowable_error = 0.1;
	double dist_from_line = abs( (p2.x-p1.x)*(p1.y-p0.y) - (p1.x-p0.x)*(p2.y*p1.y) ) / sqrt( (p2.x-p1.x)^2.0 + (p2.y-p1.y)^2.0 );
	
	return (allowable_error > dist_from_line);
	
}

// tests if segments p and q intersect, if true it returns intersection point return_point
static bool GetSegment2SegmentIntersectionPoint(Point& return_point, const Point p, const Point p2, const Point q, const Point q2)
{
	if( !DoLineSegmentsIntersect(p, p2, q, q2) )
		return false;
	
	double denominator = (p.x - p2.x)*(q.y - q2.y) - (p.y - p2.y)*(q.x - q2.x);
	double p_cross = CrossProduct(p, p2);
	double q_cross = CrossProduct(q, q2);
	
	return_point.x = ( p_cross*(q.x - q2.x) - q_cross*(p.x - p2.x)) / denominator;
	return_point.y = ( p_cross*(q.y - q2.y) - q_cross*(p.y - p2.y)) / denominator;
	
	return (return_point.x != NANUM && return_point.y != NANUM);
}

// tests if two line segments intersect
static bool DoLineSegmentsIntersect(const Point& p, const Point& p2, const Point& q, const Point& q2) 
{
	Point r, s, d;
	r = SubtractPoints(p2, p);
	s = SubtractPoints(q2, q);
	d = SubtractPoints(q, p);
	
	double u_numerator = CrossProduct(d, r);
	double denominator = CrossProduct(r, s);

	if (u_numerator == 0 && denominator == 0) { return false; } // lines are collinear
	if (denominator == 0) { return false; } // lines are parallel

	double u = u_numerator / denominator;
	double t = CrossProduct(d, s) / denominator;

	return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

// calculates the cross product of the two points
static double CrossProduct(const Point& p1, const Point& p2)
{
	return p1.x * p2.y - p1.y * p2.x;
}

// subtracts Point p1 from p2
static Point SubtractPoints(const Point& p1, const Point& p2) 
{
	Point result;
	result.x = p1.x - p2.x;
	result.y = p1.y - p2.y;

	return result;
}

