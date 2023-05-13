#include"../Game.h"
#include"../Main.h"
#include"SceneRule.h"
#include"../src/System/KeyInput.h"
//#include "SceneStartAnime.h"


namespace
{
	int game_rule_handle;//!<選択用の変数
}

void InitGameRule()
{
	game_rule_handle = LoadGraph("Data/back.jpg");
}

GAME_SCREEN UpdateGameRule()
{
	if (IsKeyOn(KEY_INPUT_RETURN)) {
		if (CheckMusic() == 0) {
			PlaySoundFile("Data/music/魔王魂 効果音 システム49.mp3", DX_PLAYTYPE_BACK);
		}
		return GAME_SCREEN::GAME_TITLE;
	}
	return GAME_SCREEN::GAME_RULE;
}

void RenderGameRule()
{
	DrawRotaGraph(SCREEN_W / 2, SCREEN_H / 2, 1.0f, 0, game_rule_handle, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);//半透明設定
	DrawBox(50, 30, SCREEN_W - 50, SCREEN_H - 30, GetColor(255, 255, 255), 1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//半透明の設定を戻っに戻す

	SetFontSize(60);//文字サイズ設定
	DrawString(60, 50, "STORY:", GetColor(10, 40, 200), TRUE);
	SetFontSize(40);
	DrawString(60, 140, "あなたは今空から箱庭に落ちています", GetColor(50, 50, 50), TRUE);
	DrawString(60, 210, "箱庭の中には色々ギミックがあります", GetColor(50, 50, 50), TRUE);
	DrawString(60, 280, "空に戻る為虹色のアイテムを10個を集め黄色ブロックに行って", GetColor(50, 50, 50), TRUE);
	DrawString(60, 360, "虹色のアイテムを捧げなさい", GetColor(50, 50, 50), TRUE);
	SetFontSize(60);//文字サイズ設定
	DrawString(60, 420, "操作説明：", GetColor(10, 40, 200), TRUE);
	SetFontSize(40);
	DrawString(60, 500, "上下左右：移動", GetColor(50, 50, 50), TRUE);
	DrawString(60, 550, "AD：カメラ左右移動", GetColor(50, 50, 50), TRUE);
	DrawString(500, 550, "WS：カメラ前後移動", GetColor(50, 50, 50), TRUE);
	DrawString(60, 600, "SPACE：JUMP", GetColor(50, 50, 50), TRUE);
	DrawString(60, 650, "ENTER:BGM調整ができ、HINTBARも見れます", GetColor(50, 50, 50), TRUE);
	DrawString(1200, 800, "ENTER TO TITLE", GetColor(10, 40, 200), TRUE);
	SetFontSize(40);
}