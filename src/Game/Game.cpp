#include "Game.h"
#include "Main.h"
#include "../src/Game/Object/Model/Model.h"
#include "../src/Game/Object/MoveBlock.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Camera.h"
#include "../src/Game/Object/MapChip.h"
#include "../src/Game/Object/Model/RedGoblin.h"
#include "../src/Game/Object/Model/TrapBall.h"
#include "../src/Game/Object/Stage/TrapArea/TrapArea.h"
#include "../src/Game/Object/Stage/SlowArea/SlowArea.h"
#include "../src/Game/Object/GameoverArea.h"
#include "../src/Game/Object/Model/Item.h"
#include "../src/Game/Object/Stage/StageBlock/StageBlock.h"
#include "../src/Game/Object/SoundEffect.h"
#include "../src/Game/Object/GoalArea.h"
#include "../src/Game/Object/Ranking.h"
#include "DxLib.h"
#include "../src/System/KeyInput.h"
#include "../src/System/Effect.h"
#include "../src/System/Vector3.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/UserInterface.h"
#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <sstream>
// Scene関連
#include "../src/Game/Scene/SceneTitle.h"
#include "../src/Game/Scene/SceneRule.h"
#include "../src/Game/Scene/SceneNowLoading.h"
#include "../src/Game/Scene/SceneStage.h"

