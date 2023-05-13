#include"../src/Game/Object/BlockBase.h"
#pragma once

class Player;


//! トラップクラス
//===============================================
class TrapArea :public BlockBase
{
private:

	int trap_num;//!<番号設定
	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して


public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	TrapArea(size_t _num);
	~TrapArea();

	void Render();
	void Update();
	void Exit();

};

