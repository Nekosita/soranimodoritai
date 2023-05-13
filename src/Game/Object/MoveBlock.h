#pragma once
#include"BlockBase.h"

//!プレイヤーはクラスで伝えます
class Player;

//===============================================
//! ムーヴクラス
//===============================================
class MoveBlock :public BlockBase
{
private:

	int move_block_num;//!<作成されるムーヴブロックのナンバー
	int block_count;//!<ブロック移動用のカウンター
	int move_block_handle;//!<移動ブロック用のモデル

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

	VECTOR move_vector;

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	VECTOR GetMoveVector() const;
	
	MoveBlock(size_t block_num);
    virtual ~MoveBlock();

	   //! @name   移動用のカウンター設定
    //!@{
	void SetCount(int now_count);
	int  GetCount() const;
    //!@}
	
	
    //!更新処理
	void Update();
	void Move();
    //!終了処理
	void Render();
};

