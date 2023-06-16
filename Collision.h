#pragma once

struct Sphere;

struct Plane;

//球と球の当たり判定
bool IsCollisionSphere(const Sphere& s1, const Sphere& s2);

//球と平面の当たり判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

//線分と平面の当たり判定
bool IsCollision(const Segment& segment, const Plane& plane);

//線分と三角形の当たり判定
bool IsCollision(const Segment& segment, const Triangle& triangle);
