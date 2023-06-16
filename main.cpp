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

	Segment segment{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f} };
	Plane plane{ {0.0f,1.0f,0.0f},1.0f };
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
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(segment, plane)) {
			color = 0xFF0000FF;
		}
		else {
			color = 0xFFFFFFFF;
		}
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");
		ImGui::DragFloat3("Plane Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane Distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::End();

		plane.normal = Normalize(plane.normal);

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawTransformLine(segment.origin, segment.diff, worldViewProjectionMatrix, viewportMatrix, color);
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

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
