/*
 * player.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */

#include "header.h"
#include "player_header.h"

/********************************      変数      *******************************************/

float CameraSpeed;		//カメラの移動速度

struct player_info player; //プレイヤー構造体

float vx; 			//プレイヤーのx速度
float vy; 			//プレイヤーのy速度
float Max_Speed;	//プレイヤーの現在の最大速度

int walk_flag;	//歩いているかいないか
int dash_flag;	//走っているかいないか
int tired_flag;	//SPが切れた状態

int direction;	//現在向いている方向を格納

SDL_Surface *win_baf; 		//プレイヤー描画のバッファー
SDL_Surface *shadow;		//影
SDL_Surface *CharaChips; 	//キャラ画像のロード

SDL_Rect player_img_pos = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; //プレイヤー画像の切り取り先
SDL_Rect player_pos = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; 	//ウィンドウ上のプレイヤー画像画面
SDL_Rect MainCamera = { 0, 0, MAP_WINDOW_WIDTH, MAP_WINDOW_HEIGHT }; //ウィンドウ上に映る部分

SDL_Rect HP_gauge = {30,500,BASIC_GAUGE_LEN,10};
SDL_Rect SP_gauge = {30,520,BASIC_GAUGE_LEN,10};
int max_HP_gauge_len;
int max_SP_gauge_len;


/********************************      関数      ********************************************/

//初期化
void player_initialize()
{
	//バッファリング用に空のサーフェス作成
	win_baf = SDL_CreateRGBSurface(SDL_HWSURFACE,WINDOW_WIDTH ,WINDOW_HEIGHT, 32, 0, 0, 0, 0);
	SDL_SetColorKey(win_baf, SDL_SRCCOLORKEY, SDL_MapRGB(win_baf->format, 0, 0, 0));



	//画像の読み込み
	CharaChips = IMG_Load("pictures/debug.png");

	//数値の初期化
	player.x = 96;
	player.y = 96;

	player_reinitialize();
}

//再初期化
void player_reinitialize()
{
	direction = down;
	walk_flag = 0;
	dash_flag = 0;
	Max_Speed = WALK_MAX;

	player.MaxHP = BASIC_HP;
	player.MaxSP = BASIC_SP;
	player.HP = player.MaxHP;
	player.SP = player.MaxSP;

	max_HP_gauge_len = BASIC_GAUGE_LEN;
	max_SP_gauge_len = BASIC_GAUGE_LEN;

	HP_gauge.w = max_HP_gauge_len;
	SP_gauge.w = max_SP_gauge_len;

}

//計算等
void player_update()
{
	//移動していないとき、速度をゼロに
	if (walk_flag == 0)
		player_friction();
	else{ //移動しているとき
		player_move();		//プレイヤーの座標を移動
		map_judge();		//プレイヤーの移動先が壁か調べる
	}

	//プレイヤーの移動先に向かってカメラを移動
	camera_move();

	//いったん描画用の座標に移動先の座標を代入
	player_pos.x = player.x;
	player_pos.y = player.y;

	//プレイヤーの描画座標を画面上の座標に直す
	player_pos = relative_point(player_pos,MainCamera);

	SP_calculate();		//スタミナ計算

	//各ゲージの値を更新
	HP_gauge.w = gauge_calculate(max_HP_gauge_len,player.HP,player.MaxHP);
	SP_gauge.w = gauge_calculate(max_SP_gauge_len,player.SP,player.MaxSP);

	//アニメーション処理
	if(walk_flag==1)
	{
		player_img_pos.x += PLAYER_WIDTH;
		if(player_img_pos.x == PLAYER_WIDTH*3 )
			player_img_pos.x=0;
	}
	else{
		player_img_pos.x = PLAYER_WIDTH *1;
	}



}

