#include "MapChip.h"
#include "DxLib.h"


//コンストラクタ
//---------------------------------
MapChip::MapChip()
	: wall_handle(-1)
	,count(0)
	,swamp_handle(0)
	, swamp_x(0.0)
{

}
void MapChip::BuildMap()
{
	wall_handle = MV1LoadModel("Data/Stage/stone.mqoz");
	swamp_handle= MV1LoadModel("Data/Stage/swamp.mqoz");

	swamp_x = 0.0f;
	count = 0;
}
void MapChip::Update()
{
	count++;

	if (count > SWAMP_COUNT*2) {
		//リセット
		count = 0;
	}
	if (count < SWAMP_COUNT)
	{
		swamp_x -= 2.0f;
	}
	if ((count > SWAMP_COUNT)&&(count < SWAMP_COUNT*2))
	{
		swamp_x += 2.0f;
	}

}
//デストラクタ
//---------------------------------
MapChip::~MapChip() 
{
}
//地図作成
//---------------------------------
void MapChip::DrawMap()
{

	SetBackgroundColor(255, 255, 255);

	SetUseLighting(TRUE);//光を照らす
	SetUseZBufferFlag(TRUE);	//ブロックを正しく描画
	SetWriteZBufferFlag(TRUE);

	//沼を描画
	MV1SetScale(swamp_handle, VGet(90.0f, 1.2f, 90.0f));//モデルの大きさを変えます
	MV1SetRotationXYZ(swamp_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(swamp_handle, VGet(5.0f * 200.0f+(swamp_x), -10.0f, 5.0f * 200.0f));
	MV1DrawModel(swamp_handle);


	//壁の外見(厚みが100.0f)
	MV1SetScale(wall_handle, VGet(4.0f, 110.0f, 110.0f));//モデルの大きさを変えます
	MV1SetRotationXYZ(wall_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(wall_handle, VGet(-32.5f * 200.0f, 5000.0f, 5.0f * 200.0f));
	MV1DrawModel(wall_handle);

	//壁の外見(厚みが100.0f)
	MV1SetScale(wall_handle, VGet(110.0f, 110.0f, 4.0f));//モデルの大きさを変えます
	MV1SetRotationXYZ(wall_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(wall_handle, VGet(5 * 200.0f, 5000.0f, 42.5f * 200.0f));
	MV1DrawModel(wall_handle);

	//壁の外見(厚みが100.0f)
	MV1SetScale(wall_handle, VGet(4.0f, 110.0f, 110.0f));//モデルの大きさを変えます
	MV1SetRotationXYZ(wall_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(wall_handle, VGet(42.5 * 200.0f, 5000.0f, 5.0f * 200.0f));
	MV1DrawModel(wall_handle);

	//壁の外見(厚みが100.0f)
	MV1SetScale(wall_handle, VGet(110.0f, 110.0f, 4.0f));//モデルの大きさを変えます
	MV1SetRotationXYZ(wall_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(wall_handle, VGet(5 * 200.0f, 5000.0f, -32.5f * 200.0f));
	MV1DrawModel(wall_handle);

	SetUseLighting(TRUE);//立方体描画終了後にライティングを再びON
}
