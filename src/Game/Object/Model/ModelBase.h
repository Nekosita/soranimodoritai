#include "../Base.h"
#pragma once

//! ModelBaseクラス
//===============================================
class ModelBase : public Base
{
protected:
    // メンバ変数
    //===============================================
    int   model;          //!<モデル用変数
    int   model_handle;   //!<モデルのデータを入れる用の変数
    float target_rot;     //!<目的地に向けた角度

    //アニメ関係の変数
    float playtime = 0.0f;
    int   anim_no;       //!<今のアニメ番号
    float totaltime;     //!<トータル放送時間
    int   attachindex;   //!<今のアニメ番号

    int    root_node = -1;   //!<モデルのルート用変数
    MATRIX identity;

    float attackradius;   //!<攻撃される半径の設定

public:
    //!コンストラクタ
    ModelBase();

    //!デストラクタ
    virtual ~ModelBase();

    //----------------------------------------------------------
    //!	@name	攻撃される半径の設定
    //----------------------------------------------------------
    //!@{
    void  SetAttackRadius(float now_radious);
    float GetAttackRadius() const;
    //!@}
};
