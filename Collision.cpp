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

bool IsCollision(const Segment& segment, const Triangle& triangle) {

	//各辺を結ぶベクトル
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v3 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	//法線nを求める
	Vector3 normal = Cross(v1, v2);

	normal = Normalize(normal);

	//距離を求める
	float distance = Dot(triangle.vertices[0], normal);

	//垂直判定を行うために法線と線の内積を求める
	float dot = Dot(normal, segment.diff);

	//垂直なら当たっていない
	if (dot == 0.0f) {
		return false;
	}

	//媒介変数tを求める
	float t = (distance - Dot(segment.origin, normal)) / dot;

	//tが定義域外なら当たっていない
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	//衝突点の計算
	Vector3 point = Add(segment.origin, Multiply(t, segment.diff));

	//衝突点と3頂点の差分ベクトル
	Vector3 vp1 = Subtract(point, triangle.vertices[1]);
	Vector3 vp2 = Subtract(point, triangle.vertices[2]);
	Vector3 vp3 = Subtract(point, triangle.vertices[0]);

	//クロス積
	Vector3 cross10 = Cross(v1, vp1);
	Vector3 cross21 = Cross(v2, vp2);
	Vector3 cross02 = Cross(v3, vp3);

	//全ての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross10, normal) >= 0.0f &&
		Dot(cross21, normal) >= 0.0f &&
		Dot(cross02, normal) >= 0.0f) {
		return true;
	}

	return false;

}
