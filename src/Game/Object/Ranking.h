#pragma once
#include<fstream>

enum class GAME_SCREEN;
class Player;

//! ランククラス
//===============================================
class Ranking
{
private:

	int		score = 0;	//!<今のスコア
	int clear_counter = 0;//!<クリアの演出用のカウンター

	std::fstream rank_file;//!<rank_fileの設定
	int		rank_RL[5] = { 0 };//!<game_rankの最大数

	//パーフェクトランク設定
	std::fstream rank_file_p;	 //!<rank_fileの設定
	int		rank_RL_p[5] = { 0 };//!<game_rankの最大数

	int game_rank_handle;	//!<GAME_RANK画像実装	
	const Player* player_ptr = nullptr;	//!<プレイヤーを宣言して

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

    //! @name  点数の設定
    //!@{
    //! now_scoreを設定します。
    //! @param  [in]    now_score  現在のnow_score
	void SetScore(int now_score);
	int GetScore() const;
    //!@}
	
    //! @name  初期実装
    //!@{
	void GameRankInit();
	void InitGameClearAnime();
    //!@}
	
	//!ランクファイルを分類
	void Sort(int* now_tmp_rank_RL);

	//!ファイルの記録
	void NormalRankRecode(std::string& Filename);
	//!ファイルの記録
	void ParfectRankRecode(std::string& Filename);

	//!ファイルの読み込み
	void ReadNormalRankingFile();
	//!ファイルの読み込み
	void ReadParfectRankingFile();

	void RankProcess(GAME_SCREEN& now_screen);

	void DrawPetfectClearText();
	void DrawGameClearText();

	void DrawGameRankText();
	void Exit();
};

