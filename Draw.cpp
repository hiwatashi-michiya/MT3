#define _USE_MATH_DEFINES
#include "Draw.h"
#include <Novice.h>
#include <math.h>

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 10; //分割数
	const float  kLonEvery = float(2 * M_PI / kSubdivision); //経度分割1つ分の角度
	const float kLatEvery = float(2 * M_PI / kSubdivision); //緯度分割1つ分の角度
	//緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(float(M_PI) / 2.0f) + kLatEvery * latIndex; //現在の緯度
		//経度の方向に分割　-π/2 ～ π/2
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の経度
			float thetaD = float(M_PI / kSubdivision);
			float phiD = float(2 * M_PI / kSubdivision);
			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = { sphere.radius * cosf(lat) * cosf(lon) + sphere.center.x,
				sphere.radius * sinf(lat) + sphere.center.y,
				sphere.radius * cosf(lat) * sinf(lon) + sphere.center.z };

			b = { sphere.radius * cosf(lat + thetaD) * cosf(lon) + sphere.center.x,
				sphere.radius * sinf(lat + thetaD) + sphere.center.y,
			sphere.radius * cosf(lat + thetaD) * sinf(lon) + sphere.center.z };

			c = { sphere.radius * cosf(lat) * cosf(lon + phiD) + sphere.center.x,
			sphere.radius * sinf(lat) + sphere.center.y,
			sphere.radius * cosf(lat) * sinf(lon + phiD) + sphere.center.z };

			//スクリーン座標に変換
			Vector3 ndcVertex = Transform(a, viewProjectionMatrix);
			a = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(b, viewProjectionMatrix);
			b = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(c, viewProjectionMatrix);
			c = Transform(ndcVertex, viewportMatrix);

			//ab,acで線を引く
			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);

		}
	}

}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridHalfWidth = 2.0f; //Gridの半分の幅
	const uint32_t kSubdivision = 10; //分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); //1つ分の長さ

	//奥から手前への線を順々に引いていく
	for (uint32_t x = 0; x <= kSubdivision; ++x) {

		//座標を求める
		Vector3 v1 = { kGridEvery * x - kGridHalfWidth, 0.0f,kGridHalfWidth };
		Vector3 v2 = { kGridEvery * x - kGridHalfWidth,0.0f,-kGridHalfWidth };

		//スクリーン座標に変換
		Vector3 ndcVertex = Transform(v1, viewProjectionMatrix);
		v1 = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(v2, viewProjectionMatrix);
		v2 = Transform(ndcVertex, viewportMatrix);

		//変換後の座標を使って表示
		Novice::DrawLine(int(v1.x), int(v1.y), int(v2.x), int(v2.y), 0xAAAAAAFF);

	}

	//左右
	for (uint32_t z = 0; z <= kSubdivision; ++z) {

		//座標を求める
		Vector3 v1 = { kGridHalfWidth, 0.0f,kGridEvery * z - kGridHalfWidth };
		Vector3 v2 = { -kGridHalfWidth,0.0f,kGridEvery * z - kGridHalfWidth };

		//スクリーン座標に変換
		Vector3 ndcVertex = Transform(v1, viewProjectionMatrix);
		v1 = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(v2, viewProjectionMatrix);
		v2 = Transform(ndcVertex, viewportMatrix);

		//変換後の座標を使って表示
		Novice::DrawLine(int(v1.x), int(v1.y), int(v2.x), int(v2.y), 0xAAAAAAFF);

	}

}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	//中心点を決める
	Vector3 center = Multiply(plane.distance, plane.normal);
	//描画に使う四つのベクトルを定義
	Vector3 perpendiculars[4];
	//法線と垂直なベクトルを一つ求める
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	//上で求めた垂直ベクトルの逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	//垂直ベクトルと法線のクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	//上で求めたクロス積の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };
	//上記の四つのベクトルを中心点にそれぞれ定数倍して足すと四頂点が出来上がる
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	//pointsを結んで描画
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);

}

void DrawTransformLine(const Vector3& p1, const Vector3& p2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 start = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);

	Vector3 end = Transform(Transform(Add(p1, p2), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);

}
