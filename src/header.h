/*
 * header.h
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */


/* include ******************************************************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>	// SDLを用いるために必要なヘッダファイル
#include <SDL/SDL_gfxPrimitives.h>	// 描画関係のヘッダファイル
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h> // SDLでサウンドを用いるために必要なヘッダファ
//#include <libcwiimote/wiimote.h>	// Wiiリモコンを用いるために必要なヘッダファイル
//#include <libcwiimote/wiimote_api.h>	// Wiiリモコンを用いるために必要なヘッダファイル

//-lcwiimote

/* 定義 ******************************************************************************************************************************/

#define WINDOW_WIDTH 800//480//ウインドウの横幅
#define WINDOW_HEIGHT 800//640//ウインドウの高さ

/* 構造体 ******************************************************************************************************************************/

struct player_info{//プレイヤーについての情報
    int x;
    int y;
    int hp;
    int stamina;//スタミナ
    int key;//キーアイテムを所持しているかのフラグ　０：未所持　１：所持
    int stock;//ストックアイテムの識別番号（所持していない場合：ー１）
    int item;//発動しているアイテムの効果（ない場合：ー１）
    int map_score;//このマップでの獲得ポイント
    int sum_score;//総獲得ポイント
};

struct enemy_info{//敵についての情報
    int x;
    int y;
    int discovery;//プレイヤーを発見しているかのフラグ　−１：未発見　プレイヤーの番号：発見
};
/* グローバル変数 ***************************************************************************************************************************/


extern int layer;
extern SDL_Surface *window; // ウィンドウ（画像）データへのポインタ
/* 列挙体 ******************************************************************************************************************************/

enum { //レイヤー
	tytle, loading, play, result, final_result
};

/* プロトタイプ宣言 ******************************************************************************************************************************/


//playgamen
void playgamen_initialize();//初期化
void playgamen_reinitialize();//再初期化
void playgamen_update();//計算等
void playgamen_draw();//描画
void playgamen_wii();//リモコン操作
void playgamen_finalize();//終了処理（あれば）

//final_result
void final_result_initialize();//初期化
void final_result_reinitialize();//再初期化
void final_result_update();//計算等
void final_result_draw();//描画
void final_result_wii();//リモコン操作
void final_result_finalize();//終了処理（あれば）

//result_
void result_initialize();//初期化
void result_reinitialize();//再初期化
void result_update();//計算等
void result_draw();//描画
void result_wii();//リモコン操作
void result_finalize();//終了処理（あれば）

//loading_
void loading_initialize();//初期化
void loading_reinitialize();//再初期化
void loading_update();//計算等
void loading_draw();//描画
void loading_wii();//リモコン操作
void loading_finalize();//終了処理（あれば）

//item_
void item_initialize();//初期化
void item_reinitialize();//再初期化
void item_update();//計算等
void item_draw();//描画
void item_wii();//リモコン操作
void item_finalize();//終了処理（あれば）

//map_
void map_initialize();//初期化
void map_reinitialize();//再初期化
void map_update();//計算等
void map_draw();//描画
void map_wii();//リモコン操作
void map_finalize();//終了処理（あれば）
void map_create();//マップの生成
int map_value(int x, int y);//マップの値を返す

//enemy_
void enemy_initialize();//初期化
void enemy_reinitialize();//再初期化
void enemy_update();//計算等
void enemy_draw();//描画
void enemy_wii();//リモコン操作
void enemy_finalize();//終了処理（あれば）

//player_
void player_initialize();//初期化
void player_reinitialize();//再初期化
void player_update();//計算等
void player_draw();//描画
void player_wii();//リモコン操作
void player_finalize();//終了処理（あれば）

//tytle_
void tytle_initialize();//初期化
void tytle_reinitialize();//再初期化
void tytle_update();//計算等
void tytle_draw();//描画
void tytle_wii();//リモコン操作
void tytle_finalize();//終了処理（あれば）
