#include "Draw.h"
#include <math.h>

bool IsCollisionSphere(const Sphere& s1, const Sphere& s2) {

	//中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//衝突判定
	if (distance <= s1.radius + s2.radius) {
		return true;
	}
	
	return false;

}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	//平面と球の中心点の距離を求める
	float distance = fabsf(Dot(plane.normal, sphere.center) - plane.distance);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}

