#include"ModelBase.h"
#include"DxLib.h"

//コンストラクタ
//-----------------------------------------------
ModelBase::ModelBase() :
	model_handle(-1),
	model(-1),
	anim_no(0),
	totaltime(0),
	attachindex(-1),
	playtime(0.0f)
{


}
//デストラクタ
//-----------------------------------------------
ModelBase::~ModelBase()
{


}
//半径の設定
void ModelBase::SetAttackRadius(float now_radious)
{
	attackradius = now_radious;
}
//半径の取得
float ModelBase::GetAttackRadius() const
{
	return attackradius;
}