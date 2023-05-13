#include"TrapArea.h"
#include"../src/Game/Object/Model/Player.h"


//コンストラクタ
//--------------------------------------
TrapArea::TrapArea(size_t _num)
{
	trap_num = int(_num);

	//位置パラメータの用意
	constexpr VECTOR TRAPAREA_START_POS[] = {
		#include"TrapStartPos.inc"
	};
	constexpr VECTOR TRAPAREA_END_POS[] = {
		#include"TrapEndPos.inc"
	};

	//座標設定
	SetBlockStartPos(TRAPAREA_START_POS[trap_num]);
	SetBlockEndPos(TRAPAREA_END_POS[trap_num]);

}
//デストラクタ
//-------------------------------------
TrapArea::~TrapArea()
{

}
void TrapArea::Update()
{

	const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
	const VECTOR& player_gravity_line_end = player_ptr->GetGravityLineEnd();

	//ブロックの当たり判定
	//プレイヤーが上板を当たったら上にいます
	block_t1 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopB(), GetBlock1TopC());
	block_t2 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopD(), GetBlock1TopC());

}
void TrapArea::Render()
{	
	SetUseLighting(FALSE);//光を消す
	SetUseZBufferFlag(TRUE);//ブロックを正しく描画
	SetWriteZBufferFlag(TRUE);
	DrawCube3D(start_pos, end_pos, GetColor(150, 0, 0), GetColor(150, 0, 0), TRUE);
	SetUseLighting(TRUE);//立方体描画終了後にライティングを再びON
}
void TrapArea::Exit()
{

}
