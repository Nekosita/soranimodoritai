#pragma once
#include"ModelBase.h"
#include"../src/System/Effect.h"

class Player;

//! コンストラクタ
//-----------------------------------------------
class TrapBall :public ModelBase
{
private:

	int elc_rock_handle; //!<黄色のボール
	Effect ball_ef;
	int my_num = -1;

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	TrapBall(size_t ball_num);
	~TrapBall();

	//!エフェクトの初期設定
	void SetEffect();

	//!当たったボールの番号を返す
	int SetKnockedOffVar() const;

	//!更新処理
	void Update();
	//!描画処理
	void Render(); 
	//!終了処理
	void Exit();   
};

