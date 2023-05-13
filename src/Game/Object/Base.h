﻿#pragma once
#include "../src/Game/Main.h"

//! 全てのソースが共通する点を先にまとめます
//===============================================
class Base
{
protected:
    VECTOR pos;      //!<座標
    VECTOR rot;      //!<向き
    float  radius;   //!<半径

public:
    //!コンストラクタ
    Base();
    //!デストラクタ
    virtual ~Base();

    //! @name プレイヤーの位置設定
    //!@{
    void   SetPos(const VECTOR& now_pos);
    VECTOR GetPos() const;
    //!@}

    //! @name プレイヤーの当たり半径関係
    //!@{
    void  SetRadius(float now_radius);
    float GetRadius() const;
    //!@}

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
