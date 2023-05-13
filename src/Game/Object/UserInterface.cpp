#include"UserInterface.h"
#include"../src/Game/Main.h"
#include"../src/Game/Object/Model/Player.h"
#include"../src/Game/Object/Model/Item.h"
#include"DxLib.h"
#include"../src/System/Vector3.h"
#include"../src/System/AppendFunction.h"



void UserInterface::SetItemComplete(bool now_item_complete)
{
	item_complete = now_item_complete;
}
bool UserInterface::GetItemComplete() const
{
	return item_complete;
}
void UserInterface::SetItemCompleteCounter(int now_count)
{
	item_complete_counter = now_count;
}
void UserInterface::ReduceItemCompleteCounter()
{
	item_complete_counter--;
}
int UserInterface::GetItemCompleteCounter() const
{
	return item_complete_counter;
}
void UserInterface::SetItemNum(int now_num)
{
	item_num = now_num;
}
int UserInterface::GetItemNum()
{
	return item_num;
}
void UserInterface::SetHint(bool now_hint)
{
	hint = now_hint;
}
bool UserInterface::GetHint() const
{
	return hint;
}
void UserInterface::SetNum(int now_num)
{
	now_num = num;
}
void UserInterface::AddNum()
{
	num++;
}
int UserInterface::Getnum() const
{
	return num;
}
void UserInterface::SetTimeCount(int now_count)
{
	time_count= now_count;
}
int UserInterface::GetTimeCount() const
{
	return time_count;
}
void UserInterface::MinueTimeCount()
{
	time_count--;
}
void UserInterface::Init()
{
	SetTimeCount(600);//スコア計算用
	SetNum(0);//アイテムを拾った数
	time_count_flame = 0;
	mini_map = LoadGraph("Data/mini_map.png");
	player_on_map = LoadGraph("Data/player_on_map.png");
	mini_pos.x = (SCREEN_W - 255.0f);
	mini_pos.y = (5.0f);

	//アイテム全て揃った時用
	item_complete_counter = 180;
	item_complete = false;
	hint = false;
	mute_handle = LoadGraph("Data/mute.png");
	for (int i = 0; i < 10; i++)item_ptr_mini[i] = nullptr;
}

