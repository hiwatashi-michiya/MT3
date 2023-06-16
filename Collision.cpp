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

bool IsCollision(const Segment& segment, const Plane& plane) {

	//垂直判定を行うために法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	//垂直なら当たっていない
	if (dot == 0.0f) {
		return false;
	}

	//媒介変数tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	//tが定義域内なら衝突している
	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;

}
