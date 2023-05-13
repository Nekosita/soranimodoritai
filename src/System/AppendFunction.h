#pragma once

//クラスの前方宣言
class Base;
class BlockBase;
class ModelBase;

//!接触確認
bool AttachCheck(const VECTOR &pos1, float r1, const VECTOR &pos2, float r2);

//!接触確認
bool AttachCheck2D(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);

//!プレイヤーの座標を小さいマップの上に載せた時、その座標換算用
float PosInMapX(const VECTOR& pos1);
float PosInMapY(const VECTOR& pos1);

//!距離取る用
float GetDistance(const VECTOR& pos1, const VECTOR& pos2);

bool CheckHitStageObject(const Base* obj1, const Base* obj2, float addjust_y = 0.0f);
bool IsHitBall(const ModelBase* ball, const ModelBase* player);
bool CheckHitWithEnemy(const Base* obj1, const ModelBase* obj2);

