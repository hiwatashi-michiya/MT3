#define _USE_MATH_DEFINES
#include <math.h>
#include <Novice.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <stdint.h>
#include "Draw.h"
#include <imgui.h>
#include "Collision.h"

const char kWindowTitle[] = "LE2A_15_ヒワタシミチヤ";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	/*const int kRowHeight = 20;*/
	/*const int kColumnWidth = 60;*/

	Vector3 rotate{ 0.0f, 0.0f, 0.0f };
	Vector3 translate{0.0f,0.0f,0.0f};
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };

	Vector3 rotateOBB{ 0.0f,0.0f,0.0f };
	OBB obb{
		.center{-1.0f,0.0f,0.0f},
		.orientations =
		{{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}},
		.size{0.5f,0.5f,0.5f}
	};
	Sphere sphere{
		.center{0.0f,0.0f,0.0f},
		.radius{0.5f}
	};

	int color = WHITE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
		ImGui::Begin("Window");
		ImGui::DragFloat3("OBB rotate", &rotateOBB.x, 0.01f);
		ImGui::DragFloat3("OBB center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("OBB orientation X", &obb.orientations[0].x, 0.01f);
		ImGui::DragFloat3("OBB orientation Y", &obb.orientations[1].x, 0.01f);
		ImGui::DragFloat3("OBB orientation Z", &obb.orientations[2].x, 0.01f);
		ImGui::DragFloat3("OBB size", &obb.size.x, 0.01f, 0.1f, 100.0f);
		ImGui::DragFloat3("Sphere center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::End();

		if (IsCollision(obb, sphere)) {
			color = 0xFF0000FF;
		}
		else {
			color = 0xFFFFFFFF;
		}

		//回転行列を生成
		Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotateOBB.x),
			Multiply(MakeRotateYMatrix(rotateOBB.y), MakeRotateZMatrix(rotateOBB.z)));

		//回転行列から軸を抽出
		obb.orientations[0].x = rotateMatrix.m[0][0];
		obb.orientations[0].y = rotateMatrix.m[0][1];
		obb.orientations[0].z = rotateMatrix.m[0][2];

		obb.orientations[1].x = rotateMatrix.m[1][0];
		obb.orientations[1].y = rotateMatrix.m[1][1];
		obb.orientations[1].z = rotateMatrix.m[1][2];

		obb.orientations[2].x = rotateMatrix.m[2][0];
		obb.orientations[2].y = rotateMatrix.m[2][1];
		obb.orientations[2].z = rotateMatrix.m[2][2];

		obb.orientations[0] = Normalize(obb.orientations[0]);
		obb.orientations[1] = Normalize(obb.orientations[1]);
		obb.orientations[2] = Normalize(obb.orientations[2]);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawOBB(obb, viewProjectionMatrix, viewportMatrix, color);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
