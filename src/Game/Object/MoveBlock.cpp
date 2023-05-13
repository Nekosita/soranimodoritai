#include "MoveBlock.h"
#include"../src/Game/Object/Model/Player.h"


MoveBlock::MoveBlock(size_t block_num)
{

	move_block_num = int(block_num);

	//位置パラメータの用意
	constexpr VECTOR MOVEBLOCK_START_POS[] = {
		{-3.0f * 200.0f, 11600.0f, -8.0f * 200.0f},
		{3.0f * 200.0f, 12000.0f, -2.0f * 200.0f},
		{10.0f * 200.0f, 13000.0f, 11.0f * 200.0f}
	};
	constexpr VECTOR MOVEBLOCK_END_POS[] = {
		{2.0f * 200.0f, 11600.0f + STAGE_HIGHT, -3.0f * 200.0f},
		{8.0f * 200.0f, 12000.0f + STAGE_HIGHT, 3.0f * 200.0f},
		{15.0f * 200.0f, 13000.0f + STAGE_HIGHT, 16.0f * 200.0f}
	};

	//座標設定
	SetBlockStartPos(MOVEBLOCK_START_POS[move_block_num]);
	SetBlockEndPos(MOVEBLOCK_END_POS[move_block_num]);

	move_block_handle = MV1LoadModel("Data/Stage/0.005stone.mqoz");

	SetCount(0);//移動用するブロックのカウンター設定

}
MoveBlock::~MoveBlock()
{
	Exit();
}
void MoveBlock::SetCount(int now_count)
{
	block_count = now_count;
}
int MoveBlock::GetCount() const
{
	return block_count;
}
//------------------------
//移動するブロックの設定用
//----------------------
void MoveBlock::Update()
{
	//プレイヤーの当たり判定を渡す
	if (player_ptr == nullptr)	return;
	const VECTOR& player_top_line_start = player_ptr->GetTopLineStart();
	const VECTOR& player_top_line_end = player_ptr->GetTopLineEnd();
	const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
	const VECTOR& player_gravity_line_end = player_ptr->GetGravityLineEnd();

	//プレイヤーが下の板をぶつかったら強制出来に落ちます
	block_b1 = HitCheck_Line_Triangle(player_top_line_start, player_top_line_end,
		GetBlock1BottomA(), GetBlock1BottomB(), GetBlock1BottomC());

	block_b2 = HitCheck_Line_Triangle(player_top_line_start, player_top_line_end,
		GetBlock1BottomA(), GetBlock1BottomD(), GetBlock1BottomC());

	//プレイヤーが上板を当たったら上にいます
	block_t1 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopB(), GetBlock1TopC());

	block_t2 = HitCheck_Line_Triangle(player_gravity_line_start, player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopD(), GetBlock1TopC());

}
void MoveBlock::Move()
{
	//移動するブロックに移動用の変数を追加します
	block_count++;

	switch (move_block_num) {
	case 0:
		//カウンターに合わせて移動する
		if (block_count < 120){
			move_vector = { 0.0f,0.0f,30.0f };
			start_pos.z += 30.0f;
			end_pos.z += 30.0f;
		}
		else if ((block_count > 120) && (block_count < 240)){
			move_vector = { 0.0f,0.0f,-30.0f };
			start_pos.z -= 30.0f;
			end_pos.z -= 30.0f;
		}
		break;
	case 1:
		//カウンターに合わせて移動する
		if (block_count < 120){
			move_vector = { 32.0f,0.0f,0.0f };
			start_pos.x += 32.0f;
			end_pos.x += 32.0f;
		}
		else if ((block_count > 120) && (block_count < 240)){
			move_vector = { -32.0f,0.0f,0.0f };
			start_pos.x -= 32.0f;
			end_pos.x -= 32.0f;
		}
		break;
	case 2:
		//カウンターに合わせて移動する
		if (block_count < 120){
			move_vector = { 0.0f, 0.0f, 25.0f };
			start_pos.z += 25.0f;
			end_pos.z += 25.0f;
		}
		else if ((block_count > 120) && (block_count < 240)){
			move_vector = { 0.0f, 0.0f,-25.0f };
			start_pos.z -= 25.0f;
			end_pos.z -= 25.0f;
		}
		break;
	}

	if (block_count > 240){
		block_count = 0;//カウンター再設定
	}
}
VECTOR MoveBlock::GetMoveVector() const
{
	return move_vector;
}
void MoveBlock::Render()
{

	SetUseLighting(TRUE);//光を設定します

	//ブロックを正しく描画
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	DrawCube3D(start_pos, end_pos, GetColor(255, 217, 105), GetColor(255, 217, 105), TRUE);

	//テクスチャーの座標設定
	VECTOR block_pos;

	block_pos.x = (end_pos.x + start_pos.x) / 2;
	block_pos.y = start_pos.y + STAGE_HIGHT / 2;
	block_pos.z = (end_pos.z + start_pos.z) / 2;

	//テクスチャーの倍率設定
	VECTOR block_rate;

	block_rate.x = (end_pos.x - start_pos.x);
	block_rate.y = 150.05f;
	block_rate.z = (end_pos.z - start_pos.z);

	//ブロックの外見
	MV1SetScale(move_block_handle, VGet(block_rate.x, block_rate.y, block_rate.z));//モデルの大きさを変えます
	MV1SetRotationXYZ(move_block_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(move_block_handle, VGet(block_pos.x, block_pos.y, block_pos.z));
	MV1DrawModel(move_block_handle);

}