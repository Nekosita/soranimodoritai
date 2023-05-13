#include"GoalArea.h"
#include"../src/Game/Object/Model/Player.h"


//ゲームクリアエリアの位置を取る為の関数
//---------------------------------
float GoalArea::GetGoalAreaHight() const
{
	return end_pos.y;
}
//コンストラクタ
//---------------------------------
GoalArea::GoalArea()
{
	//座標設定
	SetBlockStartPos(VGet(0.0f * 200.0f, 14300.0f, 15.0f * 200.0f));
	SetBlockEndPos(VGet(5.0f * 200.0f, 14300.0f + stage_hight, 20.0f * 200.0f));
}
//デストラクタ
//---------------------------------
GoalArea::~GoalArea()
{
}
//当たり判定のアップデート
//---------------------------------
void GoalArea::Update()
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
//ゲームオーヴァーエリアの描画
//---------------------------------
void GoalArea::Render()
{

	SetUseLighting(TRUE);
	DrawCube3D(start_pos, end_pos, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
	SetUseLighting(TRUE);

}

void GoalArea::Exit()
{

}
