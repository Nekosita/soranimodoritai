#pragma once
#include "Player.h"
#include "../MoveBlock.h"
#include "../src/System/Vector3.h"
#include "../src/System/KeyInput.h"
#include "DxLib.h"

//プレイヤーが１体の時しか使えない
//-----------------------------------------------
namespace
{
Player* instance = nullptr;   // プレイヤー
}

//トラップボールとの当たった後の処理関数
//-----------------------------------------------
bool IsNormalTrapBall(int num)
{
    return (num < 0 || num > 3);
}
bool IsDamageTrapBall(int num)
{
    return (num == 0 || num == 1);
}
void Player::HitTrapBall(int num, const Base& ball)
{
    if(IsNormalTrapBall(num))
        return;

    if(IsDamageTrapBall(num)) {
        SetHurted(true);
        return;
    }

    // 2, 3
    Vector3 dir;
    float   radius;
    dir    = {VGet(GetPos().x, GetPos().y + 50.0f, GetPos().z) - ball.GetPos()};
    radius = ball.GetRadius() + GetRadius();
    dir.SetLength(radius);
    SetPosWithBall(ball.GetPos(), dir);
}
//プレイヤーを取得
//-----------------------------------------------
Player* Player::GetPlayer()
{
    return instance;
}
//コンストラクタ
//-----------------------------------------------
Player::Player()
    : anim_no_in_swamp(0)
    , totaltime_in_swamp(0)
    , anim_no_got_shocked(0)
    , totaltime_got_shocked(0)
    , now_fall(false)
    , life_x(0.0f)
{
    instance = this;
    // 普通のモデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/mouse/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Neutral Idle.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);

    // 毒沼にいる時のモデル設定
    char str_in_swamp[2048];
    //ここはGameフォルダ
    GetCurrentDirectory(1024, str_in_swamp);
    // 強制mouseフォルダからセットします
    std::string strM_In_Swamp = std::string(str_in_swamp) + "/Data/Character/mouse_in_swamp/";
    SetCurrentDirectory(strM_In_Swamp.c_str());
    model_handle_in_swamp = MV1LoadModel("Neutral Idle.mv1");
    // Gameフォルダに戻します
    SetCurrentDirectory(str_in_swamp);

    //黄色いボール（電撃ある）にぶつかられた時のモデル
    char str_got_shocked[2048];
    //ここはGameフォルダ
    GetCurrentDirectory(1024, str_got_shocked);
    // 強制mouseフォルダからセットします
    std::string Str_got_shocked = std::string(str_got_shocked) + "/Data/Character/mouse_got_shocked/";
    SetCurrentDirectory(Str_got_shocked.c_str());
    model_handle_got_shocked = MV1LoadModel("Female Dance Pose.mv1");
    // Gameフォルダに戻します
    SetCurrentDirectory(str_got_shocked);

    //エフェクト設定
    player_ef.Load("Data/Effect/Fire6.efk");
    player_ef.Play3D(true);
    player_ef.rot   = {0, DX_PI_F / 3.0f, 0};
    player_ef.scale = {200, 200, 200};

    // 普通のアニメーションの初期設定
    attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);

    // 毒入りのアニメーションの初期設定
    attachindex_in_swamp = MV1AttachAnim(model_handle_in_swamp, 1, anim_handle_in_swamp[anim_no_in_swamp], FALSE);
    totaltime_in_swamp   = MV1GetAttachAnimTotalTime(model_handle_in_swamp, attachindex_in_swamp);

    // 黄色いボール（電撃ある）にぶつかられた時のモデルの初期設定
    attachindex_got_shocked =
        MV1AttachAnim(model_handle_got_shocked, 1, anim_handle_got_shocked[anim_no_got_shocked], FALSE);
    totaltime_got_shocked = MV1GetAttachAnimTotalTime(model_handle_got_shocked, attachindex_got_shocked);

    // モデルの大きさ設定
    MV1SetScale(model_handle, VGet(1.5f, 1.5f, 1.5f));

    vertex_shader = LoadVertexShader("shader/SampleVS.vso");
    pixel_shader  = LoadPixelShader("shader/SamplePS.pso");

    // プレイヤーの状態の初期設定
    // プレイヤー初期座標の設定
    VECTOR start_pos = {20 * 200.0f, 10700, -2 * 200.0f};
    SetPos(start_pos);

    //プレイヤー初期のrotを設定
    VECTOR start_rot{0.0f, 180.0f, 0.0f};
    SetRot(start_rot);

    is_jump     = false;                   //最初はジャンプしていない
    on_ground   = true;                    //地面にいない
    now_fall    = false;                   //落下していない
    mov_y       = 0.0f;                    //ジャンプ距離の初期化
    gravity     = 0.0f;                    //重力の初期化
    is_stepped  = false;                   //最初は踏めてない
    is_collided = false;                   //上にも立ってない
    SetRadius(125.0f);                     //当たり半径設定
    SetHp(500.0f);                         //最初のHPの設定
    slow_down = 0.0f;                      //初期値設定
    is_run    = false;                     //最初走ってない
    stand_by  = rot.y + TO_RADIAN(0.0f);   //is_run = false; の時使ったロット

    // 重力ラインの初期設定
    SetGravityLineStart(pos);
    SetGravityLineEnd(pos);

    // 頭上ラインの初期設定
    SetTopLineStart(pos);
    SetTopLineEnd(pos);

    // 当たられた時の関数
    SetHurted(false);    //最初は当たられてない
    flame_count = 60;    //当たられた時点滅用のフレームカウンター
    move_count  = 180;   //停止時間用カウンター

    root_node = 1;
    CreateIdentityMatrix(&identity);

    // プレイヤーダメージエフェクト用
    damage_graph = LoadGraph("Data/Effect/damage_effect.png");

    // feeling_barの初期設定
    feel_bar_model[0] = MV1LoadModel("Data/Character/feeling_bar/glad.mqoz");
    feel_bar_model[1] = MV1LoadModel("Data/Character/feeling_bar/slow.mqoz");
    feel_bar_pos      = pos;              //プレイヤーの位置と同じ
    feel_bar_pos.y    = pos.y + 250.0f;   //プレイヤーの位置より高い
    feel_bar_rot      = rot;              //プレイヤーのロットと同じ
    feel              = 3;                //<最初は何も感じない
    varity            = -50.0f;
    feel_count        = 0;
    SetIsGlad(false);

    //待機用モード用
    player_is_boring = false;
    boring_count     = 120;

    damage_a_in_swamp = 126;   //毒沼に居る時のα値の初期化
    damage_a          = 126;   //毒沼に居る時のα値の初期化

    knocked_off_variation = VGet(0.0f, 0.0f, 0.0f);   //初期化
    now_player            = COMMON;

    jump_count = 120;   //ジャンプカウンター
    SetToHintBar(false);

    // ゴールバー用の変数初期化
    is_goal_actived = false;
    SetIsClear(false);
}
//デストラクタ
//---------------------------------
Player::~Player()
{
    Exit();
    instance = nullptr;
}
void Player::SetPosWithBall(const VECTOR& now_pos, const Vector3& dir)
{
    pos.x = now_pos.x + dir.x;
    pos.y = now_pos.y + dir.y;
    pos.z = now_pos.z + dir.z;
}
void Player::SetPosOnMoveBlock(const Vector3& dir)
{
    pos.x += dir.x;
    pos.y += dir.y;
    pos.z += dir.z;
}
void Player::PlayerPosLimit()
{
    if(pos.x > 40.0f * 200.0f)
        pos.x = 40.0f * 200.0f;
    if(pos.x < -30.0f * 200.0f)
        pos.x = -30.0f * 200.0f;
    if(pos.z > 40.0f * 200.0f)
        pos.z = 40.0f * 200.0f;
    if(pos.z < -30.0f * 200.0f)
        pos.z = -30.0f * 200.0f;
}
//プレイヤーの回転角度用の設定関数
//---------------------------------
void Player::SetRot(const VECTOR& now_rot)
{
    rot = now_rot;
}
VECTOR Player::GetRot() const
{
    return rot;
}
VECTOR Player::GetPlayerModelRot() const
{
    return player_model_rot;
}
//踏んでいるのかとぶつかっているのかの関数
//---------------------------------
void Player::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
bool Player::GetIsStepped() const
{
    return is_stepped;
}
//プレイヤーが喜んでいるかいないかを設定のする関数
//---------------------------------
void Player::SetIsGlad(bool now_is_glad)
{
    is_glad = now_is_glad;
}
bool Player::GetIsGlad() const
{
    return is_glad;
}
//ブロックとぶつかったのがどうがを設定する用の関数
//---------------------------------
void Player::SetIsCollided(bool now_is_collided)
{
    is_collided = now_is_collided;
}
bool Player::GetIsCollided() const
{
    return is_collided;
}
//プレイヤーの痛みを感じる時設定する用の関数
//---------------------------------
void Player::SetHurted(bool now_hurted)
{
    hurted = now_hurted;
}
bool Player::GetHurted() const
{
    return hurted;
}
//プレイヤーの重力ラインの設定
//---------------------------------
void Player::SetGravityLineStart(VECTOR& now_gravity_start)
{
    gravity_line_start = VGet(now_gravity_start.x, now_gravity_start.y + 50.0f, now_gravity_start.z);
}
VECTOR Player::GetGravityLineStart() const
{
    return gravity_line_start;
}
void Player::SetGravityLineEnd(VECTOR& now_gravity_end)
{
    gravity_line_end = VGet(now_gravity_end.x, now_gravity_end.y - 170.0f, now_gravity_end.z);
}
VECTOR Player::GetGravityLineEnd() const
{
    return gravity_line_end;
}
//プレイヤーの頭上ラインの設定
//---------------------------------
void Player::SetTopLineStart(VECTOR& now_top_start)
{
    top_line_start = VGet(now_top_start.x, now_top_start.y + 150.0f, now_top_start.z);
}
VECTOR Player::GetTopLineStart() const
{
    return top_line_start;
}
void Player::SetTopLineEnd(VECTOR& now_top_end)
{
    top_line_end = VGet(now_top_end.x, now_top_end.y + 250.0f, now_top_end.z);
}
VECTOR Player::GetTopLineEnd() const
{
    return top_line_end;
}
//プレイヤーのHP関係の関数設定
//---------------------------------
void Player::SetHp(float now_Hp)
{
    life_x = now_Hp;
}
float Player::GetHp() const
{
    return life_x;
}
float Player::GetLifeX() const
{
    return life_x;
}
//プレイヤーの感情関係設定
//---------------------------------
void Player::SetFeel(int now_feel)
{
    feel = now_feel;
}
int Player::GetFeel() const
{
    return feel;
}
void Player::HpDecrese()
{
    life_x -= 0.05f;
}
void Player::BulletDamage()
{
    life_x -= 3.0f;
}
void Player::HpDamageInSwamp()
{
    SetInSwamp(true);
    life_x -= 0.5f;
}
void Player::GotShocked()
{
    SetAnimation(anim_handle, 1);
    PlayAnimationGotShocked();

    move_count--;
    flame_count--;

    life_x -= 0.1f;

    if(flame_count < 0)
        flame_count = 60;

    if(move_count < 0) {
        move_count  = 120;   //カウンターリセット
        flame_count = 60;
        hurted      = false;   //PLAYER_STATUS::COMMONに入ります
    }
}
void Player::SetInSwamp(bool Now_in_swamp)
{
    in_swamp = Now_in_swamp;
}
void Player::SetSlowDown(float now_slow_down)
{
    slow_down = now_slow_down;
}
void Player::JumpProcess(bool now_is_collided)
{
    //もしもプレイヤーが動いてないなら
    if((!is_run) && !(is_jump)) {
        NowPlayerIsBoring();
    }

    //地面から飛べる時とブロックから飛ぶ時の判断
    IsJump();

    //飛んだら
    if(is_jump) {
        mov_y += JUMP_SPEED;
        OnResistance();
        //もしもブロックにぶつかったらリセット
        if(now_is_collided) {
            JumpReset();
            now_fall = true;
        }
    }
    //飛んでいた時と落ちた時重力起動
    GravityInJump();

    //調整量は常に重力追加
    mov_y -= ADD_GRAVITY;

    //プレイヤーY座標＝　(調整量)　+（重力補正）
    pos.y += (mov_y) + (gravity);

    //地面に居てそして飛んでいないなら
    if((pos.y < 0.0f) && !is_jump) {
        JumpResetGround();
    }
}
void Player::IsJump()
{
    //地面から飛んた場合（ジャンプしてないと落ちてない時のみジャンプ）
    if(PushHitKey(KEY_INPUT_SPACE) && (!is_jump) && (!now_fall)) {
        JumpOnTheGround();
    }

    //階段に踏んだ場合
    if(is_stepped) {
        is_run = false;
        //ジャンプの中初期化
        JumpReset();
        //ブロックから飛んた場合（ジャンプしてないと落ちでない時のみジャンプ）
        if(PushHitKey(KEY_INPUT_SPACE)) {
            JumpOnTheBlock();
        }
    }
}
void Player::JumpReset()
{
    //ジャンプの中初期化用変数
    mov_y    = 0.0f;
    gravity  = 0.0f;
    is_jump  = false;
    now_fall = false;
}
void Player::JumpResetGround()
{
    //ジャンプの中初期化用変数
    pos.y    = 0.0f;
    mov_y    = 0.0f;
    gravity  = 0.0f;
    is_jump  = false;
    now_fall = false;
}
void Player::JumpHit()
{
    //ジャンプの中初期化用変数
    if(mov_y > 0)
        mov_y = 0.0f;
    is_jump  = false;
    now_fall = true;
}
void Player::JumpOnTheBlock()
{
    player_is_boring = false;   //動いたら待機実行しない
    boring_count     = 180;     //カウンターもリセット
    is_run           = true;
    is_jump          = true;
}
void Player::JumpOnTheGround()
{
    player_is_boring = false;   //動いたら待機実行しない
    boring_count     = 180;     //カウンターもリセット
    is_run           = true;
    is_jump          = true;
    is_stepped       = false;
}
void Player::OnResistance()
{
    is_jump  = false;   //重力加速度は常にかかり続けるほうが良い
    now_fall = true;    //本来このフラグは必要ない mov_yの方向でわかる
}
void Player::GravityInJump()
{
    if(is_jump || now_fall) {
        gravity -= 0.5f;
    }
}
void Player::SetTopWhenHit(float block_hight)
{
    SetPosY(block_hight - 300.0f);
    SetIsCollided(true);
    JumpHit();
}
void Player::SetDownWhenHit(float block_hight)
{
    SetIsStepped(true);
    SetPosY(block_hight);
}

