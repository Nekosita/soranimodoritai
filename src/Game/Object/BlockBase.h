#include "Base.h"
#pragma once

//===============================================
//! ブロッククラス
//===============================================
class BlockBase : public Base
{
protected:
    static constexpr float STAGE_HIGHT = 150.0f;
    VECTOR                 start_pos;
    VECTOR                 end_pos;

    //!ブロックの上との当たり判定
    HITRESULT_LINE block_t1;
    HITRESULT_LINE block_t2;

    //!ブロックの下との当たり判定
    HITRESULT_LINE block_b1;
    HITRESULT_LINE block_b2;

public:
    BlockBase();
    virtual ~BlockBase();

    //!上層の床用
    bool CheckHitStageFloor() const { return (GetBlockT1().HitFlag == 1) || (GetBlockT2().HitFlag == 1); }

    //!下層の床用
    bool CheckHitStageBottom() const { return (GetBlockB1().HitFlag == 1) || (GetBlockB2().HitFlag == 1); }

    //!△の当たり判定用end_posを弄って最も上のposを探す
    float GetBlockHight() const;

    //!△の当たり判定用end_posを弄って最も上のposを探す
    VECTOR GetBlock1TopA() const;
    VECTOR GetBlock1TopB() const;
    VECTOR GetBlock1TopC() const;
    VECTOR GetBlock1TopD() const;

    //!△の当たり判定用end_posを弄って最も上のposを探す
    VECTOR GetBlock1BottomA() const;
    VECTOR GetBlock1BottomB() const;
    VECTOR GetBlock1BottomC() const;
    VECTOR GetBlock1BottomD() const;

    //!△の当たり判定の取得
    HITRESULT_LINE GetBlockT1() const;
    HITRESULT_LINE GetBlockT2() const;

    HITRESULT_LINE GetBlockB1() const;
    HITRESULT_LINE GetBlockB2() const;

    //!踏み台1のポス設定
    void SetBlockStartPos(const VECTOR& now_block_pos);
    void SetBlockEndPos(const VECTOR& now_block_endpos);
};
