#define _USE_MATH_DEFINES
#include <math.h>
#include <Novice.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <stdint.h>

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

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);
	
	Vector3 rotate{ 0.0f, 0.0f, 0.0f };
	Vector3 translate{0.0f,0.0f,1.0f};
	Vector3 cameraPosition(0.0f, 0.0f, -200.0f);
	const Vector3 kLocalVertices[3] = {
		Vector3(-25.0f,-25.0f,0.0f),
		Vector3(0.0f,25.0f,0.0f),
		Vector3(25.0f,-25.0f,0.0f)
	};

	Vector3 vA = Subtract(kLocalVertices[2], kLocalVertices[1]);
	Vector3 vB = Subtract(kLocalVertices[0], kLocalVertices[2]);
	Vector3 vC = Subtract(translate, cameraPosition);

	Vector3 crossAB = {};

	float dotCAB = 0.0f;

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

		//キー入力による移動と回転の処理

		//X軸の移動
		if (keys[DIK_A]) {
			translate = Add(translate, Vector3(-1.0f, 0.0f, 0.0f));
		}
		else if (keys[DIK_D]) {
			translate = Add(translate, Vector3(1.0f, 0.0f, 0.0f));
		}

		//Z軸の移動
		if (keys[DIK_W]) {
			translate = Add(translate, Vector3(0.0f, 0.0f, 1.0f));
		}
		else if (keys[DIK_S]) {

			//移動制限
			if (translate.z > -90.0f) {
				translate = Add(translate, Vector3(0.0f, 0.0f, -1.0f));
			}
			
		}

		//回転処理
		rotate = Add(rotate, Vector3(0.0f, float(M_PI / 60.0f), 0.0f));

		if (rotate.y >= M_PI * 2.0f) {
			rotate = Vector3(0.0f, 0.0f, 0.0f);
		}

		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		//各種ベクトル更新
		vA = Subtract(screenVertices[1], screenVertices[2]);
		vB = Subtract(screenVertices[2], screenVertices[0]);
		vC = Subtract(cameraPosition, translate);
		crossAB = Cross(vA, vB);
		dotCAB = Dot(vC, crossAB);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		if (dotCAB <= 0.0f) {
			Novice::DrawTriangle(
				int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
				int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid
			);
		}
		
		VectorScreenPrintf(0, 0, cross, "Cross");
		
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