bool Player::IsJumpUp()
{
    return mov_y > 0;
}
//ゲームクリアの時の演出
//---------------------------------
void Player::Clear()
{
    pos.y += 27.0f;
    player_model_rot.y += TO_RADIAN(6.0f);
}
void Player::SetPosY(float fix_y)
{
    pos.y = fix_y;
}
//アニメの設定
//---------------------------------
void Player::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    switch(now_player) {
    case HURT:
        //電撃ボールに当たった場合
        if(anim_no_got_shocked != new_anim_no) {
            MV1DetachAnim(model_handle_got_shocked, attachindex_got_shocked);
            anim_no_got_shocked     = new_anim_no;
            attachindex_got_shocked = MV1AttachAnim(model_handle_got_shocked, 1, handle[anim_no_got_shocked], FALSE);
            totaltime_got_shocked   = MV1GetAttachAnimTotalTime(model_handle_got_shocked, attachindex_got_shocked);
        }
        break;
    case COMMON:
        if(anim_no != new_anim_no) {
            MV1DetachAnim(model_handle, attachindex);
            anim_no     = new_anim_no;
            attachindex = MV1AttachAnim(model_handle, 1, handle[anim_no], FALSE);
            totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
        }
        break;
    case IN_SWAMP:
        //毒沼に居る場合
        if(anim_no_in_swamp != new_anim_no) {
            MV1DetachAnim(model_handle_in_swamp, attachindex_in_swamp);
            anim_no_in_swamp     = new_anim_no;
            attachindex_in_swamp = MV1AttachAnim(model_handle_in_swamp, 1, handle[anim_no_in_swamp], FALSE);
            totaltime_in_swamp   = MV1GetAttachAnimTotalTime(model_handle_in_swamp, attachindex_in_swamp);
        }
        break;
    }
}
void Player::ResetPlayerIsBoring()
{
    player_is_boring = false;   //動いたら待機実行しない
    boring_count     = 180;     //カウンターもリセット
}
void Player::NowPlayerIsBoring()
{
    //退屈すぎて踊り用のカウンターが始まる
    boring_count--;
    //3秒経過してまだ動いていないなら　
    if(boring_count < 0) {
        player_is_boring = true;
    }
    else {
        player_is_boring = false;
    }
}
//プレイヤーとブロックの確認用の関数
//--------------------------------------------------
//判定が被らないように　先に上と下の判定を消します
//--------------------------------------------------
void Player::ResetBeforeCheck()
{
    SetIsStepped(false);
    SetIsCollided(false);
}
//プレイヤーとブロックの当たり判定
// --------------------------------------------------
bool Player::CheckHitGoalBar(bool is_on_goal, int ckeck_num, int total_num, float hight)
{
    if(is_on_goal) {
        is_goal_actived = true;
        SetIsStepped(is_goal_actived);
        SetPosY(hight);   //ここで止で欲しいY座標をプレイヤーに渡す
        if(ckeck_num == total_num) {
            SetIsClear(true);
        }
        else {
            SetToHintBar(true);
        }
    }
    else {
        SetToHintBar(false);
        is_goal_actived = false;
    }
    return is_goal_actived;
}
bool Player::GetGoalActived() const
{
    return is_goal_actived;
}
void Player::SetToHintBar(bool now_hint)
{
    to_hint_bar = now_hint;
}
bool Player::GetToHintBar() const
{
    return to_hint_bar;
}
void Player::SetIsClear(bool now_is_clear)
{
    is_clear = now_is_clear;
}
bool Player::GetIsClear() const
{
    return is_clear;
}
//プレイヤーとステージブロックの当たり用
//--------------------------------------------------
bool Player::HitWithStageBlock(const BlockBase* block)
{
    if(IsJumpUp()) {
        SetIsCollided(false);
        //上昇中の時
        //当たてないなら処理は戻る
        if(!block->CheckHitStageBottom()) {
            return false;
        }
        //当たてたら処理はこちに来ます
        SetTopWhenHit(block->GetBlockHight());
        return true;
    }
    else {
        //下から上何がぶつかったら処理に戻る
        if(GetIsCollided())
            return false;
        //当たてないなら処理は戻る
        if(!block->CheckHitStageFloor())
            return false;
        //当たてたら処理はこちに来ます
        SetDownWhenHit(block->GetBlockHight());
        return true;
    }
    return false;
}
bool Player::HitWithMoveBlock(const MoveBlock* block)
{
    //全ての判定先に初期化します
    on_move_block = false;
    if(IsJumpUp()) {
        SetIsCollided(false);
        //上昇中の時
        if(!block->CheckHitStageBottom()) {
            return false;
        }
        // 当たった時
        on_move_block = true;
        SetTopWhenHit(block->GetBlockHight());
        return true;
    }
    else {
        if(GetIsCollided())
            return false;
        if(!block->CheckHitStageFloor())
            return false;
        // 当たった時
        on_move_block = true;
        SetDownWhenHit(block->GetBlockHight());
        //ブロックのヴェクターを渡します
        SetPosOnMoveBlock(block->GetMoveVector());
        return true;
    }
    return on_move_block;
}
bool Player::GetOnMoveBlock() const
{
    return on_move_block;
}
//点滅演出
//---------------------------------------------------------
void Player::Fade()
{
    if(flame_count < 20) {
        MV1SetRotationXYZ(model_handle_got_shocked, rot);
        MV1SetPosition(model_handle_got_shocked, VGet(pos.x, pos.y + 90.0f, pos.z));
        MV1DrawModel(model_handle_got_shocked);
    }
    else if(flame_count > 40) {
        MV1SetRotationXYZ(model_handle_got_shocked, rot);
        MV1SetPosition(model_handle_got_shocked, VGet(pos.x, pos.y + 90.0f, pos.z));
        MV1DrawModel(model_handle_got_shocked);
    }
}
bool Player::GetIsJump() const
{
    return is_jump;
}
void Player::Move()
{
    //角度変えます
    if(CheckHitKey(KEY_INPUT_A)) {
        rot.y += TO_RADIAN(PLAYER_ROT_SPEED);
    }
    if(CheckHitKey(KEY_INPUT_D)) {
        rot.y -= TO_RADIAN(PLAYER_ROT_SPEED);
    }
    is_run = false;   //最初は走ってない
    if(CheckHitKey(KEY_INPUT_RIGHT)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(右
        SetRight();
    }
    if(CheckHitKey(KEY_INPUT_LEFT)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(左
        SetLeft();
    }
    if(CheckHitKey(KEY_INPUT_UP)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(前
        SetFront();
    }
    if(CheckHitKey(KEY_INPUT_DOWN)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(後
        SetBack();
    }
    //プレイヤー移動位置の限界値
    PlayerPosLimit();
}
void Player::SetBeforeRun()
{
    ResetPlayerIsBoring();
    is_run = true;   //走っています
    SetAnimation(anim_handle, 1);
}
void Player::SetRight()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(90.0f);
    stand_by           = rot.y + TO_RADIAN(90.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED - slow_down) * sinf(rot.y + TO_RADIAN(270.0f));
    pos.z += (PLAYER_MOVE_SPEED - slow_down) * cosf(rot.y + TO_RADIAN(270.0f));
}
void Player::SetLeft()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(270.0f);
    stand_by           = rot.y + TO_RADIAN(270.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED - slow_down) * sinf(rot.y + TO_RADIAN(90.0f));
    pos.z += (PLAYER_MOVE_SPEED - slow_down) * cosf(rot.y + TO_RADIAN(90.0f));
}
void Player::SetFront()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(0.0f);
    stand_by           = rot.y + TO_RADIAN(0.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED - slow_down) * sinf(rot.y + TO_RADIAN(180.0f));
    pos.z += (PLAYER_MOVE_SPEED - slow_down) * cosf(rot.y + TO_RADIAN(180.0f));
}
void Player::SetBack()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(180.0f);
    stand_by           = rot.y + TO_RADIAN(180.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED - slow_down) * sinf(rot.y);
    pos.z += (PLAYER_MOVE_SPEED - slow_down) * cosf(rot.y);
}
//プレイヤーアニメーション演出
//---------------------------------------------------------
void Player::PlayAnimation()
{
    playtime += 0.5f;   //アニメーションを進める
    if(totaltime <= playtime) {
        playtime = 0.0f;
        if(anim_no == 0) {
            MV1DetachAnim(model_handle, attachindex);
            anim_no     = 1;
            attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
            totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
        }
    }
    MV1SetAttachAnimTime(model_handle, attachindex, playtime);   //再生時間をセットする
}
void Player::PlayAnimationInSwamp()
{
    playtime += 0.5f;

    if(totaltime_in_swamp <= playtime) {
        playtime = 0.0f;
        if(anim_no_in_swamp == 0) {
            MV1DetachAnim(model_handle_in_swamp, attachindex_in_swamp);
            anim_no_in_swamp = 1;
            attachindex_in_swamp =
                MV1AttachAnim(model_handle_in_swamp, 1, anim_handle_in_swamp[anim_no_in_swamp], FALSE);
            totaltime_in_swamp = MV1GetAttachAnimTotalTime(model_handle_in_swamp, attachindex_in_swamp);
        }
    }

    MV1SetAttachAnimTime(model_handle_in_swamp, attachindex_in_swamp, playtime);   //再生時間をセットする
}
void Player::PlayAnimationGotShocked()
{
    playtime += 0.5f;

    if(totaltime_got_shocked <= playtime) {
        playtime = 0.0f;
        MV1DetachAnim(model_handle_got_shocked, attachindex_got_shocked);
        attachindex_got_shocked = MV1AttachAnim(model_handle_got_shocked, 1, anim_handle_got_shocked[0], FALSE);
        totaltime_got_shocked   = MV1GetAttachAnimTotalTime(model_handle_got_shocked, attachindex_got_shocked);
    }

    MV1SetAttachAnimTime(model_handle_got_shocked, attachindex_got_shocked, playtime);   //再生時間をセットする
}
void Player::ResetPlayerPosition()
{
    if(player_is_boring) {
        player_model_rot.y = stand_by;   //角度は合わせて調整
        SetAnimation(anim_handle, 3);
    }
    else {
        player_model_rot.y = stand_by;   //角度は合わせて調整
        SetAnimation(anim_handle, 0);
    }
}
//プレイヤーのアップデート
//-----------------------------------
void Player::Update()
{
    //今の状態に合わせてプレイヤーのステータスを決まます
    if(hurted) {
        now_player = HURT;
    }
    else {
        now_player = COMMON;
    }
    if(in_swamp) {
        now_player = IN_SWAMP;
    }
    if(is_clear) {
        now_player = CLEAR;
    }

    switch(now_player) {
    case PLAYER_STATUS::HURT:
        // 演出処理に入ります
        GotShocked();
        break;
    case PLAYER_STATUS::COMMON:
        //アニメーションの処理をする
        PlayAnimation();
        JumpProcess(GetIsCollided());
        //プレイヤーの移動に関する関数
        Move();
        GravityLine();
        //走ってないなら
        if(!is_run)
            ResetPlayerPosition();
        //ルートマトリクスの回転と移動をなくす
        MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
        break;
    case PLAYER_STATUS::IN_SWAMP:

        feel = DAMAGE;
        damage_a_in_swamp -= 2;
        if(damage_a_in_swamp < 4) {
            damage_a_in_swamp = 126;
        }
        PlayAnimationInSwamp();
        JumpProcess(GetIsCollided());
        //プレイヤーの移動に関する関数
        Move();
        GravityLine();
        //走ってないなら
        if(!is_run)
            ResetPlayerPosition();
        // ルートマトリクスの回転と移動をなくす
        MV1SetFrameUserLocalMatrix(model_handle_in_swamp, root_node, identity);
        break;
    case PLAYER_STATUS::CLEAR:
        //クリア演出に入ります
        Clear();
        //エフェクトの位置更新
        player_ef.pos = pos;
        player_ef.Update();
        break;
    }

    // プレイヤーの頭上のfeeling-barーの設定
    switch(feel) {
    case STATUS_PATTERN::SLOW:
        feel_bar_pos   = pos;              //プレイヤーの位置と同じ
        feel_bar_pos.y = pos.y + 275.0f;   //プレイヤーの位置より高い
        feel_bar_rot   = rot;              //プレイヤーのロットと同じ
        SetIsGlad(false);
        break;
    case STATUS_PATTERN::DAMAGE:
        damage_a -= 2;
        if(damage_a < 4) {
            damage_a = 126;
        }
        SetIsGlad(false);
        break;
    case STATUS_PATTERN::GET:
        feel_bar_pos.x = pos.x;   //プレイヤーの位置と同じ
        feel_bar_pos.z = pos.z;   //プレイヤーの位置と同じ
        feel_bar_rot   = rot;     //プレイヤーのロットと同じ
        feel_count += 1;

        //1秒は上に
        if(feel_count < 30) {
            varity += 4.0f;
        }
        //1秒～2秒は下に
        if((feel_count > 30) && (feel_count < 60)) {
            varity -= 3.0f;
        }
        //2秒以後演出終わり
        if(feel_count > 60) {
            SetIsGlad(false);
            feel_count = 0;   //カウンターを0にします
            SetFeel(STATUS_PATTERN::NONE);
        }
        feel_bar_pos.y = pos.y + 275.0f + varity;   //プレイヤーのYが
        break;
    case STATUS_PATTERN::NONE:
        break;
    }
}
//描画処理用関数
//----------------------------------------------
void Player::Render()
{
    SetUseLighting(FALSE);   //ライティングOFF
    switch(now_player) {
    case PLAYER_STATUS::HURT:
        Fade();
        break;
    case PLAYER_STATUS::COMMON:
        // プレイヤーの描画
        MV1SetScale(model_handle, VGet(1.2f, 1.2f, 1.2f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(model_handle, player_model_rot);
        MV1SetPosition(model_handle, VGet(pos.x, pos.y + 70.0f, pos.z));
        MV1DrawModel(model_handle);
        player_ef.Draw();
        break;
    case PLAYER_STATUS::IN_SWAMP:
        MV1SetScale(model_handle_in_swamp, VGet(1.2f, 1.2f, 1.2f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(model_handle_in_swamp, player_model_rot);
        MV1SetPosition(model_handle_in_swamp, VGet(pos.x, pos.y + 70.0f, pos.z));
        MV1DrawModel(model_handle_in_swamp);
        break;
    case PLAYER_STATUS::CLEAR:
        // プレイヤーの描画
        MV1SetScale(model_handle, VGet(1.2f, 1.2f, 1.2f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(model_handle, player_model_rot);
        MV1SetPosition(model_handle, VGet(pos.x, pos.y + 70.0f, pos.z));
        MV1DrawModel(model_handle);
        //エフェクトを描画
        player_ef.Draw();
        break;
    }
    SetUseLighting(FALSE);   //ライティングON
    // feeling_barの描画
    switch(feel) {
    case STATUS_PATTERN::GET:
        SetUseLighting(TRUE);
        MV1SetScale(feel_bar_model[0], VGet(0.25f, 0.25f, 0.25f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(feel_bar_model[0], VGet(feel_bar_rot.x, feel_bar_rot.y, feel_bar_rot.z));
        MV1SetPosition(feel_bar_model[0], feel_bar_pos);
        MV1DrawModel(feel_bar_model[0]);
        SetUseLighting(FALSE);
        break;
    case STATUS_PATTERN::SLOW:
        MV1SetScale(feel_bar_model[1], VGet(0.25f, 0.25f, 0.25f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(feel_bar_model[1], VGet(feel_bar_rot.x, feel_bar_rot.y, feel_bar_rot.z));
        MV1SetPosition(feel_bar_model[1], feel_bar_pos);
        MV1DrawModel(feel_bar_model[1]);
        break;
    case STATUS_PATTERN::DAMAGE:
        // α値は
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, damage_a);   //半透明設定
        DrawGraph(0, 0, damage_graph, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);   //半透明の設定を戻っに戻す
        break;
    case STATUS_PATTERN::NONE:
        break;
    }
    SetUseLighting(TRUE);   //ライティングON
}
//消去処理
//---------------------------------------------------
void Player::Exit()
{
    MV1DeleteModel(model_handle);
    MV1DeleteModel(model_handle_in_swamp);
    MV1DeleteModel(model_handle_got_shocked);
}
void Player::GravityLine()
{
    // 重力ラインのアプデート
    SetGravityLineStart(pos);
    SetGravityLineEnd(pos);

    // 頭上ラインのアプデート
    SetTopLineStart(pos);
    SetTopLineEnd(pos);
}
