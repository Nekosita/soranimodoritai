#include "Model.h"
#include "../Base.h"
#include "DxLib.h"
#include <string>

#pragma once
//コンストラクタ
//-----------------------------------------------
Model::Model()
{
    // モデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダからセットします
    std::string strM = std::string(str) + "/Data/Character/mouse/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Neutral Idle.mv1");

    // Gameフォルダに戻します
    SetCurrentDirectory(str);
    attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);

    pos = {5.0f * 200.0f, 19000.0f, 5.0f * 200.0f};   // オープニングのモデルの生成位置

    root_node = MV1SearchFrame(model_handle, "mixamorig1:Spine");
    root_node = 1;   // 描画用の関数設定

    CreateIdentityMatrix(&identity);
    scene_num = 4;
    playtime  = 0.0f;   // 再生時間の初期化
}
//デストラクタ
//---------------------------------
Model::~Model()
{
    Exit();
}
//アニメーション設定
//---------------------
void Model::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    if(anim_no != new_anim_no) {
        MV1DetachAnim(model_handle, attachindex);
        anim_no     = new_anim_no;
        attachindex = MV1AttachAnim(model_handle, 1, handle[anim_no], FALSE);
        totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    }
}
void Model::PlayAnimation()
{
    playtime += 0.5f;   //描画時間を更新

    if(totaltime <= playtime) {
        playtime = 0.0f;
        if(anim_no == 0) {
            MV1DetachAnim(model_handle, attachindex);
            anim_no     = 1;
            attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
            totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
        }
    }
}
void Model::SetNowScene(int now_scene_num)
{
    scene_num = now_scene_num;
}
int Model::GetNowScene() const
{
    return scene_num;
}
//アップデート
//---------------------
void Model::Update()
{
    switch(scene_num) {
    case Model_PATTERN::OPENING_ANIME:
        break;
    case Model_PATTERN::FALLLING:
        //描画用のアニメの設定
        SetAnimation(anim_handle, 2);
        pos.y -= 20.0f;
        break;
    case Model_PATTERN::GAME_CLEAR:
        SetAnimation(anim_handle, 1);
        pos = {0.0f, SCREEN_H / 2, 0.0f};
        break;
    case Model_PATTERN::GAME_OVER:
        SetAnimation(anim_handle, 0);
        pos = {0.0f, SCREEN_H / 2, 0.0f};
        break;
    case Model_PATTERN::GAME_PARFECT_CLEAR:
        SetAnimation(anim_handle, 3);
        pos = {0.0f, SCREEN_H / 2, 0.0f};
        break;
    case Model_PATTERN::NOTHING:
        break;
    }

    PlayAnimation();
    MV1SetAttachAnimTime(model_handle, attachindex, playtime);   //再生時間をセットする
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//モデルの描画
//--------------------
void Model::Render()
{
    MV1SetScale(model_handle, VGet(2.0f, 2.0f, 2.0f));   //モデルの大きさを変えます
    MV1SetRotationXYZ(model_handle, player_model_rot);
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
}
//消去処理
//-----------------
void Model::Exit()
{
    MV1DeleteModel(model_handle);
}
