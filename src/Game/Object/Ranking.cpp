#include"Ranking.h"
#include"../src/Game/Game.h"
#include"../Object/Model/Player.h"
#include<fstream>
#include<sstream>
#include<string>



void Ranking::SetScore(int now_score)
{
	score = now_score;
}

int Ranking::GetScore() const
{
	return score;
}

void Ranking::InitGameClearAnime() {
	clear_counter = 0;//クリア時の演出時間
}
void Ranking::GameRankInit() {
	game_rank_handle = LoadGraph("Data/back.jpg");
}
//ランキングファイル処理
//-----------------------------------------------
void Ranking::Sort(int* now_tmp_rank_RL) {
	int min_idx;
	int tmp_score;
	//すべてのデータを並べ替え
	for (int i = 0; i < 6; ++i) {
		int min_score = now_tmp_rank_RL[i];	 //スコア並び替え
		min_idx = i;
		for (int j = i; j < 6; ++j) {
			if (min_score > now_tmp_rank_RL[j]) {
				min_score = now_tmp_rank_RL[j];
				min_idx = j;
			}
		}
		//並べ替え
		tmp_score = now_tmp_rank_RL[i];
		now_tmp_rank_RL[i] = now_tmp_rank_RL[min_idx];
		now_tmp_rank_RL[min_idx] = tmp_score;
	}
}
//ランキング記録用の関数
//-----------------------------------------------
void Ranking::NormalRankRecode(std::string& Filename) {
	int tmp_rank_RL[6];		 //スコア記録
	for (int i = 0; i < 5; i++) {
		tmp_rank_RL[i] = rank_RL[i];
	}
	tmp_rank_RL[5] = score;
	//並び替え
	Sort(tmp_rank_RL);
	//書き込み
	rank_file.open(Filename, std::ios::out);
	if (rank_file.is_open()) {
		for (int x = 0; x < 5; x++) {
			rank_file << tmp_rank_RL[x] << std::endl;
		}
		rank_file.close();
	}
}

void Ranking::ParfectRankRecode(std::string& Filename) {
	int tmp_rank_RL_P[6];		 //スコア記録
	for (int i = 0; i < 5; i++) {
		tmp_rank_RL_P[i] = rank_RL_p[i];
	}
	tmp_rank_RL_P[5] = score;
	//並び替え
	Sort(tmp_rank_RL_P);
	//書き込み
	rank_file.open(Filename, std::ios::out);
	if (rank_file.is_open()) {
		for (int x = 0; x < 5; x++) {
			rank_file << tmp_rank_RL_P[x] << std::endl;
		}
		rank_file.close();
	}
}

void Ranking::RankProcess(GAME_SCREEN& now_screen)
{
		clear_counter++;
	if (clear_counter > 120) {
		//もしもプレイヤーのHPが欠損ないならパーフェクトクリアに入ります	
		if (player_ptr->GetHp() == 500.0f) {
			std::string PerfectrankFile = "Data/text/parfect_rank.txt";//ファイルを入れてランキングを並び替えます
			NormalRankRecode(PerfectrankFile);
			ReadParfectRankingFile();	//念のためもう一回ファイル読み込む
			now_screen = GAME_SCREEN::GAME_PARFECT_CLEAR;
		}
		else {
			std::string Normalrankfile = "Data/text/rank.txt";//ファイルを入れてランキングを並び替えます
			ParfectRankRecode(Normalrankfile);
			ReadNormalRankingFile();	//念のためもう一回ファイル読み込む
			now_screen = GAME_SCREEN::GAME_CLEAR;
		}
		return;
	}
}

void Ranking::DrawPetfectClearText() {
	SetFontSize(50);
	char clear_time_P[256];
	sprintf_s(clear_time_P, "使用時間:%03d秒", score);
	DrawString(200, 450, clear_time_P, GetColor(50, 50, 50));
	DrawString(int(SCREEN_W / 2 - 150.0f), int(SCREEN_H / 2 - 350.0f), "GAME PARFECT CLEAR", GetColor(237, 28, 36), TRUE);
	DrawString(int(150.0f), int(SCREEN_H - 250.0f), "ENTER TO TITLE", GetColor(237, 28, 36), TRUE);
	DrawString(int(SCREEN_W / 2 + 225.0f), int(SCREEN_H - 250.0f), "R TO RANK", GetColor(237, 28, 36), TRUE);
	SetFontSize(50);
}

