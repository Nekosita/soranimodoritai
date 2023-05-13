#include"../src/System/Vector3.h"
#pragma once

class Vector3;
class Player;
class Item;

//! ユーザーインターフェースクラス
//-----------------------------------------------
class UserInterface
{
private:

	int mini_map; //!<右上の小さいマップ
	Vector3 mini_pos;
	int player_on_map;//!<プレイヤー座標(小さいマップ上の座標)
	Vector3 player_lm_pos;
	
	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

	const Item* item_ptr_mini[10];
	int time_count;//!<タイム
	int time_count_flame;//!<タイム計測
	int num;//!<現在のアイテム取得数	
	bool item_complete;	//!<アイテム全て揃った時
	int item_complete_counter;//!<アイテム全て揃った時用
	bool hint;//!<プレイヤーがアイテム集め終わって無い時

	int mute_handle;//!<ミュート画像
	int item_num;

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}
	void SetItem(Item* x, int num) {
		item_ptr_mini[num] = x;
	}

	
    //! @name ヒント用
    //!@{
	void SetItemComplete(bool now_item_complete);
	bool GetItemComplete() const;
	void SetItemCompleteCounter(int now_count);
	void ReduceItemCompleteCounter();
	int GetItemCompleteCounter() const;
    //!@}

	void SetItemNum(int now_num);
	int GetItemNum();

	void SetHint(bool now_hint);
	bool GetHint() const;

	void SetNum(int now_num);
	int Getnum() const;
	void AddNum();

	void SetTimeCount(int now_count);
	int GetTimeCount() const;
	void MinueTimeCount();
	void TimeCountFlame();
	
	void Init();
	void DrawMiniMap();
	void DrawUi();
	void DrawHintBar();
	void DrawGamepauseText(bool music_on, int sound);
	void ItemCompleteHint();
	void DrawGameoverText();
};

