#include"Base.h"

class Player;
#pragma once

//! カメラのベースクラス
//===============================================
class Camera : public Base
{
	//!カメラのパターンの設定
	enum CAMERA_PATTERN
	{
		OPENING_ANIME,	
		GAME_START,
		TPV,
		GAME_CLEAR,
		GAME_CLEAR_MOVIE,
		GAME_OVER,
		GAME_PARFECT_CLEAR,
		NOTHING
	};

	int camera_type;//!<今使っていたカメラの番号
	CAMERA_PATTERN now_camera;//!<今使われたカメラのパターンの設定

	VECTOR gravity_line_start;	  //!<プレイヤーの重力ラインの上
	VECTOR gravity_line_end;	  //!<プレイヤーの重力ラインの下

	VECTOR top_line_start;	  //プレイヤーの頭上ラインの下
	VECTOR top_line_end;	  //プレイヤーの頭上ラインの上

	VECTOR look;//!<カメラが見る座標
	VECTOR camera_pos = { 0.0f,0.0f,0.0f };	//!<カメラの座標
	float camera_dist;//!<カメラとの距離を置く
	float camrea_h_rotate;//!<カメラのx軸
	float camrea_v_rotate;//!<カメラのｙ軸
	float camera_height;//!<カメラの高さ
	float y_dis;//!<カメラの見る座標の
	int oa_count;//!<OPENING_ANIME用カメラカウンター設定

	float scale = 1.0f;//!<カメラの遠近調整
	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

public:

	//!カメラのコンストラクタ
	Camera();
	//!カメラのデストラクタ
	~Camera();

    //! @name 上演シーン設定
    //!@{
	void SetNowCamera(int now_camera_nom);
	int GetNowCamera() const;
    //!@}

	void SetPosY(float now_y);
	void SetPosWhenHit(float block_hight);

    //! @name プレイヤー重力ラーイン関係設定
    //!@{
	void   SetGravityLineStart(VECTOR& now_gravity_start);
	VECTOR GetGravityLineStart() const;
	void   SetGravityLineEnd(VECTOR& now_gravity_end);
	VECTOR GetGravityLineEnd() const;
    //!@}

	  //! @name プレイヤー重力ラーイン関係設定
    //!@{
	void   SetTopLineStart(VECTOR& now_top_start);
	VECTOR GetTopLineStart() const;
	void   SetTopLineEnd(VECTOR& now_top_end);
	VECTOR GetTopLineEnd() const;
    //!@}
	
	  //! @name OA_count設定
    //!@{
	void SetOpenAnimeCount(int now_OA_count);
	int GetOpenAnimeCount() const;
    //!@}
	
	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	//カメラの最大値範囲
	void CameraPosLimit();
    //!更新処理
	void Update(int mov_);
    //!描画処理
	void Render();	
	//!終了処理
	void Exit();	

};
