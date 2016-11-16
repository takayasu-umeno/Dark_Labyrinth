/*
 * player_header.h
 *
 *  Created on: 2016/11/11
 *      Author: vmware
 */

#ifndef PLAYER_HEADER_H_
#define PLAYER_HEADER_H_

/********************************     定義など      **************************************/

#define BASIC_HP 300
#define BASIC_SP 500
#define BASIC_GAUGE_LEN 150
#define BASIC_CIRCLE_R 50

#define ACCEL (float)0.1 		//プレイヤーの加速度
#define DASH_MAX 5				//最大ダッシュ速度
#define WALK_MAX 2			//最大ウォーク速度
#define SP_DECREASE 2
#define SP_INCREASE 1
#define PLAYER_WIDTH 24		//プレイヤーの横の長さ
#define PLAYER_HEIGHT 32		//プレイヤーの縦の大きさ


#define MAP_WINDOW_WIDTH 800	//マップを表示する横の長さ
#define MAP_WINDOW_HEIGHT 480	//マップを表示する縦の長さ
#define MAP_WIDE 40			//マップの横のチップ数
#define MAP_HEIGHT 40			//マップの縦のチップ数
#define MAP_TIP_SIZE 32		//マップチップの大きさ
#define FLOAR_N 4				//
#define MINMAP_SIZE 7			//
enum {
	up, down, left, right
};

/********************************   プロトタイプ宣言   **************************************/

void player_initialize(); 	//初期化
void player_reinitialize(); 	//再初期化
void player_update(); 		//計算等
void player_draw();			//描画
void player_wii(); 			//リモコン操作
void player_finalize(); 		//終了処理（あれば）

void player_move(); 			//プレイヤーの歩行
void player_friction(); 		//プレイヤーの動きを止める関数
void map_judge(); 				//マップとのあたり判定

void camera_move(); 			//MainCameraを動かす関数

void SP_calculate();			//スタミナ計算関数

int gauge_calculate(int w,int p,int max_p);
SDL_Rect relative_point(SDL_Rect a,SDL_Rect b);


#endif /* PLAYER_HEADER_H_ */
