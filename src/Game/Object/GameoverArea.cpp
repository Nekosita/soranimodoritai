#include"GameoverArea.h"
#include"../src/Game/Object/Model/Player.h"


//コンストラクタ
//---------------------------------
GameoverArea::GameoverArea()
{
	SetBlockStartPos(VGet(-32.0f * 200.0f, 5.0f, -32.0f * 200.0f));
	SetBlockEndPos(VGet(42 * 200.0f,6.0f, 42 * 200.0f));
}
//デストラクタ
//---------------------------------
GameoverArea::~GameoverArea()
{
}
//更新処理
//--------------------------------------
void GameoverArea::Update()
{
	const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
	const VECTOR& player_gravity_line_end = player_ptr->GetGravityLineEnd();

	//ブロックとの当たり判定
	//プレイヤーが上板を当たったら上にいます
	block_t1 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopB(), GetBlock1TopC());
	block_t2 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopD(), GetBlock1TopC());

}
//描画処理
//--------------------------------------
void GameoverArea::Render()
{

	SetUseLighting(FALSE);//光を消す
	SetUseZBufferFlag(TRUE);//ブロックを正しく描画
	SetWriteZBufferFlag(TRUE);
	DrawCube3D(start_pos, end_pos, GetColor(225, 20, 225), GetColor(225, 20, 225), TRUE); //立方体描画終了後にライティングを再びON
	SetUseLighting(TRUE);
}
//終了処理
//--------------------------------------
void GameoverArea::Exit()
{

}
