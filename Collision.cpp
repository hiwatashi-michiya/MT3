#include "Draw.h"
#include <math.h>
#include <algorithm>
#include <cassert>

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

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}

	return false;

}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {

	//最接近点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};

	//最接近点と球の中心との座標を求める
	float distance = Length(Subtract(closestPoint, sphere.center));

	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}


	return false;

}

bool IsCollision(const AABB& aabb, const Segment& segment) {


	//それぞれの媒介変数を求める
	float tMinX = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tMaxX = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tMinY = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tMaxY = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tMinZ = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tMaxZ = (aabb.max.z - segment.origin.z) / segment.diff.z;

	//衝突点の近い方と遠い方を求める
	float tNearX = min(tMinX, tMaxX);
	float tFarX = max(tMinX, tMaxX);
	float tNearY = min(tMinY, tMaxY);
	float tFarY = max(tMinY, tMaxY);
	float tNearZ = min(tMinZ, tMaxZ);
	float tFarZ = max(tMinZ, tMaxZ);

	//AABBとの衝突点(貫通点)のtが小さい方
	float tMin = max(max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点(貫通点)のtが大きい方
	float tMax = min(min(tFarX, tFarY), tFarZ);

	//衝突しているかどうか判定
	if (tMin <= tMax && 
		((tMin >= 0.0f && tMin <= 1.0f) ||
		(tMax >= 0.0f && tMax <= 1.0f))) {
		return true;
	}

	return false;

}

bool IsCollision(const OBB& obb, const Sphere& sphere) {

	//衝突判定のための変数を設定
	Matrix4x4 obbWorldMatrix{
		{
		{obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z,0,},
		{obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z,0,},
		{obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z,0,},
		{obb.center.x, obb.center.y,obb.center.z, 1}
		}
	};

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);

	AABB aabbOBBLocal{ Vector3(-obb.size.x, -obb.size.y, -obb.size.z), obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };
	//ローカル空間で衝突判定
	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}
	
	return false;

}
