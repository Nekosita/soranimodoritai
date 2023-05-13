#include"Camera.h"
#include"../src/Game/Object/Model/Player.h"



//カメラのコンストラクタ設定
//---------------------------------------------------------------------------------
Camera::Camera() :
	look{ (0.0f,0.0f,0.0f) },
	camera_dist(0.0f),
	camrea_h_rotate(0.0f),
	camrea_v_rotate(0.0f),
	camera_height(0.0f)
{

	//カメラ座標の初期化
	look = VGet(8.0f * 200.0f, 5000.0f, 28.0f * 200.0f);//カメラの見る座標
	pos = { -200.0f * 200.0f, 20000.0f, -200.0f * 200.0f };//カメラ座標

	oa_count = 0;	//OPENING_ANIME用カウンター
	now_camera = Camera::GAME_START;//一番最初のカメラのバージョン

	camera_height =500.0f;	//カメラとプレイヤーの距離を設定します
	camera_dist = 1500.0f;
	y_dis = 150.0f;

}
//カメラのデストラクタ設定
//---------------------------------
Camera::~Camera()
{
	Exit();
}
//カメラタイプの設定と取得の関数
//---------------------------------
void Camera::SetNowCamera(int camera_type_num)
{
	camera_type = camera_type_num;
}
int Camera::GetNowCamera() const
{
	return camera_type;
}
void Camera::SetPosY(float now_y)
{
	pos.y = now_y;
}
void Camera::SetPosWhenHit(float block_hight)
{
	SetPosY(block_hight - 300.0f);
}
//カメラーのグラビティラインの設定
//---------------------------------
void Camera::SetGravityLineStart(VECTOR& now_gravity_start)
{
	gravity_line_start = VGet(now_gravity_start.x, now_gravity_start.y + 50.0f, now_gravity_start.z);
}
VECTOR Camera::GetGravityLineStart() const
{
	return gravity_line_start;
}
void Camera::SetGravityLineEnd(VECTOR& now_gravity_end)
{
	gravity_line_end = VGet(now_gravity_end.x, now_gravity_end.y - 170.0f, now_gravity_end.z);
}
VECTOR Camera::GetGravityLineEnd() const
{
	return gravity_line_end;
}
//カメラーの頭上ラインの設定
//---------------------------------
void Camera::SetTopLineStart(VECTOR& now_top_start)
{
	top_line_start = VGet(now_top_start.x, now_top_start.y + 150.0f, now_top_start.z);
}
VECTOR Camera::GetTopLineStart() const
{
	return top_line_start;
}
void Camera::SetTopLineEnd(VECTOR& now_top_end)
{
	top_line_end = VGet(now_top_end.x, now_top_end.y + 250.0f, now_top_end.z);
}
VECTOR Camera::GetTopLineEnd() const
{
	return top_line_end;
}
//カメラカウンターの設定と取得の関数
//---------------------------------
void Camera::SetOpenAnimeCount(int now_OA_count)
{
	oa_count = now_OA_count;
}
int Camera::GetOpenAnimeCount() const
{
	return oa_count;
}
void Camera::CameraPosLimit()
{
	if (camera_pos.x < -29.5f * 200.0f) {
		camera_pos.x = -29.5f * 200.0f;
	}
	if (camera_pos.x > 39.5f * 200.0f) {
		camera_pos.x = 39.0f * 200.0f;
	}
	if (camera_pos.z > 39.5f * 200.0f) {
		camera_pos.z = 39.5f * 200.0f;
	}
	if (camera_pos.z < -29.5f * 200.0f) {
		camera_pos.z = -29.5f * 200.0f;
	}
}
//更新処理
//-------------------------------
void Camera::Update(int mov_)
{
	//もしもnullならその先の処理をしない(プレイヤーのデータがないなら前に行っても意味ない
	if (player_ptr == nullptr)	return;
	const VECTOR& player_pos = player_ptr->GetPos();
	const VECTOR& player_rot = player_ptr->GetRot();
	bool now_jump = player_ptr->GetIsJump();
	int now_camera_type = GetNowCamera();

	camrea_v_rotate = player_rot.y + TO_RADIAN(180.0f);//カメラの方向調整
	VECTOR look_at_pos = { (0.0f,0.0f,0.0f) };	//カメラが見ている座標
	camera_type = now_camera_type;

	//プレイヤーの今まで状態に応じてカメラモードを変更する
	switch (camera_type) {
	case Camera::OPENING_ANIME:
		oa_count++;
		if (oa_count < 90) {
			look_at_pos = VGet(4.0f * 200.0f, 0.0f, 4.0f * 200.0f);
			camera_pos = { 5.0f * 200.0f, 20500.0f, 5.0f * 200.0f };
		}
		if ((oa_count > 90) && (oa_count < 190)) {
			look_at_pos = VGet(5.0f * 200.0f, 16000.0f, 5.0f * 200.0f);
			camera_pos = { 45.0f * 200.0f, 16000.0f, 5.0f * 200.0f };
		}
		break;
	case Camera::GAME_START:
		look_at_pos = VGet(30.0f * 200.0f, 9000.0f, 30.0f * 200.0f);
		camera_pos = { -20.0f * 200.0f, 1000.0f, -20.0f * 200.0f };
		break;
	case  Camera::TPV:

		if (CheckHitKey(KEY_INPUT_W)) {
			camrea_h_rotate += 0.015f;
			if (camrea_h_rotate > 0.0f) {
				camrea_h_rotate = 0.0f;
			}
			camera_height += 25.0f;
			if (camera_height > 1500.0f) {
				camera_height = 1500.0f;
			}
		}
		if (CheckHitKey(KEY_INPUT_S)) {
			camera_height -= 25.0f;
			if (camera_height < 300.0f) {
				camera_height = 300.0f;
			}
			camrea_h_rotate -= 0.015f;
			if (camrea_h_rotate < -0.5f) {
				camrea_h_rotate = -0.5f;
			}
		}

		//TEST用カメラ
		if (CheckHitKey(KEY_INPUT_Q)) {

			look_at_pos = VGet((player_pos.x+ camera_pos.x)/2, (player_pos.y+ camera_pos.y)/2, (player_pos.z+ camera_pos.z)/2);

			camera_pos.x = look_at_pos.x+1500.0f;
			camera_pos.y = look_at_pos.y;
			camera_pos.z = look_at_pos.z;
		}
		else {

			look_at_pos = VGet(player_pos.x, player_pos.y, player_pos.z);

			camera_pos.x = look_at_pos.x - camera_dist * sinf(player_rot.y + TO_RADIAN(180.0f));
			camera_pos.y = look_at_pos.y + camera_height;
			camera_pos.z = look_at_pos.z - camera_dist * cosf(player_rot.y + TO_RADIAN(180.0f));

		}

		//カメラの限界値を設定する
		CameraPosLimit();
	break;
	case  Camera::GAME_CLEAR_MOVIE:
		look_at_pos = VGet(3.0f * 200.0f, 14000.0f, 18.0f * 200.0f);
		camera_pos = { -9.0f * 200.0f, 16000.0f, -9.0f * 200.0f };
		break;
	case  Camera::GAME_CLEAR:
		look_at_pos = VGet(0.0f, SCREEN_H / 2, 0.0f);           
		camera_pos = { 0.0f , SCREEN_H / 2 + 100.0f ,-600.0f };	
		break;
	case Camera::GAME_OVER:
		look_at_pos = VGet(0.0f, SCREEN_H / 2, 0.0f);           
		camera_pos = { 0.0f , SCREEN_H / 2 + 100.0f ,-600.0f }; 
		break;
	case Camera::GAME_PARFECT_CLEAR:
		look_at_pos = VGet(0.0f, SCREEN_H / 2, 0.0f);           
		camera_pos = { 0.0f , SCREEN_H / 2 + 100.0f ,-600.0f };
		break;
	}

	look = look_at_pos;
	pos = camera_pos;

}
//カメラ描画
//-------------------------------------------
void Camera::Render()
{
	if (camera_type == Camera::TPV) {
		SetCameraPositionAndTarget_UpVecY(pos, look);//カメラの描画
	}
	else {
		SetCameraPositionAndTarget_UpVecY(pos, look);//カメラの描画
	}
}
void Camera::Exit()
{

}
