#include"RedGoblin.h"
#include"ModelBase.h"
#include"../src/System/Random.h"
#include"Player.h"
#include"DxLib.h"
#include<vector>
#include<array>


//コンストラクタ
//-----------------------------------------------
RedGoblin::RedGoblin(size_t num) :
	move_count(0),
	is_move(false),
	is_live(true)
{

	//ゴブリンモデル実装
	model_handle = MV1LoadModel("Data/Character/RedGoblin/RedGoblin.mv1");
	attachindex = MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
	totaltime = MV1GetAttachAnimTotalTime(model_handle, attachindex);

	//ゴブリンの初期状態
	goblin_action = STAY;

	MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));//モデルの大きさを変えます

	//ゴブリンの位置設定 
	constexpr VECTOR GOBLIN_POS_LIST[] = {
		{-12.0f * 200.0f, 3450.0f, -7.0f * 200.0f},
		{-12.0f * 200.0f, 3450.0f, 20.0f * 200.0f},
		{-12.0f * 200.0f, 750.0f,  15.0f * 200.0f}
	};
	//item_numが配列外の場合、0にする
	if (num < 0 || num >= 3)
	{
		num = 0;
	}
	pos = GOBLIN_POS_LIST[num];

	//開始な時は2秒
	move_count = 120;//移動用開始カウンター
	fix_move_distance = 75.0f;//ゴブリンの移動距離設定

	//ゴブリンの角度設定
	float Start_rot = TO_RADIAN(180.0f);
	rot = { 0.0f, Start_rot, 0.0f };

	SetIsShoot(false);
	bullet_pos = { (0.0f,0.0f,0.0f) }; //初期化

	target_rot = 0.0f;//移動先の角度を0にする
	is_move = false;//今はまだ動いでない

	is_attatched = false;//固定距離になったら離れます

	SetAttackRadius(2000.0f);//攻撃半径

	root_node = MV1SearchFrame(model_handle, "root");
	CreateIdentityMatrix(&identity);

	//feeling_barの初期設定 

	//モデル実装
	feel_bar_model[0] = MV1LoadModel("Data/Character/feeling_bar/normal.mqoz");
	feel_bar_model[1] = MV1LoadModel("Data/Character/feeling_bar/attack.mqoz");
	feel_bar_pos = pos;//ゴブリンの位置と同じ
	feel_bar_pos.y = pos.y + 450.0f;//でもゴブリンの位置より高い
	feel_bar_rot = rot;//ゴブリンのロットと同じ

}
RedGoblin::~RedGoblin()
{
	Exit();
}
void RedGoblin::PlayAnimation()
{
	playtime += 0.5f;//アニメーションを進める

	if (totaltime < playtime) {
		playtime = 0;
		MV1DetachAnim(model_handle, attachindex);
		attachindex =
			MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
		totaltime = MV1GetAttachAnimTotalTime(model_handle, attachindex);
	}
	MV1SetAttachAnimTime(model_handle, attachindex, playtime);//再生時間をセットする

}
VECTOR RedGoblin::GetRot() const
{
	return rot;
}
void RedGoblin::ResetRot()
{
	is_move = false;
	int re_start_move_count = GetRandomI(1, 3);
	move_count = re_start_move_count * 60;
}
//更新処理
//---------------
void RedGoblin::Update()
{
	bool is_appect = GetIsAttatched();
	const VECTOR& player_rot = player_ptr_inr->GetRot();
	const VECTOR& player_pos = player_ptr_inr->GetPos();

	//アニメーションの処理をする
	PlayAnimation();

	move_count--;//移動カンター減る

	if (is_appect) goblin_action = SHOOT;

	if (!is_appect) {
		if (move_count > 0) {
			goblin_action = STAY;
			is_move = false;
		}
		else if (move_count <= 0) {
			goblin_action = MOVE;
		}
	}

	switch (goblin_action) {
	case RedGoblin::STAY:
		//ゴブリンのニメーション設定
		SetAnimation(anim_handle, 1);
		Fixpos(pos);//位置固定
		FeelingBarInStay();//Feelingbarの処理
		break;
	case RedGoblin::MOVE:
		//ムーヴをセット
		if (!is_move) {
			SetPosBeforeMove(pos, rot.y);
		}

		if (is_move) {
			//アニメーション設定
			SetAnimation(anim_handle, 2);
			//方向設定
			rot.y = target_rot;
			//指定された方向で進む
			pos.x -= GOBLIN_NORMAL_SPEED * sinf(rot.y);
			pos.z -= GOBLIN_NORMAL_SPEED * cosf(rot.y);

			if (move_count <= (-180)) {
				ResetRot();
			}
		}
		FeelingBarInMove();//Feelingbarの処理
		break;
	case RedGoblin::SHOOT:
		SetAnimation(anim_handle, 0);
		VECTOR Standerd = { 0.0f , 0.0f , -1.0f };//基準用ゴブリンの角度
		VECTOR Goblin_to_player = { 0.0f , 0.0f , 0.0f };//ゴブリンがプレイヤーに向けている方向の初期値
		//プレイヤーに向かっているベクターを作る 
		//VECTORを基準化
		Goblin_to_player.x = (player_pos.x - pos.x);
		Goblin_to_player.z = (player_pos.z - pos.z);

		//計算した角度はプラスとマイナスがない
		float roty = CalculateAngle(Standerd, Goblin_to_player);

		if (count == 0) {
			//弾丸の角度を再設定
			bullet_rot = rot.y;
		}

		//それを位置に合わせて調整します
		roty *= -1;
		if (player_pos.x < pos.x)roty = -(roty);
		rot.y = roty;

		count++;
		//2秒を越えられないならリロード
		Reload();

		//bulletの角度に応じて前進する
		bullet_pos.x += (float)((-1) * (BULLET_SPEED * sin(bullet_rot)));
		bullet_pos.z += (float)((-1) * (BULLET_SPEED * cos(bullet_rot)));

		FeelingBarInMove();//Feelingbarの処理
		feel_bar_rot.y = roty;

		break;
	}

	MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//描画処理
//---------------
void RedGoblin::Render()
{
	//平常状態の描画設定
	MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
	MV1SetPosition(model_handle, pos);
	MV1DrawModel(model_handle);

	//feeling_barの描画
	MV1SetScale(feel_bar_model[0], VGet(0.35f, 0.35f, 0.35f));//モデルの大きさを変えます
	MV1SetRotationXYZ(feel_bar_model[0], VGet(feel_bar_rot.x, feel_bar_rot.y, feel_bar_rot.z));
	MV1SetPosition(feel_bar_model[0], feel_bar_pos);
	MV1DrawModel(feel_bar_model[0]);

	//攻撃状態の描画設定
	switch (goblin_action) {
	case RedGoblin::SHOOT:
		//攻撃表示用feeling_bar描画
		SetUseLighting(FALSE);	//!<ライティングOFF
		MV1SetScale(feel_bar_model[1], VGet(0.35f, 0.35f, 0.35f));//モデルの大きさを変えます
		MV1SetRotationXYZ(feel_bar_model[1], VGet(feel_bar_rot.x, feel_bar_rot.y, feel_bar_rot.z));
		MV1SetPosition(feel_bar_model[1], feel_bar_pos);
		MV1DrawModel(feel_bar_model[1]);
		SetUseLighting(TRUE);//!<ライティングON
		//弾丸描画
		DrawSphere3D(VGet(bullet_pos.x, bullet_pos.y + 200.0f, bullet_pos.z), bullet_radius, 32, GetColor(255, 100, 0), GetColor(255, 0, 0), TRUE);
		break;
	}
	//立方体描画終了後にライティングを再びON

}
void RedGoblin::SetDiedPos()
{
	pos = { 0.0f, -500.0f, 0.0f };
}
void RedGoblin::SetAtariRadius(float now_atari_radius)
{
	atari_radius = now_atari_radius;
}
float RedGoblin::GetAtariRadius() const
{
	return atari_radius;
}
void RedGoblin::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
	if (anim_no != new_anim_no) {
		MV1DetachAnim(model_handle, attachindex);
		anim_no = new_anim_no;
		attachindex = MV1AttachAnim(model_handle, 0, handle[anim_no], FALSE);
		totaltime = MV1GetAttachAnimTotalTime(model_handle, attachindex);
	}
}
void RedGoblin::SetPosBeforeMove(VECTOR& pos, float rot)
{

	pos_before_move = pos;//動く前の位置を記録

	float Rand = GetRandomF(0.0f, 360.0f);//ランダム(0~360)な数値を設定

	target_rot = TO_RADIAN(Rand);//それをターゲットとして設定

	rot = target_rot;

	is_move = true;//もう動いでいい

}
//固定位置用の関数
//--------------------
void RedGoblin::Fixpos(VECTOR& pos)
{
	this->pos = { pos.x, pos.y, pos.z };
}
float RedGoblin::CalculateAngle(const VECTOR& V1, const VECTOR& V2)
{
	float ans;
	ans = ((V1.x * V2.x) + (V1.y * V2.y) + (V1.z * V2.z)) /
		((sqrtf((V1.x * V1.x) + (V1.y * V1.y) + (V1.z * V1.z))) * (sqrtf((V2.x * V2.x) + (V2.y * V2.y) + (V2.z * V2.z))));

	ans = acosf(ans);

	return ans;
}
//終了処理
//---------------
void RedGoblin::Exit()
{
	MV1DeleteModel(model_handle);
}
void RedGoblin::SetIsLive(bool now_live)
{
	is_live = now_live;
}
bool RedGoblin::GetIsLive() const
{
	return is_live;
}
void RedGoblin::SetIsAttatched(bool now_)
{
	is_attatched = now_;
}
bool RedGoblin::GetIsAttatched() const
{
	return is_attatched;
}
void RedGoblin::SetBulletPos(const VECTOR& now_pos)
{
	bullet_pos = now_pos;
}
VECTOR RedGoblin::GetBulletPos() const
{
	return bullet_pos;
}