//当たり判定用の関数
// ==================================================
bool Game::CheckHitWithBullet(const Base* obj1, const RedGoblin* obj2)
{
    return AttachCheck2D(obj1->GetPos(), 25.0f, obj2->GetBulletPos(), obj2->GetBulletRadius());
}
// ==================================================
//設定見る用の関数
// ==================================================
bool Game::IsPause()
{
    //もしもEnterキーを押したらset画面が出ます
    if(IsKeyOn(KEY_INPUT_RETURN))
        return show_button ? false : true;
    //もしもボタンが押されていない場合は、現在のset_buttonの値が返る
    return show_button;
}
bool Game::GetMusicOn()
{
    if(IsKeyOn(KEY_INPUT_1))
        return music_on ? false : true;
    //もしもボタンが押されていない場合は、現在のmusic_onの値が返る
    return music_on;
}
GAME_SCREEN Game::UpdatePause()
{
    show_button = IsPause();
    //セットボタン押したらGAME_PAUSE画面に入ります
    if(show_button) {
        return GAME_SCREEN::GAME_PAUSE;
    }
    return GAME_SCREEN::GAME_STAGE;
}
// ==================================================
//ステージのBGMを再生する関数
// ==================================================
void Game::PlayStageBGM()
{
    if(CheckMusic() == 0) {
        // 音楽が再生していない状態
        if(music_on) {
            PlayMusic("Data/music/魔王魂  ファンタジー09.mp3", DX_PLAYTYPE_LOOP);
        }
        return;
    }
    if(!music_on) {
        sound->ResetVolume();
        StopMusic();   //音楽再生の停止
    }
}
// --------------------------------------------------
//被った演出用のブロック、罠エリア、速度ダーウィンエリア描画
// --------------------------------------------------
void Game::Show_Render()
{
    for(size_t i = 0; i < block_1.size(); i++) {
        block_1[i]->Render();
    }
    for(size_t i = 0; i < move_block.size(); i++) {
        move_block[i]->Render();
    }
    for(size_t i = 0; i < trap_area.size(); i++) {
        trap_area[i]->Render();   //トラップエリア実装
    }
    for(size_t i = 0; i < slow_area.size(); i++) {
        slow_area[i]->Render();   //スローエリア実装
    }
}
//----------------------------
//初期化処理
//----------------------------
void Game::Init()
{
    Exit();
    // --------------------------------------------------
    // 全体で使用している変数・オブジェクトの初期化
    // --------------------------------------------------
    current_screen = GAME_SCREEN::GAME_TITLE;   //初期はタイトル
    model          = new Model();               //model実装
    player         = new Player();              //プレイヤーとカメラと打つ弾丸を実装

    camera = new Camera;   //カメラ実装
    camera->SetPlayer(player);   //カメラクラスのオブジェクトに、Playerクラスのオブジェクトのアドレスを渡す
    sound = new SoundEffect();

    //ブロックの設定
    for(size_t i = 0; i < BLOCK_NUM; i++) {
        auto p = std::make_shared<StageBlock>(i);
        p->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        block_1.push_back(std::move(p));
    }
    //ブロックの設定
    for(size_t i = 0; i < MOVE_BLOCK_NUM; i++) {
        auto m = std::make_shared<MoveBlock>(i);
        m->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        move_block.push_back(std::move(m));
    }
    //レッドゴブリンの数設定します
    for(size_t i = 0; i < RED_NUM; i++) {
        auto g = std::make_shared<RedGoblin>(i);
        g->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        redgoblin.push_back(std::move(g));
    }
    //トラップボール設定
    for(size_t i = 0; i < TRAP_BALL; i++) {
        auto t = std::make_shared<TrapBall>(i);
        t->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        trap_ball.push_back(std::move(t));
    }
    //トラップArea設定
    for(size_t i = 0; i < TRAP_AREA; i++) {
        auto t = std::make_shared<TrapArea>(i);
        t->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        trap_area.push_back(std::move(t));
    }
    //スローエリアを設定します
    for(size_t i = 0; i < SLOW_AREA; i++) {
        auto s = std::make_shared<SlowArea>(i);
        s->SetPlayer(player);   //	Playerクラスのオブジェクトのアドレスを渡す
        slow_area.push_back(std::move(s));
    }

    //拾うアイテム実装
    for(size_t i = 0; i < TOTAL_ITEM ; i++) {
        auto it = std::make_shared<Item>(i);
        item.push_back(std::move(it));
        //item[i] = new Item(i);
    }

    //UI設定
    ui = new UserInterface;
    ui->Init();
    ui->SetPlayer(player);
    for(size_t i = 0; i < item.size(); i++) {
        ui->SetItem(item[i].get(), int(i));
    }

    ui->SetItemNum((int)(item.size()));

    //ランクイング実装
    ranking = new Ranking;
    ranking->SetPlayer(player);

    //ゲームオーヴァーエリア実装
    gameover_area = new GameoverArea;
    gameover_area->SetPlayer(player);

    //ゴールエリア実装
    goal_area = new GoalArea();
    goal_area->SetPlayer(player);   //ゴールのオブジェクトに、Playerクラスのオブジェクトのアドレスを渡す

    //マっプの設定
    mapchip = new MapChip;
    mapchip->BuildMap();

    //ステージ1にプレイヤー設定
    //stage1 = new Stage1;
    //stage1->SetPlayer(player);
    //stage1->Init();

    GameStartAnime();
    GameStageInit();
    InitGameTitle();

    //ランクファイルの読み込み
    ranking->ReadNormalRankingFile();
    ranking->ReadParfectRankingFile();
    ranking->InitGameClearAnime();
    ranking->GameRankInit();

    InitGameRule();
    sound->InitEffectSound();
    //セットポタン
    music_on    = true;
    show_button = false;
}
//更新処理
//-----------------------------------
void Game::Update()
{
    switch(current_screen) {
    case GAME_SCREEN::GAME_TITLE:
        camera->SetNowCamera(1);
        camera->Update(player->IsJumpUp());
        //トラップボールのアップデート
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Update();
        }
        //タイトル画面の更新処理
        current_screen = UpdateGameTitle();
        break;
    case GAME_SCREEN::GAME_RULE:
        current_screen = UpdateGameRule();
        break;
    case GAME_SCREEN::GAME_NOWLOADING:
        Init();
        current_screen = UpdateGameNowLoading();
        break;
    case GAME_SCREEN::GAME_START_ANIME:
        //スタート演出、終わったらゲームに入ります
        model->SetNowScene(1);
        model->Update();
        camera->SetNowCamera(0);   //使うカメラモード設定
        camera->Update(player->IsJumpUp());
        
        //画像を入れなさい
        if((camera->GetOpenAnimeCount() > 180)) {
            current_screen = GAME_SCREEN::GAME_STAGE;
        }

        if(IsKeyOn(KEY_INPUT_RETURN)) {
            current_screen = GAME_SCREEN::GAME_STAGE;
        }
     
        break;
    case GAME_SCREEN::GAME_STAGE:

        camera->SetNowCamera(2);   //第3視点でプレイヤーを見ています
        //ゲームクリアやゲームセット画面に入ったらのゲームクリアとゲームセットの画面処理を優先します
        PlayStageBGM();                   //BGMを流す
        current_screen = UpdatePause();   //Pause処理

        //クリアバーの画面処理を優先します
        if(player->GetIsClear()) {
            current_screen = GAME_SCREEN::GAME_CLEAR_ANIME;
        }

        //クリアではない場合は普通にアプデします
        ui->ItemCompleteHint();

        goal_area->Update();

        //クリアバーの処理を優先します
        player->CheckHitGoalBar(goal_area->CheckHitStageFloor(),
                                ui->Getnum(),
                                (int)item.size(),
                                goal_area->GetGoalAreaHight());
        ui->SetHint(player->GetToHintBar());   //ヒントテキストが出るが出ないが確認

        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Move();
        }

        //プレイヤーがゴールエリアの上に立ってない
        if(!player->GetGoalActived()) {
            player->ResetBeforeCheck();   //判定が被らないように判定前に先に上と下の判定を消します
            for(size_t i = 0; i < move_block.size(); i++) {
                move_block[i]->Update();
                if(player->HitWithMoveBlock(move_block[i].get())) {
                    break;
                }
            }
            //ムーヴブロックとゴールバーの上に立ってないならこちの処理に入ります
            if(!player->GetOnMoveBlock()) {
                //ブロック普通のブロックの上に立つ判定が発動する
                player->ResetBeforeCheck();   //判定が被らないように判定前に先に上と下の判定を消します
                for(size_t i = 0; i < block_1.size(); i++) {
                    player->HitWithStageBlock(block_1[i].get());
                    block_1[i]->Update();
                }
            }
        }

        gameover_area->Update();   //ゲームオーバーエリアの処理

        if(gameover_area->CheckHitStageFloor()) {
            player->HpDamageInSwamp();   //毒沼にいる時特別用
        }
        else {
            player->SetInSwamp(false);   //基本実行しない
        }

        //喜んでいない時何も感じない
        if(!player->GetIsGlad()) {
            player->SetFeel(3);   //感情はノーマル状態
        }

        //拾うの当たり判定
        for(size_t i = 0; i < item.size(); i++) {
            if(!CheckHitStageObject(item[i].get(), player))
                continue;
            sound->PlayGetSound();
            ui->AddNum();
            player->SetFeel(2);   //とても喜んでいます
            item[i]->SetIsPicked(true);
            VECTOR pick_pos = {0.0f, -2000.0f, 0.0f};
            item[i]->SetPos(pick_pos);
            player->SetIsGlad(true);   //ノーマル状態に入らないようにtrueに入ります
        }

        //トラップArea設定
        for(size_t i = 0; i < trap_area.size(); i++) {
            trap_area[i]->Update();
            if(!trap_area[i]->CheckHitStageFloor())
                continue;
            player->SetFeel(1);    //痛い状態
            player->HpDecrese();   //ダメージ設定
        }

        //スローエリアの設定
        player->SetSlowDown(0.0f);   //普段は起動しません
        for(size_t i = 0; i < slow_area.size(); i++) {
            slow_area[i]->Update();
            if(!slow_area[i]->CheckHitStageFloor())
                continue;
            player->SetFeel(0);   //緩める状態
            player->SetSlowDown(25.0f);
        }

        //拾うアイテムアプデート
        for(size_t i = 0; i < item.size(); i++) {
            if(item[i]->GetIsPicked())
                continue;
            item[i]->Update();
        }

        //player->Update();

        //RedGoblin関係判定
        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!redgoblin[i])
                continue;
            redgoblin[i]->Update();
        }

        //playerとRedgoblinの追撃判定
        for(size_t i = 0; i < redgoblin.size(); i++) {
            //この距離から攻撃モードに入ります
            if(CheckHitWithEnemy(player, redgoblin[i].get())) {
                redgoblin[i]->SetIsAttatched(true);   //SHOOTモードに入ります
                if(!redgoblin[i]->GetIsShoot()) {
                    redgoblin[i]->SetBulletPos(redgoblin[i]->GetPos());
                    redgoblin[i]->SetIsShoot(true);
                }
            }
            else {
                redgoblin[i]->SetIsAttatched(false);   //SHOOTモードから離れます
                redgoblin[i]->SetIsShoot(false);
            }
        }

        //弾丸と当たった時の判定
        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!CheckHitWithBullet(player, redgoblin[i].get()))
                continue;
            player->BulletDamage();
            redgoblin[i]->SetIsShoot(false);
        }

        //トラップボールのアップデート
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Update();
        }

        //ボールとプレイヤーの当たり判定
        for(size_t i = 0; i < trap_ball.size(); i++) {
            if(!IsHitBall(trap_ball[i].get(), player))
                continue;
            int hit_ball_num = trap_ball[i]->SetKnockedOffVar();
            if(hit_ball_num != -1) {
                player->HitTrapBall(hit_ball_num, *trap_ball[i]);
            }
        }

        player->Update();

        //残り時間のカウンター
        ui->TimeCountFlame();
        ui->ItemCompleteHint();

        //時間切れとプレイヤーHPが0になったらゲームオーヴァー
        if((ui->GetTimeCount() < 0) || (player->GetHp() < 0)) {
            current_screen = GAME_SCREEN::GAME_OVER;
        }

        //mapchip->Update();                    //沼の流れ演出

        camera->Update(player->IsJumpUp());   //カメラの更新

        //ステージ1の更新
        //stage1->Update();

        break;
    case GAME_SCREEN::GAME_PAUSE:
        PlayStageBGM();
        // Pause処理
        current_screen = UpdatePause();
        music_on       = GetMusicOn();
        //音量調整
        SetVolumeMusic(sound->SetVolume());
        sound->Update();
        break;
    case GAME_SCREEN::GAME_CLEAR_ANIME:
        //GAME_CLEAR_ANIME用のカメラ設定を作ります
        player->Update();
        camera->SetNowCamera(4);
        camera->Update(player->IsJumpUp());
        ranking->SetScore(600 - ui->GetTimeCount());   //スコア記録
        ranking->RankProcess(current_screen);          //ランクの処理画面に行きます

        break;
    case GAME_SCREEN::GAME_OVER:
        StopMusic();
        camera->SetNowCamera(5);
        camera->Update(player->IsJumpUp());
        model->SetNowScene(3);
        model->Update();

        if(IsKeyOn(KEY_INPUT_RETURN)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_TITLE;
        };
        break;
    case GAME_SCREEN::GAME_CLEAR:
        StopMusic();
        camera->SetNowCamera(3);
        camera->Update(player->IsJumpUp());
        model->SetNowScene(2);
        model->Update();
        if(IsKeyOn(KEY_INPUT_RETURN)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_TITLE;
        };
        if(IsKeyOn(KEY_INPUT_R)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_RANK;
        };
        break;
    case GAME_SCREEN::GAME_RANK:
        if(IsKeyOn(KEY_INPUT_RETURN)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_TITLE;
        }
        break;
    case GAME_SCREEN::GAME_PARFECT_CLEAR:
        StopMusic();
        camera->SetNowCamera(6);
        camera->Update(player->IsJumpUp());
        model->SetNowScene(4);
        model->Update();

        if(IsKeyOn(KEY_INPUT_RETURN)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_TITLE;
        };
        if(IsKeyOn(KEY_INPUT_R)) {
            if(CheckMusic() == 0) {
                sound->PlayEffectSound();
            }
            current_screen = GAME_SCREEN::GAME_RANK;
        };
        break;
    }
}
//描画処理
//---------------------------------------------
void Game::Render(bool shadow_)
{
    if(shadow_ == false) {
        camera->Render();
    }
    switch(current_screen) {
    case GAME_SCREEN::GAME_TITLE:
        //フォント設定
        ChangeFont("ＭＳ 明朝");
        //先に地図がら描画
        if(shadow_ == false) {
            mapchip->DrawMap();
        }
        //トラップボールの描画
        for(size_t i = 0; i < trap_ball.size(); i++) {
            //周りを黒くするシェーバーシェーダーを使ってモデル描画
            trap_ball[i]->Render();
        }
        //その後地図上の描画
        for(size_t i = 0; i < block_1.size(); i++) {
            block_1[i]->Render();   //一行にまとめるとここでBreakが効かない
        }
        RenderGameTitle();
        break;
    case GAME_SCREEN::GAME_NOWLOADING:
        DrawGameNowLoadingText();
        break;
    case GAME_SCREEN::GAME_RULE:
        RenderGameRule();
        break;
    case GAME_SCREEN::GAME_START_ANIME:
        SetFontSize(60);
        DrawString(1150, 800, "ENTER TO SKIP", GetColor(10, 40, 200), TRUE);
        SetFontSize(40);

        mapchip->DrawMap();
        //トラップボールの描画
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Render();
        }
        for(size_t i = 0; i < block_1.size(); i++) {
            block_1[i]->Render();
        }
        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Render();
        }
        for(size_t i = 0; i < slow_area.size(); i++) {
            slow_area[i]->Render();
        }
        //拾うアイテム実装
        for(size_t i = 0; i < item.size(); i++) {
            if(item[i]->GetIsPicked())
                continue;
            item[i]->Render();
        }
        goal_area->Render();   //ゴールエリア実装
        model->Render();       //モデル描画
       // DrawGameNowLoadingText();
    
        break;
    case GAME_SCREEN::GAME_STAGE:
        //影を描画したくない所
        if(shadow_ == false) {
            mapchip->DrawMap();
        }

        //トラップボールの描画
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Render();
        }

        //影を描画したくない所
        if(shadow_ == false) {
            for(size_t i = 0; i < redgoblin.size(); i++) {
                if(redgoblin[i]->GetIsLive() == false)
                    continue;
                redgoblin[i]->Render();
            }
            Show_Render();
        }

        gameover_area->Render();   //ゲームオーヴァエリア実装

        //拾うアイテム実装
        for(size_t i = 0; i < item.size(); i++) {
            if(item[i]->GetIsPicked())
                continue;
            item[i]->Render();
        }

        goal_area->Render();   //ゴールエリア実装
        player->Render();      //プレイヤーの描画

        //影を描画したくない所
        if(shadow_ == false) {
            ui->DrawMiniMap();   //設定画面の描画
            ui->DrawUi();
        }

        // HintBarに入れるがどうか
        ui->DrawHintBar();

        //ステージ1の更新
        // stage1->Render(shadow_);
        break;
    case GAME_SCREEN::GAME_PAUSE:
        //影を書きたくない所
        if(shadow_ == false) {
            mapchip->DrawMap();
        }

        //トラップボールの描画
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Render();
        }

        //影を描画したくない所
        if(shadow_ == false) {
            for(size_t i = 0; i < redgoblin.size(); i++) {
                if(redgoblin[i]->GetIsLive() == false)
                    continue;
                redgoblin[i]->Render();
            }
            Show_Render();
        }

        gameover_area->Render();   //ゲームオーバーエリア実装

        //拾うアイテム実装
        for(size_t i = 0; i < item.size(); i++) {
            if(item[i]->GetIsPicked())
                continue;
            item[i]->Render();
        }

        goal_area->Render();   //ゴールエリア実装
        player->Render();      //プレイヤーの描画

        //影を描画したくない所
        if(shadow_ == false) {
            //2D用の小さいマップの描画
            ui->DrawMiniMap();
            ui->DrawUi();
        }

        //ステージ1の更新
        //stage1->Render(shadow_);

        ui->DrawGamepauseText(music_on, sound->GetVolume());   //設定画面の描画
        break;
    case GAME_SCREEN::GAME_CLEAR_ANIME:

        //影を描画したくない所
        if(shadow_ == false) {
            mapchip->DrawMap();
        }

        //トラップボールの描画
        for(size_t i = 0; i < trap_ball.size(); i++) {
            trap_ball[i]->Render();
        }

        //影を描画したくない所
        if(shadow_ == false) {
            //レッドゴブリンの描画
            for(size_t i = 0; i < redgoblin.size(); i++) {
                if(redgoblin[i]->GetIsLive() == false)
                    continue;
                redgoblin[i]->Render();
            }
            Show_Render();
        }

        gameover_area->Render();   //ゲームオーヴァエリア実装

        //拾うアイテム実装
        for(size_t i = 0; i < item.size(); i++) {
            if(!item[i]->GetIsPicked()) {
                item[i]->Render();
            }
        }
        goal_area->Render();   //ゴールエリア実装
        player->Render();      //プレイヤーの描画

        //影を描画したくない所
        if(shadow_ == false) {
            ui->DrawMiniMap();   //2D用の小さいマップの描画
            ui->DrawUi();        //HPとカウンターの描画
        }

        break;
    case GAME_SCREEN::GAME_CLEAR:
        camera->Render();
        model->Render();
        ranking->DrawGameClearText();
        break;
    case GAME_SCREEN::GAME_PARFECT_CLEAR:
        camera->Render();
        model->Render();
        ranking->DrawPetfectClearText();
        break;
    case GAME_SCREEN::GAME_OVER:
        camera->Render();
        model->Render();
        ui->DrawGameoverText();
        SetBackgroundColor(200, 200, 200);
        break;
    case GAME_SCREEN::GAME_RANK:
        ranking->DrawGameRankText();
        break;
    }
}
void Game::Exit()
{
    //ゲーム終わっていた時全ての物は解放される
    if(camera) {
        delete camera;
        camera = nullptr;
    }
    if(mapchip) {
        delete mapchip;
        mapchip = nullptr;
    }
    if(player) {
        delete player;
        player = nullptr;
    }
    if(model) {
        delete model;
        model = nullptr;
    }
    if(ranking) {
        delete ranking;
        ranking = nullptr;
    }
    if(ui) {
        delete ui;
        ui = nullptr;
    }
    if(sound) {
        delete sound;
        sound = nullptr;
    }
    if(gameover_area) {
        delete gameover_area;
        gameover_area = nullptr;
    }
    if(goal_area) {
        delete goal_area;
        goal_area = nullptr;
    }

    redgoblin.clear();
    block_1.clear();
    trap_ball.clear();
    trap_area.clear();
    slow_area.clear();
    item.clear();
    move_block.clear();

    //if(stage1) {
    //    delete stage1;
    //    stage1 = nullptr;
    //}
}
