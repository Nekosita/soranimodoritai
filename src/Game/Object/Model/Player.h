#pragma once
#include "ModelBase.h"
#include "../src/System/Effect.h"
#include <vector>
#include <array>
#include <string>

class Base;
class BlockBase;
class MoveBlock;
class Vector3;

//! Playerクラス
//===============================================
class Player : public ModelBase
{
private:
    // メンバ定数
    //===============================================
    static constexpr float JUMP_SPEED        = 135.0f;   //!<飛んでるスピード
    static constexpr float ADD_GRAVITY       = 5.0f;     //!<重力加速度設定
    static constexpr float PLAYER_ROT_SPEED  = 1.5f;     //!<プレイヤーの回転スピード設定
    static constexpr float PLAYER_MOVE_SPEED = 30.0f;    //!<プレイヤーの移動速度設定

    // メンバ変数
    //===============================================
    //!フィーリングバーの状態設定
    enum STATUS_PATTERN
    {
        SLOW,
        DAMAGE,
        GET,
        NONE,
    };

    //!プレイヤーの状態設定
    enum PLAYER_STATUS
    {
        COMMON,
        HURT,
        CLEAR,
        IN_SWAMP,
    };

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/mouse/Neutral.mv1"),
        MV1LoadModel("Data/Character/mouse/Running.mv1"),
        MV1LoadModel("Data/Character/mouse/Jumping.mv1"),
        MV1LoadModel("Data/Character/mouse/Salsa Dancing.mv1"),
    };

    //!キャラクターが毒沼にいる時のモデル設定
    std::vector<int> anim_handle_in_swamp{
        MV1LoadModel("Data/Character/mouse_in_swamp/Neutral.mv1"),
        MV1LoadModel("Data/Character/mouse_in_swamp/Running.mv1"),
        MV1LoadModel("Data/Character/mouse_in_swamp/Jumping.mv1"),
        MV1LoadModel("Data/Character/mouse_in_swamp/Salsa Dancing.mv1"),
    };

    std::vector<int> anim_handle_got_shocked{
        MV1LoadModel("Data/Character/mouse_got_shocked/Female Dance Pose.mv1"),
    };

    int vertex_shader;   //!< 頂点シェーダー
    int pixel_shader;    //!< ピクセルシェーダー
    float player_radius;   //!<プレイヤーの当たり半径

    //----------------------------------------------------------
    //!	@name	毒沼にいる時
    //----------------------------------------------------------
    //!@{
   
    int   anim_no_in_swamp;        //!<毒沼に居る時のアニメ番号
    int   model_handle_in_swamp;   //!<毒沼に居る時のモデル
    float totaltime_in_swamp;      //!<毒沼に居る時のトータル放送時間
    int   attachindex_in_swamp;    //!<毒沼に居る時の今のアニメ番号
    int   damage_a_in_swamp;       //!<毒沼に居る時の今のα値

    //!@}

    //----------------------------------------------------------
    //!	@name	電撃にされた時
    //----------------------------------------------------------
    //!@{

    int   anim_no_got_shocked;        //!<毒沼に居る時のアニメ番号
    int   model_handle_got_shocked;   //!<電撃にされた時のモデル
    float totaltime_got_shocked;      //!<電撃にされた時のトータル放送時間
    int   attachindex_got_shocked;    //!<電撃にされた時の今のアニメ番号

    //!@}

    //----------------------------------------------------------
    //!	@name	JUMP用設定
    //----------------------------------------------------------
    //!@{

    bool  is_jump;     //!<JUMPするがどうが確認の変数
    bool  on_ground;   //!<今は地面に接触しでいるがどうか
    bool  now_fall;    //!<落下
    float mov_y;       //!<JUMP距離用の変数
    float gravity;     //!<重力
    float slow_down;   //!<速度を落とす
    bool  is_run;      //!<走り用の変数
    float stand_by;    //!<待機用のロット

    //!@}

    //----------------------------------------------------------
    //!	@name	ブロックとの当たり用設定
    //----------------------------------------------------------
    //!@{

    bool is_stepped;    //!<踏み台の上に踏んでいるのか
    bool is_collided;   //!<踏み台の下にぶつかって行くがどうか
    bool game_over;

    //!@}

    //----------------------------------------------------------
    //!	@name	ブロックと当たり判定用ライン設定
    //----------------------------------------------------------
    //!@{
    VECTOR gravity_line_start;   //!<プレイヤーの重力ラインの上
    VECTOR gravity_line_end;     //!<プレイヤーの重力ラインの下
    VECTOR top_line_start;       //!<プレイヤーの頭上ラインの下
    VECTOR top_line_end;         //!<プレイヤーの頭上ラインの上

    //!@}

    //----------------------------------------------------------
    //!	@name	プレイヤーがダメージを貰った時の設定
    //----------------------------------------------------------
    //!@{
   
    PLAYER_STATUS now_player;              //!<プレイヤーのステータス
    VECTOR        knocked_off_variation;   //!<ぶつけられた時の変数
    bool          hurted;                  //!<今までのプレイヤーの状態
    int           damage_a;                //!<赤いエリアに居る時のα値
    int           move_count;              //!<移動用のカウンター
    int           flame_count;             //!<点滅カウンター
    int           jump_count;              //!<ジャンプカウンター
    int           damage_graph;            //!<プレイヤーダメージエフェクトの画像
    bool          in_swamp;                //!<もしもプレーヤーが沼の中に居たら

    //プレイヤーのHP関係変数
    float life_x;   //!<プレイヤーの初期LIFE＿POINT

    //!@}

    //----------------------------------------------------------
    //!	@name	feeling_bar関係設定
    //----------------------------------------------------------
    //!@{
    
    int    feel;
    int    feel_bar_model[2];
    VECTOR feel_bar_pos;
    VECTOR feel_bar_rot;
    int    feel_count;
    float  varity;    //!<移動用の変数
    bool   is_glad;   //!<嬉しいがどうがを判定する変数

    //!@}

    //----------------------------------------------------------
    //!	@name	待機用モデル用設定
    //----------------------------------------------------------
    //!@{

    bool player_is_boring;
    int  boring_count;
    bool jumping_to_camera;   //!<カメラあげる用

    //!@}

    Effect player_ef;   //!<プレイヤー用エフェクト設定

    //ブロックーを宣言して
    bool is_clear;          //!<プレイヤーがゲームクリアしているがどうか
    bool to_hint_bar;       //!<ヒントバーにあげる関数
    bool is_goal_actived;   //!<ゴールバーの上に立っているがいないが
    bool on_move_block;

    VECTOR player_model_rot;   //!<プレイヤーのモデル用の角度
    float  PlayTime_start;

