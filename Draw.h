#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <stdint.h>

struct Sphere
{
	Vector3 center;
	float radius;
};

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