void Ranking::DrawGameClearText() {
	SetFontSize(50);
	char clear_time[256];
	sprintf_s(clear_time, "使用時間:%03d秒", score);
	DrawString(200, 450, clear_time, GetColor(50, 50, 50));
	DrawString(int(SCREEN_W / 2 - 150.0f), int(SCREEN_H / 2 - 350.0f), "GAME CLEAR", GetColor(237, 28, 36), TRUE);
	DrawString(int(150.0f), int(SCREEN_H - 250.0f), "ENTER TO TITLE", GetColor(237, 28, 36), TRUE);
	DrawString(int(SCREEN_W / 2 + 225.0f), int(SCREEN_H - 250.0f), "R TO RANK", GetColor(237, 28, 36), TRUE);
	SetFontSize(50);
}

void Ranking::DrawGameRankText() {
	DrawRotaGraph(SCREEN_W / 2, SCREEN_H / 2, 1.0f, 0, game_rank_handle, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);//半透明設定
	DrawBox(50, 30, SCREEN_W - 50, SCREEN_H - 30, GetColor(255, 255, 255), 1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//半透明の設定を戻っに戻す

	SetFontSize(40);
	DrawString(1200, int(SCREEN_H / 2 + 350.0f), "ENTER TO TITLE", GetColor(10, 40, 200), TRUE);
	SetFontSize(40);

	SetFontSize(60);
	DrawString((int)(SCREEN_W / 2 - 175.0f), 75, "GAME RANK", GetColor(10, 40, 200), TRUE);

	//普通クリア用
	DrawString((int)(SCREEN_W / 2 - 550.0f), 200, "NORMAL CLEAR", GetColor(10, 40, 200), TRUE);

	rank_file.open("Data/text/rank.txt", std::ios::in);

	SetFontSize(60);
	char time_str[256];//ゲームTIME描画
	for (int i = 0; i < 5; i++) {
		sprintf_s(time_str, "%2d位:%02d秒", i + 1, rank_RL[i]);
		if (rank_RL[i] > 600) {
			DrawString((int)(SCREEN_W / 2 - 475.0f), (int)(320 + 80 * i), "-----", GetColor(10, 40, 200));
		}
		else {
			DrawString((int)(SCREEN_W / 2 - 500.0f), (int)(320 + 80 * i), time_str, GetColor(10, 40, 200));
		}
	}

	//パーフェクトクリア用
	DrawString((int)(SCREEN_W / 2 + 100.0f), 200, "PARFECT CLEAR", GetColor(10, 40, 200), TRUE);

	rank_file_p.open("Data/text/parfect_rank.txt", std::ios::in);
	char time_str_P[256];//ゲームTIME描画
	for (int i = 0; i < 5; i++) {
		sprintf_s(time_str_P, "%2d位:%02d秒", i + 1, rank_RL_p[i]);
		if (rank_RL_p[i] > 600) {
			DrawString((int)(SCREEN_W / 2 + 125.0f), (int)(320 + 80 * i), "-----", GetColor(10, 40, 200));
		}
		else {
			DrawString((int)(SCREEN_W / 2 + 150.0f), (int)(320 + 80 * i), time_str_P, GetColor(10, 40, 200));
		}
	}

	SetFontSize(60);
	rank_file.close();
}
//ランキングのテキストデータ読み込みファイル処理関数
//-----------------------------------------------
void Ranking::ReadNormalRankingFile()
{
	//ランクファイルの読み込み
	rank_file.open("Data/text/rank.txt", std::ios::in);//rankfileの読み込み
	if (rank_file.is_open()) {
		std::string str;
		for (int i = 0; i < 5; i++) {
			getline(rank_file, str);
			std::stringstream sstr(str);
			sstr >> rank_RL[i];
		}
		rank_file.close();
	}
}
//パーフェクトランキングのテキストデータ読み込みファイル処理関数
//-----------------------------------------------
void Ranking::ReadParfectRankingFile() {
	//パーフェクトランクファイルの読み込み
	rank_file_p.open("Data/text/parfect_rank.txt", std::ios::in);//rankfileの読み込み

	if (rank_file_p.is_open()) {
		std::string str_P;
		for (int i = 0; i < 5; i++) {
			getline(rank_file_p, str_P);
			std::stringstream sstr(str_P);
			sstr >> rank_RL_p[i];
		}
		rank_file_p.close();
	}
}
void Ranking::Exit()
{
	DeleteGraph(game_rank_handle);
}