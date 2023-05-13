#pragma once
#include "ModelBase.h"
#include <vector>

//! Modelクラス
//===============================================
class Model : public ModelBase
{
private:
    // メンバ定数
    //===============================================
    static constexpr float JUMP_SPEED        = 135.0f;   //!<　飛ぶ時の速度
    static constexpr float ADD_GRAVITY       = 5.0f;     //!<  加速度
    static constexpr float PLAYER_ROT_SPEED  = 2.0f;     //!<　回転する速度　
    static constexpr float PLAYER_MOVE_SPEED = 35.0f;    //!<　移動速度

    // メンバ変数
    //===============================================

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/mouse/Falling Into Pool.mv1"),
        MV1LoadModel("Data/Character/mouse/Joyful Jump.mv1"),
        MV1LoadModel("Data/Character/mouse/Falling.mv1"),
        MV1LoadModel("Data/Character/mouse/Salsa Dancing.mv1"),
    };

    //!カメラ視点の設定
    enum Model_PATTERN
    {
        OPENING_ANIME,
        FALLLING,
        GAME_CLEAR,
        GAME_OVER,
        GAME_PARFECT_CLEAR,
        NOTHING,
    };

    Model_PATTERN model;   //!<使ってるモデル番号
    float play_time_start;   //!<動画の開始時間
    VECTOR player_model_rot;   //!<プレイヤーのモデル用の角度
    int    scene_num;          //!<今上演している番号

public:

    //!コンストラクタ
    Model();
    //!デストラクタ
    ~Model();

    //----------------------------------------------------------
    //!	@name	アニメの設定
    //----------------------------------------------------------
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    void PlayAnimation();
    //!@}
      
    //----------------------------------------------------------
    //!	@name	上演シーン設定
    //----------------------------------------------------------
    //!@{
    void SetNowScene(int now_scene);
    int  GetNowScene() const;
    //!@}

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
