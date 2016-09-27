/*
 * player.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */


#include "header.h"


struct player_info player;	//プレイヤー構造体
SDL_Surface *win_baf;		//プレイヤー描画のバッファー

SDL_Surface *CharaChips;	//キャラ画像のロード

//初期化
void player_initialize()
{
	CharaChips = IMG_Load("pictures/char_m01.png");

}


//再初期化
void player_reinitialize(){

}


//計算等
void player_update(){

}

//描画
void player_draw()
{


}


//リモコン操作
void player_wii(){

}

//終了処理（あれば）
void player_finalize(){

}
