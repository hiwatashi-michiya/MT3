#include <Vector3.h>
#include <math.h>
#include <Novice.h>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 v;

	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;

	return v;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v1) {

	Vector3 v;

	v.x = scalar * v1.x;
	v.y = scalar * v1.y;
	v.z = scalar * v1.z;

	return v;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {

	float x;

	x = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return x;
}

// 長さ(ノルム)
float Length(const Vector3& v) {

	float length;

	length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return length;
}

// 正規化
Vector3 Normalize(const Vector3& v) {

	float length = Length(v);

	Vector3 newV;

	newV.x = v.x;
	newV.y = v.y;
	newV.z = v.z;

	if (length != 0.0f) {

		newV.x = v.x / length;
		newV.y = v.y / length;
		newV.z = v.z / length;
	}

	return newV;
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	const int kColumnWidth = 60;

	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
