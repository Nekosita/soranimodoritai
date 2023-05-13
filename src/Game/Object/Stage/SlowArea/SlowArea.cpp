#include"SlowArea.h"
#include"../src/Game/Object/Model/Player.h"


//コンストラクタ
//-------------------------------
SlowArea::SlowArea(size_t _num)
{

	slow_num = int(_num);

	//位置パラメータの用意
	constexpr VECTOR SLOWBLOCK_START_POS[] = {
		#include"SlowStartPos.inc"
	};

	constexpr VECTOR SLOWBLOCK_END_POS[] = {
		#include"SlowEndPos.inc"
	};

	//座標設定
	SetBlockStartPos(SLOWBLOCK_START_POS[slow_num]);
	SetBlockEndPos(SLOWBLOCK_END_POS[slow_num]);
	
}
//デストラクタ
//------------------------------------
SlowArea::~SlowArea()
{
}
//当たり判定のアップデート
//--------------------------------------
void SlowArea::Update()
{
	//プレイヤーの当たり判定を渡す
	const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
	const VECTOR& player_gravity_line_end = player_ptr->GetGravityLineEnd();

	//ブロックの当たり判定
	block_t1 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopB(), GetBlock1TopC());

	block_t2 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopD(), GetBlock1TopC());

}
//描画用の関数
//---------------------------------------
void SlowArea::Render()
{

	SetUseLighting(FALSE);	//光を消す

	//ブロックを正しく描画
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);

	DrawCube3D(start_pos, end_pos, GetColor(0, 0, 150), GetColor(0, 0, 0), TRUE);
	SetUseLighting(TRUE);//光を戻す

}
void SlowArea::Exit()
{

}
