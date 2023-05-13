#include"BlockBase.h"
#pragma once

class Player;

//===============================================
//! ゴールクラス
//===============================================
class GoalArea :public BlockBase
{
private:

	static constexpr float stage_hight = 100.0f;

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して


public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	//!△の当たり判定用end_posを弄って最も上のposを探す
	float GetGoalAreaHight() const;

	GoalArea();
	~GoalArea();

	void Render();
	void Update();
	void Exit();

};

