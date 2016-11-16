/*
 * title.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */


#include "header.h"

static SDL_Surface *title_back;
static SDL_Surface *title_name;
static SDL_Surface *dokuro_over;
static SDL_Surface *kyouryoku;

//初期化
void title_initialize(){
	title_back  = IMG_Load("pictures/title_back.png");
	title_name  = IMG_Load("pictures/title_name.png");
	dokuro_over = IMG_Load("pictures/dokuro_over.png");
	kyouryoku   = IMG_Load("pictures/kyouryoku.png");
}


//再初期化
void title_reinitialize(){

}


//計算等
void title_update(){

}

//描画
void title_draw(){
	SDL_BlitSurface(title_back, NULL, window, NULL);
	SDL_BlitSurface(title_name, NULL, window, NULL);
	SDL_BlitSurface(dokuro_over, NULL, window, NULL);
	SDL_BlitSurface(kyouryoku, NULL, window, NULL);
}


//リモコン操作
void title_wii(){
	//HOMEボタン
	if (key[9] >= 1) {
		loop_flag = 0;
	}

	/* 2ボタン */
	if (key[2] == 1) {
		layer = play;
	}
}

//終了処理（あれば）
void title_finalize(){

}