//描画まとめ
void player_draw() {

	//描画バッファをクリアする
	SDL_FillRect(win_baf, NULL, SDL_MapRGB(window->format, 0, 0, 0) );

	//ゲージの描画
	SDL_FillRect(win_baf,&HP_gauge,SDL_MapRGB(window->format, 0, 255, 0));
	SDL_FillRect(win_baf,&SP_gauge,SDL_MapRGB(window->format, 255, 255, 0));

	//キャラクターの描画
	//SDL_FillRect(win_baf, &player_pos, SDL_MapRGB(window->format, 0, 255, 0) );
	SDL_BlitSurface(CharaChips, &player_img_pos, win_baf, &player_pos);

	//バッファーをメインウィンドウに描画
	SDL_BlitSurface(win_baf,NULL,window,NULL);


}

//リモコン操作
void player_wii() {
	/* 1ボタン */
	if (key[1] >= 1) {

	}

	/* 2ボタン */
	if (key[2] == 1) {
		dash_flag = 1;
	} else if (key[2] == 0) {
		dash_flag = 0;
	}

	/*****   十字キー下   *****/
	if (key[5] == 1) {
		direction = down;
		player_img_pos.y = PLAYER_HEIGHT * 2;
		walk_flag = 1;
	}

	/*******    十字キー上　  ********/
	if (key[6] == 1) {
		direction = up;
		player_img_pos.y = PLAYER_HEIGHT * 0;
		walk_flag = 1;
	}
	/*****    十字キー   *****/
	if (key[7] == 1) {
		direction = left;
		player_img_pos.y = PLAYER_HEIGHT * 3;
		walk_flag = 1;
	}
	/*****    down    *****/
	if (key[8] == 1) {
		direction = right;
		player_img_pos.y = PLAYER_HEIGHT * 1;
		walk_flag = 1;
	}
	//十字キーが押されていないとき
	if (key[5] == 0 && key[6] == 0 && key[7] == 0 && key[8] == 0) {
		walk_flag = 0;
	}

	//HOMEボタン
	if (key[9] >= 1) {
		loop_flag = 0;

	}
}

//終了処理（あれば）
void player_finalize() {

}





//プレイヤーの歩行を司る関数
//11/11加速度のシステム廃止
void player_move()
{
	/* 斜め移動ができないようにする
	 * 横方向から縦方向に進む際に横方向の速度を０にしている*/
	if (direction == left || direction == right)
		vy = 0;
	else
		vx = 0;

	//ダッシュの時の最大速度を設定する
	if (dash_flag == 1)
		Max_Speed = DASH_MAX;
	else
		Max_Speed = WALK_MAX;

	if(tired_flag == 1)
		Max_Speed = WALK_MAX/2;


	//進む方向にあわせて、x速度,y速度の値を変更
	if(direction==left)
		vx = -Max_Speed;
	else if(direction==right)
		vx = Max_Speed;
	else if(direction == up)
		vy = -Max_Speed;
	else if(direction==down)
		vy = Max_Speed;
	else{
		vx = 0;
		vy = 0;
	}

	//プレイヤーの座標を動かす
	player.x += vx;
	player.y += vy;



}




//プレイヤーの移動速度を落とす関数。
// 9/30　あまり意味が無いので廃止
void player_friction() {
	vx = 0;
	vy = 0;

}




