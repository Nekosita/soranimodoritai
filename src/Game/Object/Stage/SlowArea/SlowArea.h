#include"../src/Game/Object/BlockBase.h"
#pragma once

class Player;

//! スローエリアクラス
//===============================================
class SlowArea :public BlockBase
{
private:

	int slow_num;

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	SlowArea(size_t _num);
	~SlowArea();

	void Update();
	void Render();
	void Exit();

};

