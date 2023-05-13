#include"BlockBase.h"
#pragma once
class Player;

//! ゲームオーバークラス
//===============================================
class GameoverArea :public BlockBase
{
private:

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	GameoverArea();
	~GameoverArea();

	//！更新処理
	void Update();
	//！描画処理
	void Render();
	//！終了処理
	void Exit();
};