public:

    //! コンストラクタ
    Player();
    //! デストラクタ
    ~Player();

      //! @name  アニメの設定
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    //アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    void PlayAnimationInSwamp();
    void PlayAnimationGotShocked();
    //!@}
    
      //! @name  プレイヤー重力ラーイン関係設定
    //!@{
    void   SetGravityLineStart(VECTOR& now_gravity_start);
    VECTOR GetGravityLineStart() const;
    void   SetGravityLineEnd(VECTOR& now_gravity_end);
    VECTOR GetGravityLineEnd() const;
    //!@}

    //! @name  プレイヤー重力ラーイン関係設定
    //!@{
    
    void   SetTopLineStart(VECTOR& now_top_start);
    VECTOR GetTopLineStart() const;
    void   SetTopLineEnd(VECTOR& now_top_end);
    VECTOR GetTopLineEnd() const;

      //!@}

    void GravityLine();   //上下の当たりラインの処理

    //! @name   プレイヤーHP取得関数設定
    //!@{

    //! HPを設定します。
    //! @param  [in]    now_Hp  現在のHP
    void SetHp(float now_Hp);

    //! HPを取得します。
    //! @return 現在のHP
    
    float GetHp() const;
    float GetLifeX() const;

    //!@}

    //! @name   感情関係設定
    //!@{
    //! feelを設定します。
    //! @param  [in]    now_feel  現在のfeel
    
    void SetFeel(int now_feel);

    //! feelを取得します。
    //! @return 現在のfeel
    
    int GetFeel() const;

    //!@}

    //! @name   ダメージ関係設定
    //!@{

    //! now_hurtedを設定します。
    //! @param  [in]    now_hurted  現在のnow_hurted
    void SetHurted(bool now_hurted);

    //! hurtedを取得します。
    //! @return 現在のhurted
    bool GetHurted() const;

    //!@}

    //! @name   HPの増加と減るの設定
    //!@{
    //!HP減る
    void HpDecrese();
    //!プレイヤーが弾丸に打たれた時の演出
    void BulletDamage();
    //!毒沼にいる時の確認
    void SetInSwamp(bool Now_in_swamp);
    //!毒沼にいる時のダメージ
    void HpDamageInSwamp();
    //!電撃ボールにぶつかった時の処理
    void GotShocked();
    //!@}

    //! @name   Jump処理
    //!@{
    //!全ての飛ぶ処理のまとめ
    void JumpProcess(bool now_is_collided);
    //!飛ぶ事ができるがを判断する関数
    void IsJump();
    void JumpReset();
    //!Jump処理_地面
    void JumpResetGround();
    //!頭がぶつかったときの動作
    void JumpHit();
    //!ブロック上のジャンプ
    void JumpOnTheBlock();
    //!地面からのジャンプ
    void JumpOnTheGround();
    //!ジャンプの時のプレイヤーの重力(抵抗力)処理
    void OnResistance();
    void GravityInJump();
    //!下から上にぶつかった時の高さ設定
    void SetTopWhenHit(float block_hight);
    //!上から下に落ちた時の下の設定
    void SetDownWhenHit(float block_hight);
    //!上昇中か
    bool IsJumpUp();
    //!@}

    //! @name  プレイヤーのYの位置再設定
    //!@{
    void SetPosY(float fix_y);
    void SetPosOnMoveBlock(const Vector3& dir);
    void SetPosWithBall(const VECTOR& now_pos, const Vector3& dir);
    void PlayerPosLimit();

    //!@}

    //! @name  ロット関係の設定
    //!@{
    //! now_rotを設定します。
    //! @param  [in]    now_rot  現在のnow_rot
    void   SetRot(const VECTOR& now_rot);
    //! rotを取得します。
    //! @return 現在のrot
    VECTOR GetRot() const;
    //! player_model_rotを取得します。
    //! @return 現在のplayer_model_rot
    VECTOR GetPlayerModelRot() const;
    //!@}

    
    //! @name  楽しい設定
    //!@{
    //! now_is_gladを設定します。
    //! @param  [in]    now_is_glad  現在のnow_is_glad
    void SetIsGlad(bool now_is_glad);
    //! is_gladを取得します。
    //! @return 現在のis_glad
    bool GetIsGlad() const;
    //!@}

    //! @name 踏み台の上に踏んでいるのかの取得関数
    //!@{
    //! now_is_steppedを設定します。
    //! @param  [in]    now_is_stepped  現在のnow_is_stepped
    void SetIsStepped(bool now_is_stepped);
    //! is_steppedを取得します。
    //! @return 現在のis_stepped
    bool GetIsStepped() const;
    //!@}

    //! @name 踏み台の下のぶつかっているのかの取得関数
    //!@{
    //! now_is_collidedを設定します。
    //! @param  [in]    now_is_collided  現在のnow_is_collided
    void SetIsCollided(bool now_is_collided);
    //! is_collidedを取得します。
    //! @return 現在のis_collided
    bool GetIsCollided() const;
    //!@}

    //! @name プレイヤーとゴールブロックの当たり判定
    //!@{
    void ResetBeforeCheck();
    bool CheckHitGoalBar(bool is_on_goal, int ckeck_num, int total_num, float hight);
    bool GetGoalActived() const;
    void SetToHintBar(bool now);
    bool GetToHintBar() const;
    void SetIsClear(bool now_is_clear);
    bool GetIsClear() const;
    //!@}

    
    //! @name その他の設定
    //!@{

    //!プレイヤーとステージブロックの当たり判定
    bool HitWithStageBlock(const BlockBase* block);
    //!プレイヤーとスムーヴブロックの当たり判定
    bool HitWithMoveBlock(const MoveBlock* block);
    bool GetOnMoveBlock() const;
    //!点滅演出
    void Fade();
    //!飛ぶ処理用
    bool GetIsJump() const;

      
    //!待機用模型のリセット
    void ResetPlayerIsBoring();
    //!プレイヤーの待機モーションを再生します
    void NowPlayerIsBoring();



    //!速度を落とす
    void SetSlowDown(float now_slow_down);

    //!Game_clearの後
    void Clear();

    //!走る用の関数
    void Move();
    //!プレイヤーが走る処理を入る前の設定
    void SetBeforeRun();
    //!右
    void SetRight();
    //!左
    void SetLeft();
    //!前
    void SetFront();
    //!後
    void SetBack();
    //!移動中でもない場合は角度に合わせてプレイヤーの角度をセット
    void ResetPlayerPosition();
    void HitTrapBall(int num, const Base& ball);
     //!プレイヤーを取得
    static Player* GetPlayer();

     //!@}

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();


};
