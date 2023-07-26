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

	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};

	Vector3 rotates[3] = {
		{0.0f,0.0f,-6.0f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};

	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};

	float radius = 0.05f;

	Sphere spheres[3] = {
		{.center{translates[0]},.radius{radius}},
		{.center{translates[1]},.radius{radius}},
		{.center{translates[2]},.radius{radius}},
	};

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

		ImGui::Begin("arm");
		ImGui::DragFloat3("translate[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotate[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scale[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translate[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotate[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scale[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translate[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotate[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scale[2]", &scales[2].x, 0.01f);
		ImGui::End();

		Matrix4x4 matShoulder = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		Matrix4x4 matElbow = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		matElbow = Multiply(matElbow, matShoulder);
		Matrix4x4 matHand = MakeAffineMatrix(scales[2], rotates[2], translates[2]);
		matHand = Multiply(matHand, matElbow);

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
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::End();

		Vector3 shoulderTranslate = { matShoulder.m[3][0], matShoulder.m[3][1], matShoulder.m[3][2] };
		Vector3 elbowTranslate = { matElbow.m[3][0], matElbow.m[3][1], matElbow.m[3][2] };
		Vector3 handTranslate = { matHand.m[3][0], matHand.m[3][1], matHand.m[3][2] };
		
		spheres[0].center = shoulderTranslate;
		spheres[1].center = elbowTranslate;
		spheres[2].center = handTranslate;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSphere(spheres[0], viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(spheres[1], viewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(spheres[2], viewProjectionMatrix, viewportMatrix, BLUE);
		DrawChangeLine(spheres[0].center, spheres[1].center, viewProjectionMatrix, viewportMatrix, WHITE);
		DrawChangeLine(spheres[1].center, spheres[2].center, viewProjectionMatrix, viewportMatrix, WHITE);

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
