#include"Base.h"


//コンストラクタ
//-----------------------------------------------
Base::Base():
	pos{ (0.0f, 0.0f,0.0f) },
	rot{ (0.0f, 0.0f,0.0f) }
{
	
}
//デストラクタ
//----------------------------------------------
Base::~Base()
{

}
//プレイヤーの位置設定用の設定関数
//-----------------------------------------------
void Base::SetPos(const VECTOR& now_pos)
{
	pos = now_pos;
}
VECTOR Base::GetPos() const
{
	return pos;
}
//プレイヤーの当たり半径用の設定関数
//-----------------------------------------------
void Base::SetRadius(float now_radius)
{
	radius = now_radius;
}
float Base::GetRadius() const
{
	return radius;
}
void Base::Update()
{

}
void Base::Render()
{

}
void Base::Exit()
{

}

