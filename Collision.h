#pragma once

struct Sphere;

struct Plane;

bool IsCollisionSphere(const Sphere& s1, const Sphere& s2);

bool IsCollision(const Sphere& sphere, const Plane& plane);
