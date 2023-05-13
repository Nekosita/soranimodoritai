#include "ModelBase.h"
#include <vector>

class Player;

//! キャラクター
//===============================================
class RedGoblin : public ModelBase
{
private:
    // メンバ変数
    //===============================================
    static constexpr float GOBLIN_NORMAL_SPEED = 2.0f;    //!< ゴブリンの移動速度
    static constexpr float GOBLIN_DUSH_SPEED   = 6.0f;    //!< ゴブリンの走る速度
    static constexpr float GOBLIN_ROT_SPEED    = 1.0f;    //!< ゴブリンの回転する速度
    static constexpr float BULLET_SPEED        = 60.0f;   //!< 飛ばす弾丸スピード

    enum MOVE_PATTERN
    {
        STAY,
        MOVE,
        RUN,
        SHOOT,
    };

    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/RedGoblin/Anim_Attack1.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Neutral.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Walk.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Run.mv1"),
    };

    //----------------------------------------------------------
    //!	@name	モデル移動関係設定
    //----------------------------------------------------------
    //!@{

    MOVE_PATTERN goblin_action;       //!<ゴブリンの移動パターン
    int          move_count;          //!<固定の秒数の後で移動;
    float        fix_move_distance;   //!<固定で移動する距離;
    VECTOR       pos_before_move;     //!<逃げる前の位置
    bool         is_move;             //!<動いでいがどうが
    bool         is_live;             //!<生きるがどうかの確認

    //!@}

    //----------------------------------------------------------
    //!	@name	feeling_bar関係設定
    //----------------------------------------------------------
    //!@{
    int    feel_bar_model[2];
    VECTOR feel_bar_pos;
    VECTOR feel_bar_rot;
    //!@}

    //----------------------------------------------------------
    //!	@name	弾丸関係設定
    //----------------------------------------------------------
    //!@{

    float  count         = 0;
    float  bullet_radius = 80.0f;
    VECTOR bullet_pos;
    bool   is_shoot;
    float  bullet_rot;

    //!@}

    bool  is_attatched;   //!<逃げる接触
    float atari_radius;   //!<当たり半径

    //プレイヤーを宣言して
    const Player* player_ptr_inr = nullptr;

public:
    void SetPlayer(Player* p) { player_ptr_inr = p; }

    //! コンストラクタ
    RedGoblin(size_t num);
    //! デストラクタ
    virtual ~RedGoblin();

    //! @name  殺された時設定
    //!@{
    //! now_liveを設定します。
    //! @param  [in]    now_live  現在のnow_live
    void SetIsLive(bool now_live);

    //! is_liveを取得します。
    //! @return 現在のis_live
    bool GetIsLive() const;
    //!@}

    //! @name  ロット関係の設定
    //!@{
    //! rotを取得します。
    //! @return 現在のrot
    VECTOR GetRot() const;

    void ResetRot();
    //!@}

    //当たり半径設定
    void  SetAtariRadius(float atari_radious);
    float GetAtariRadius() const;

    //! @name   プレイヤと接触の判定
    //!@{
    //! now_を取得します。
    //! @return 現在のnow_
    void SetIsAttatched(bool now_);

    //! is_attatchedを取得します。
    //! @return 現在のis_attatched
    bool GetIsAttatched() const;
    //!@}

    //! @name   弾丸用
    //!@{
    void   SetBulletPos(const VECTOR& now_pos);
    VECTOR GetBulletPos() const;
    float  GetBulletRadius() const;
    void   SetIsShoot(bool is_shoot);
    bool   GetIsShoot() const;
    void   Reload();
    //!@}

    //! @name   Feeling_bar関係設定
    //!@{
    void FeelingBarInStay();
    void FeelingBarInMove();
    void FeelingBarInShoot();
    //!@}

    //! @name   アニメの設定
    //!@{
    //!アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    //!posからrotに向けで移動
    void  SetPosBeforeMove(VECTOR& pos, float rot);
    void  Fixpos(VECTOR& pos);
    float CalculateAngle(const VECTOR& V1, const VECTOR& V2);
    //!@}

    //!殺された時の座標設定
    void SetDiedPos();
    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