//残り時間をの計算関数
//----------------------------------------------
void UserInterface::TimeCountFlame() {
	//タイムカウンター
	time_count_flame++;
	if (time_count_flame == 60) {
		time_count_flame = 0;
		MinueTimeCount();
	}
}
//HPとカウンター描画
// ----------------------------------------------
void UserInterface::DrawUi() {
	//HP描画
	SetFontSize(40);
	DrawString(10, 10, "HP:", GetColor(63, 72, 204));
	DrawFillBox(110, 10, 500 + 110, 50, GetColor(63, 72, 204));
	DrawFillBox(110, 10, static_cast<int>(player_ptr->GetLifeX()) + 110, 50, GetColor(50, 160, 130));
	DrawLineBox(110, 10, 500 + 110, 50, GetColor(63, 72, 204));
	SetFontSize(20);
	//拾うアイテムの確認
	SetFontSize(40);
	char Item[256];
	sprintf_s(Item, "アイテム数:%02d", num);
	DrawString(10, 80, Item, GetColor(63, 72, 204));
	SetFontSize(40);
	//Game_count を出す
	SetFontSize(40);
	char time[256];
	sprintf_s(time, "残り時間:%03d秒", time_count);
	DrawString(0, 150, time, GetColor(63, 72, 204));
	SetFontSize(40);
}
void UserInterface::DrawMiniMap()
{
	//小さいマップ描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	player_lm_pos.x = mini_pos.x + PosInMapX(player_ptr->GetPos());
	player_lm_pos.y = mini_pos.y + PosInMapY(player_ptr->GetPos());
	DrawGraph((int)mini_pos.x, (int)mini_pos.y, mini_map, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(18);//文字サイズ設定
	DrawCircle((int)(SCREEN_W - 60), (int)30, (int)3.0f, GetColor(0, 255, 0), 1, TRUE);
	DrawString((int)(SCREEN_W - 110), (int)25, "近い", GetColor(0, 255, 0));
	DrawCircle((int)(SCREEN_W - 60), (int)60, (int)3.0f, GetColor(255, 255, 255), 1, TRUE);
	DrawString((int)(SCREEN_W - 110), (int)55, "遠い", GetColor(255, 255, 255));
	SetFontSize(40);//文字サイズ設定

	//小さいマップのプレイヤーの位置描画
	DrawRotaGraph((int)player_lm_pos.x, (int)player_lm_pos.y, 0.15f, player_ptr->GetPlayerModelRot().y, player_on_map, TRUE);

	//小さいマップのアイテムの位置描画
	for (int i = 0; i < GetItemNum(); i++) {
		//拾われない時のみ描画
		if (item_ptr_mini[i]->GetIsPicked())continue;
		//この座標にアイテムを描画
		float nx = mini_pos.x + PosInMapX(item_ptr_mini[i]->GetPos());
		float ny = mini_pos.y + PosInMapY(item_ptr_mini[i]->GetPos());
		//プレイヤーに近いなら緑　他全部黒
		if (GetDistance(player_ptr->GetPos(), item_ptr_mini[i]->GetPos()) < 3000.0f) {
			DrawCircle((int)nx, (int)ny, (int)3.0f, GetColor(0, 255, 0), 1, TRUE);
		}else{
			DrawCircle((int)nx, (int)ny, (int)3.0f, GetColor(255, 255, 255), 1, TRUE);
		}
	}
}
void UserInterface::DrawHintBar() {
	SetFontSize(40);
	if ((item_complete_counter < 200) && (item_complete)) {
		DrawString((SCREEN_W / 2) - 650, (SCREEN_H / 2), "アイテム全て集まりました、最上階を目指しましょう！", GetColor(255, 255, 0), TRUE);
	}
	if (player_ptr->GetToHintBar()) {
		DrawString((SCREEN_W / 2) - 350, (SCREEN_H / 2), "10個のアイテムはまだ集め終わっていません", GetColor(255, 255, 0), TRUE);
	}
	SetFontSize(40);
}
//GAME_PAUSEのテキスト描画
// ----------------------------------------------
void UserInterface::DrawGamepauseText(bool music_on,int sound) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);//半透明設定
	DrawBox((SCREEN_W / 2) - 100, 100, SCREEN_W - 100, SCREEN_H - 100, GetColor(255, 255, 255), 1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//半透明の設定を戻っに戻す
	DrawString((SCREEN_W / 2), 140, "音量:", GetColor(0, 0, 0));
	//音量確認
	if (music_on) {
		char text_volume[256];
		sprintf_s(text_volume, "%3d", sound);
		DrawString((SCREEN_W / 2) + 100, 140, text_volume, GetColor(0, 0, 0));
		float box_x = 200;
		DrawFillBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 510, 180, GetColor(0, 0, 0));
		DrawFillBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 255 + (int)(sound), 180, GetColor(100, 100, 100));
		DrawLineBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 510, 180, GetColor(160, 160, 160));
	}
	if (!music_on) {
		DrawRotaGraph((SCREEN_W / 2) + 300, 155, 0.2f, 0, mute_handle, TRUE, FALSE);
	}
	SetFontSize(40);
	SetFontSize(30);
	DrawString((SCREEN_W / 2), int(210), "右キー・左キーでBGMの音量調整", GetColor(0, 0, 0), TRUE);
	DrawString((SCREEN_W / 2), int(240), "１キーでONとOFF切り替え", GetColor(0, 0, 0), TRUE);
	SetFontSize(40);
	//HINT_BAR描画
	SetFontSize(50);
	DrawString((SCREEN_W / 2), SCREEN_H / 2, "HINT BAR :", GetColor(0, 0, 0), TRUE);
	SetFontSize(30);
	DrawString((SCREEN_W / 2), SCREEN_H / 2 + 70, "周りを探索し、特別な色のゾーンの効果を理解し", GetColor(0, 0, 0), TRUE);
	DrawString((SCREEN_W / 2), SCREEN_H / 2 + 120, "アイテムを集めながら最上階を目指しましょう", GetColor(0, 0, 0), TRUE);
	SetFontSize(30);
	SetFontSize(40);
}
//全てのアイテムを集めた後HINTバーを出す用の関数
//----------------------------------------------
void UserInterface::ItemCompleteHint() {
	if (!GetItemComplete()) {
		if (Getnum() == GetItemNum()) {
			SetItemComplete(true);
		}
	}
	//演出入ります
	if (GetItemCompleteCounter() < 200) {
		if (GetItemComplete()) {
			ReduceItemCompleteCounter();
			if (GetItemCompleteCounter() < 0) {
				//1秒の後自動的に消します
				SetItemCompleteCounter(300);
				SetItemComplete(true);
			}
		}
	}
}
//GAME_SCREEN::GAME_OVERで使う変数・関数
// --------------------------------------------------
void UserInterface::DrawGameoverText() {
	SetFontSize(60);
	DrawString(int(SCREEN_W / 2 - 150.0f), int(SCREEN_H / 2 - 350.0f), "GAME OVER", GetColor(237, 28, 36), TRUE);
	DrawString(int(SCREEN_W / 2 - 225.0f), int(SCREEN_H - 250.0f), "ENTER TO TITLE", GetColor(237, 28, 36), TRUE);
	SetFontSize(60);
}