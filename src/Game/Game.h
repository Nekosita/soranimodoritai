#pragma once
#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//!全てのゲーム画面
enum class GAME_SCREEN
{
    GAME_TITLE,
    GAME_RULE,
    GAME_NOWLOADING,
    GAME_PAUSE,
    GAME_START_ANIME,
    GAME_STAGE,
    GAME_OVER,
    GAME_CLEAR_ANIME,
    GAME_CLEAR,
    GAME_RANK,
    GAME_PARFECT_CLEAR,
};

//!クラスの前方宣言
class RedGoblin;
class Base;
class Camera;
class Player;
class Model;
class MapChip;
class Ranking;
class UserInterface;
class SoundEffect;
class GameoverArea;
class GoalArea;
class StageBlock;
class TrapBall;
class TrapArea;
class SlowArea;
class Item;
class MoveBlock;
class Stage1;

//! Gameクラス
//===============================================
class Game
{
private:
    // メンバ定数
    //===============================================
    static constexpr int RED_NUM        = 3;
    static constexpr int BLOCK_NUM      = 33;
    static constexpr int MOVE_BLOCK_NUM = 3;
    static constexpr int TRAP_BALL      = 4;
    static constexpr int TRAP_AREA      = 5;
    static constexpr int SLOW_AREA      = 8;
    static constexpr int TOTAL_ITEM     = 10;
    static constexpr int RANKING_NUM    = 5;

    GAME_SCREEN    current_screen;            //!<今ゲーム画面はどこにいたのかの設定
    Camera*        camera        = nullptr;   //!<カメラ
    MapChip*       mapchip       = nullptr;   //!<マップ設定
    Player*        player        = nullptr;   //!<playerの設定
    Model*         model         = nullptr;   //!<model実装
    Ranking*       ranking       = nullptr;   //!<ランキングの処理
    UserInterface* ui            = nullptr;   //!<UIを設定
    SoundEffect*   sound         = nullptr;   //!<音関係
    GameoverArea*  gameover_area = nullptr;   //!<最下層の毒沼実装
    GoalArea*      goal_area     = nullptr;   //!<ゴールドエリアー実装
    Stage1*        stage1        = nullptr;   //!<ステージ1実装

    std::vector<std::shared_ptr<RedGoblin>>  redgoblin;    //!<レードゴブリンの設定
    std::vector<std::shared_ptr<StageBlock>> block_1;      //!<ステージ1の踏み台のブロック実装
    std::vector<std::shared_ptr<TrapBall>>   trap_ball;    //!<トラップボール実装
    std::vector<std::shared_ptr<TrapArea>>   trap_area;    //!<トラップエリア実装
    std::vector<std::shared_ptr<SlowArea>>   slow_area;    //!<スローエリア実装
    std::vector<std::shared_ptr<Item>>       item;         //!<拾うアイテム実装
    std::vector<std::shared_ptr<MoveBlock>>  move_block;   //!<移動するブロック実装

    //----------------------------------------------------------
    //!	@name	ゲームクリア演出用
    //----------------------------------------------------------
    //!@{
    bool goal_actived;   //!<ゴールが起動するがしないか
    bool show_button;    //!<設定出すが出さないが用
    bool music_on;       //!<音楽切り替え用
    //!@}

    // !当たり判定用関数
    bool CheckHitWithBullet(const Base* obj1, const RedGoblin* obj2);

    //! @name その他の設定
    //!@{
    //	!設定見る用の関数
    bool        IsPause();
    bool        GetMusicOn();
    GAME_SCREEN UpdatePause();
    //!@}

    //　!ステージのBGMを再生する関数
    void PlayStageBGM();
    //	!被った演出用のブロック、罠エリア、速度ダーウィンエリア描画
    void Show_Render();

public:
    //	!初期化処理
    void Init();
    //	!更新処理
    void Update();
    //	!描画処理
    void Render(bool shadow_);
    //	!終了処理
    void Exit();
};
