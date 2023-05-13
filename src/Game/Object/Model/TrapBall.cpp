#include"TrapBall.h"
#include"../src/System/Vector3.h"
#include"Player.h"


//コンストラクタ
//----------------------------------
TrapBall::TrapBall(size_t ball_num)
{

	SetRadius(450.0f);//半径設定

	my_num = int(ball_num);

	constexpr VECTOR TRAPBALL_POS_LIST[] = {
	{-3.0f * 200.0f, 1200.0f, -30.0f * 200.0f},
	{40.0f * 200.0f, 5800.0f, -30.0f * 200.0f},
	{1.0f * 200.0f, 10000.0f, -30.0f * 200.0f},
	{9.0f * 200.0f, 10000.0f, -30.0f * 200.0f}
	};

	if (my_num < 0 || my_num >= 4) {
		my_num = 0;
	}

	//ボールが0番1番の場合
	if (my_num < 2 && my_num >= 0){
		SetEffect();
		model_handle = MV1LoadModel("Data/Stage/elc_rock.mqoz");//モデル設定
		pos=TRAPBALL_POS_LIST[my_num];//位置設定
	}
	//ボールが0番1番の場合
	if (my_num < 4 && my_num >= 2) {
		model_handle = MV1LoadModel("Data/Stage/rock.mqoz");//モデル設定
		pos = TRAPBALL_POS_LIST[my_num];//位置設定
	}


}
//デストラクタ
//---------------------------------
TrapBall::~TrapBall()
{
}
void TrapBall::SetEffect()
{

	//エフェクト設定
	ball_ef.Load("Data/Effect/Light1.efk");
	ball_ef.Play3D(true);
	ball_ef.rot = { 0,DX_PI_F / 3.0f,0 };
	ball_ef.scale = { 220,220,220 };

}
//更新処理
//------------------------------
void TrapBall::Update()
{

	//ボールの番号に合わせて処理を変えます
	switch (my_num)
	{
	case 0:
		pos.z += 70.0f;
		if (pos.z > 40 * 200.0f)pos.z = -30.0f * 200.0f;
		ball_ef.pos = pos;
		ball_ef.Update();
		break;
	case 1:
		pos.x -= 50.0f;
		pos.z += 50.0f;
		if ((pos.z > 40 * 200.0f) && (pos.x < -30 * 200.0f)) {
			pos.x = 40.0f * 200.0f;
			pos.z = -30.0f * 200.0f;
		}
		ball_ef.pos = pos;
		ball_ef.Update();
		break;
	case 2:
		pos.z += 50.0f;
		if (pos.z > 40 * 200.0f)pos.z = -30.0f * 200.0f;
		break;
	case 3:
		pos.z += 80.0f;
		if (pos.z > 40 * 200.0f)pos.z = -30.0f * 200.0f;
		break;
	}


}
//トラップボールの描画
//--------------------------------------
void TrapBall::Render()
{
	//ボールを描画します
	SetUseLighting(TRUE);

	//ボール表面の描画
	MV1SetScale(model_handle, VGet(4.5f, 4.5f, 4.5f));//モデルの大きさを変えます
	MV1SetRotationXYZ(model_handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(model_handle, pos);
	MV1DrawModel(model_handle);

	//エフェクトを描画
	ball_ef.Draw();

}
void TrapBall::Exit()
{

}
int TrapBall::SetKnockedOffVar() const
{
	return my_num;
}