//マップの当たり判定。(バグを見つけたら即座に報告ください）
void map_judge() {
	int right_point, left_point, top_point, under_point;
	int i;

	right_point = 0;
	left_point = 0;
	top_point = 0;
	under_point = 0;

	//左方向
	if (direction == left) {
		for (i = 0; i <= PLAYER_HEIGHT; i++) {
			if (map_value(player.x / MAP_TIP_SIZE, (player.y + i) / MAP_TIP_SIZE) != 0)
				left_point++;
		}
		if (left_point >= 10) {
			player.x = (player.x / MAP_TIP_SIZE +1) * MAP_TIP_SIZE ;
			vx = 0;

		}
	}
	//右方向
	else if (direction == right) {
		for (i = 0; i <  PLAYER_HEIGHT; i++) {
			if (map_value((player.x +  PLAYER_WIDTH) / MAP_TIP_SIZE, (player.y + i) / MAP_TIP_SIZE) != 0)
				right_point++;
		}
		if (right_point >= 10) {
			player.x = ((player.x +  PLAYER_WIDTH) / MAP_TIP_SIZE) * MAP_TIP_SIZE -  PLAYER_WIDTH;
			vx = 0;
		}
	}

	//下方向
	else if (direction == down) {
		for (i = 0; i <  PLAYER_WIDTH; i++) {
			if (map_value((player.x + i) / MAP_TIP_SIZE, (player.y + PLAYER_HEIGHT) / MAP_TIP_SIZE) != 0)
				under_point++;
		}
		if (under_point >= 10) {
			player.y = (player.y +  PLAYER_HEIGHT) / MAP_TIP_SIZE * MAP_TIP_SIZE -  PLAYER_HEIGHT;
			vy = 0;
		}
	}
	//上方向
	else if (direction == up) {
		for (i = 0; i < PLAYER_WIDTH; i++) {
			if (map_value((player.x + i) / MAP_TIP_SIZE, player.y / MAP_TIP_SIZE) != 0)
				top_point++;
		}
		if (top_point >= 10) {
			player.y = (player.y / MAP_TIP_SIZE) * MAP_TIP_SIZE +  PLAYER_HEIGHT;
			vy = 0;
		}
	}

}




//スタミナの増減を行う関数
void SP_calculate()
{
	//ダッシュボタンを押して移動中
	if(walk_flag == 1 && dash_flag == 1 && tired_flag == 0)
	{
		player.SP -= SP_DECREASE;

		if(player.SP <= 0 ){
			player.SP = 0;
			tired_flag = 1;
		}
	}

	else
	{
		player.SP += SP_INCREASE;
		if(walk_flag == 0)
			player.SP += SP_INCREASE;

		if(player.SP >= player.MaxSP ){
			player.SP = player.MaxSP;
			tired_flag = 0;
		}
	}

}





//main_cameraの移動を行う関数
void camera_move()
{
	//キャラクターが走っているとき
	if(dash_flag == 1 && tired_flag == 0)
		CameraSpeed = DASH_MAX;
	else //それ以外
		CameraSpeed = WALK_MAX;

	if (player.x < (MainCamera.x + (MainCamera.x + MainCamera.w)) /2 - PLAYER_WIDTH) {
		if (0 < MainCamera.x)
			MainCamera.x -= CameraSpeed;
	}
	if (player.x > (MainCamera.x + (MainCamera.x + MainCamera.w)) / 2 - PLAYER_WIDTH) {
		if ((MainCamera.x + MainCamera.w) < MAP_WIDE*2*MAP_TIP_SIZE)
			MainCamera.x += CameraSpeed;
	}

	if (player.y < (MainCamera.y + MainCamera.y + MainCamera.h) / 2 - PLAYER_HEIGHT) {
		if (0 < MainCamera.y)
			MainCamera.y -= CameraSpeed;

	}
	if (player.y > (MainCamera.y + MainCamera.y + MainCamera.h) / 2 - PLAYER_HEIGHT) {
		if ((MainCamera.y + MainCamera.h) < MAP_HEIGHT*2*MAP_TIP_SIZE)
			MainCamera.y += CameraSpeed;

	}


}




//aのbからの相対座標を導く
SDL_Rect relative_point(SDL_Rect a,SDL_Rect b)
{
	SDL_Rect buf;
	buf.x = a.x - b.x;
	buf.y = a.y - b.y;
	buf.w = a.w;
	buf.h = a.h;
	return buf;
}



//なんかゲージを計算する関数（汎用性低し・・・）
int gauge_calculate(int w,int p,int max_p)
{
	int after_w;

	after_w = w * ( (float)p/(float)max_p );

	return after_w;
}


