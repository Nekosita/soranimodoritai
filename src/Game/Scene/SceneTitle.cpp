// ==================================================
// SceneTitle.cpp
//	タイトル画面用のファイル
// ==================================================
#include"../Game.h"
#include"../Main.h"
#include"../src/System/KeyInput.h"
#include"SceneTitle.h"

namespace
{
	int seclect_num;//!<選択用の変数
	int num_X;		//!<選択肢の初期座標X
	int num_Y;		//!<選択肢の初期座標Y
}
//初期化処理
//--------------------------------
void InitGameTitle()
{
	//ゲームスタート画面の選択用
	seclect_num = 0;
	num_X = 280;
	num_Y = 600;
}
//戻り値: 選択されたシーンの番号
//------------------------------------------
GAME_SCREEN UpdateGameTitle()
{
	if (IsKeyOn(KEY_INPUT_DOWN)) {
		seclect_num++;
		if (seclect_num > 2)seclect_num = 0;
	}
	if (IsKeyOn(KEY_INPUT_UP)) {
		seclect_num--;
		if (seclect_num < 0)seclect_num = 2;
	}
	if (IsKeyOn(KEY_INPUT_RETURN)) {
		if (CheckMusic() == 0) {
			PlaySoundFile("Data/music/魔王魂 効果音 システム49.mp3", DX_PLAYTYPE_BACK);
		}
		switch (seclect_num)
		{
		case 0:
			return GAME_SCREEN::GAME_RULE;
		case 1:
			return GAME_SCREEN::GAME_RANK;
		case 2:
			return GAME_SCREEN::GAME_NOWLOADING;
		}
	}
	return GAME_SCREEN::GAME_TITLE;
}
//描画処理
//---------------------------------------------
void RenderGameTitle()
{
	DrawCircle(225, 625 + seclect_num * 50, 5, GetColor(50, 160, 130), TRUE);
	SetFontSize(80);
	DrawString(330, 70, "空に戻りたい！", GetColor(50, 160, 130), TRUE);
	SetFontSize(30);
	DrawString(225, 770, "(Enterキーで決定)", GetColor(255, 255, 255), TRUE);
	SetFontSize(60);

	if (seclect_num == 0) {
		DrawString(num_X, num_Y, "RULE", GetColor(255, 255, 255), TRUE);
	}
	else {
		DrawString(num_X-30, num_Y, "RULE", GetColor(50, 160, 130), TRUE);
	}
	if (seclect_num == 1) {
		DrawString(num_X, num_Y+  50, "RANK", GetColor(255, 255, 255), TRUE);
	}
	else {
		DrawString(num_X-30, num_Y+ 50, "RANK", GetColor(50, 160, 130), TRUE);
	}
	if (seclect_num == 2) {
		DrawString(num_X, num_Y+ 100, "STAGE", GetColor(255, 255, 255), TRUE);
	}
	else {
		DrawString(num_X-30, num_Y+ 100, "STAGE", GetColor(50, 160, 130), TRUE);
	}
}
