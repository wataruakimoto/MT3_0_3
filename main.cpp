#include <Novice.h>
#include <assert.h>

struct Vector3 {
	float X;
	float Y;
	float Z;
};

struct Matrix4x4 {
	float m[4][4];
};

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 resultTranslate = {};

	resultTranslate.m[0][0] = 1.0f;
	resultTranslate.m[0][1] = 0.0f;
	resultTranslate.m[0][2] = 0.0f;
	resultTranslate.m[0][3] = 0.0f;

	resultTranslate.m[1][0] = 0.0f;
	resultTranslate.m[1][1] = 1.0f;
	resultTranslate.m[1][2] = 0.0f;
	resultTranslate.m[1][3] = 0.0f;

	resultTranslate.m[2][0] = 0.0f;
	resultTranslate.m[2][1] = 0.0f;
	resultTranslate.m[2][2] = 1.0f;
	resultTranslate.m[2][3] = 0.0f;

	resultTranslate.m[3][0] = translate.X;
	resultTranslate.m[3][1] = translate.Y;
	resultTranslate.m[3][2] = translate.Z;
	resultTranslate.m[3][3] = 1.0f;

	return resultTranslate;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 resultScale = {};

	resultScale.m[0][0] = scale.X;
	resultScale.m[0][1] = 0.0f;
	resultScale.m[0][2] = 0.0f;
	resultScale.m[0][3] = 0.0f;

	resultScale.m[1][0] = 0.0f;
	resultScale.m[1][1] = scale.Y;
	resultScale.m[1][2] = 0.0f;
	resultScale.m[1][3] = 0.0f;

	resultScale.m[2][0] = 0.0f;
	resultScale.m[2][1] = 0.0f;
	resultScale.m[2][2] = scale.Z;
	resultScale.m[2][3] = 0.0f;

	resultScale.m[3][0] = 0.0f;
	resultScale.m[3][1] = 0.0f;
	resultScale.m[3][2] = 0.0f;
	resultScale.m[3][3] = 1.0f;

	return resultScale;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	// w = 1 がデカルト座標系であるので(x,y,z,1)のベクトルとしてmatrixとの積をとる
	Vector3 resultTransform = {};

	resultTransform.X = vector.X * matrix.m[0][0] + vector.Y * matrix.m[1][0] + vector.Z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	resultTransform.Y = vector.X * matrix.m[0][1] + vector.Y * matrix.m[1][1] + vector.Z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	resultTransform.Z = vector.X * matrix.m[0][2] + vector.Y * matrix.m[1][2] + vector.Z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.X * matrix.m[0][3] + vector.Y * matrix.m[1][3] + vector.Z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	// ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
	assert(w != 0.0f);

	// w = 1 がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	resultTransform.X /= w;
	resultTransform.Y /= w;
	resultTransform.Z /= w;

	return resultTransform;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 3次元ベクトルの数値表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%.02f", vector.X);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.Y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.Z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// 4x4行列の数値表示

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

const char kWindowTitle[] = "LE2B_01_アキモト_ワタル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 translate = { 4.1f,2.6f,0.8f };

	Vector3 scale = { 1.5f,5.2f,7.3f };

	Vector3 point = { 2.3f,3.8f,1.4f };

	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f,
	};

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		Vector3 transformed = Transform(point, transformMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, transformed, "transformed");
		MatrixScreenPrintf(0, kRowHeight * 1, translateMatrix);
		MatrixScreenPrintf(0, kRowHeight * 6, scaleMatrix);

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
