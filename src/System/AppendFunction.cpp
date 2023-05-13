#include"DxLib.h"
#include"../src/Game/Object/Base.h"
#include"../src/Game/Object/BlockBase.h"
#include"../src/Game/Object/Model/Model.h"
#include"AppendFunction.h"

//接触関数3D
//-------------------------------------
bool AttachCheck(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
	float X = pos1.x - pos2.x;
	float Y = pos1.y - pos2.y;
	float Z = pos1.z - pos2.z;

	float length = sqrtf(X * X + Y * Y + Z * Z);

	if(length < r1 + r2) {
		return true;
	}
	return false;
}
//接触関数2D
//-------------------------------------
bool AttachCheck2D(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
	float X = pos1.x - pos2.x;
	float Y = pos1.y - pos2.y;
	float Z = pos1.z - pos2.z;

	float length = sqrtf(X * X + Z * Z);

	//距離が上下の5.0f近くの時
	if ((Y>-10.0f)&&(Y<100.0f))
	{
		if (length < r1 + r2) {
			return true;
		}
	}

	return false;
}
//プレイヤーの座標を小さいマップの上に載せた時、その座標換算用
//-------------------------------
float PosInMapX(const VECTOR& pos1)
{
	float map_x;
	float rate = 60.0f;
	//プレイヤーX-ワールドマップの左上X
	float length_x= pos1.x - (-32.0f * 200.0f);

	map_x = sqrtf(length_x* length_x);

	return map_x/rate;
}

float PosInMapY(const VECTOR& pos1)
{
	float map_y;
	float rate=60.0f;

	//プレイヤーZ-ワールドマップの左上Z
	float length_y = pos1.z - (42.0f * 200.0f);

	map_y = sqrtf(length_y * length_y);

	return map_y/rate;
}
//距離取る用の関数
//-----------------------------------------
float GetDistance(const VECTOR& pos1, const VECTOR& pos2)
{
	float distance;

	float X = pos1.x - pos2.x;
	float Y = pos1.y - pos2.y;
	float Z = pos1.z - pos2.z;

	distance= sqrtf(X * X + Y * Y + Z * Z);

	return distance;
}
// ==================================================
//当たり判定関数
// ==================================================
bool CheckHitStageObject(const Base* obj1, const Base* obj2, float addjust_y)
{
	return AttachCheck(obj1->GetPos(), obj1->GetRadius(),
		VGet(obj2->GetPos().x, obj2->GetPos().y + addjust_y, obj2->GetPos().z), obj2->GetRadius());
}
//指定されたボールとプレイヤーの当たり判定
//--------------------------------------------
bool IsHitBall(const ModelBase* ball, const ModelBase* player)
{
	return CheckHitStageObject(ball, player, 50.0f);
}
//ゴブリン攻撃モードに切り替え用
//--------------------------------------------
bool CheckHitWithEnemy(const Base* obj1, const ModelBase* obj2)
{
	return AttachCheck2D(obj1->GetPos(), 350.0f,
		obj2->GetPos(), obj2->GetAttackRadius());
}