float RedGoblin::GetBulletRadius() const
{
	return bullet_radius;
}
void RedGoblin::SetIsShoot(bool now_is_shoot)
{
	is_shoot = now_is_shoot;
}
bool RedGoblin::GetIsShoot() const
{
	return is_shoot;
}
void RedGoblin::Reload()
{
	// 2秒の後再判定します
	if (count > 120) {
		// もう一度弾の行き先を判定します
		SetIsShoot(false);//処理終わってたらフォルスになる
		count = 0;		  //countを０にする
	}
}
void RedGoblin::FeelingBarInStay()
{
	// feelingバーが回る
	feel_bar_pos = pos;//ゴブリンの位置と同じ
	feel_bar_pos.y = pos.y + 450.0f;//でもゴブリンの位置より高い
	feel_bar_rot.y += TO_RADIAN(5.0f);
}
void RedGoblin::FeelingBarInMove()
{
	// feelingバーが回る
	feel_bar_pos = pos;//ゴブリンの位置と同じ
	feel_bar_pos.y = pos.y + 450.0f;//でもゴブリンの位置より高い
	feel_bar_rot.y += TO_RADIAN(5.0f);

}
void RedGoblin::FeelingBarInShoot()
{
	// feelingバーがプレイヤーに向いています
	feel_bar_pos = pos;//ゴブリンの位置と同じ
	feel_bar_pos.y = pos.y + 450.0f;//でもゴブリンの位置より高い
}

