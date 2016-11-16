/*
 * map.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */

/*
 * キャラの画像仕様補正
 *
 *
 */
#include "header.h"

#define MAP_WIDE 40
#define MAP_HEIGHT 40
#define MAP_TIP_SIZE 32
#define FLOAR_N 4
#define MINMAP_SIZE 7
#define MAPTIP_NUM 33
/****************************************************************************************/

enum {
	room = -1, road = 0, wall
};
/****************************************************************************************/

extern SDL_Rect MainCamera; //プレイヤーを中心としたSDL_Rect

static SDL_Event event; // SDLによるイベントを検知するための構造体

SDL_Surface *map_buf; //マップシステムの中間描画用サーフェス
static SDL_Surface *minmap_surface; //ミニマップ描画用
static SDL_Surface *map_surface; //マップ描画用
static SDL_Surface *map_tip_img[MAPTIP_NUM];

static int main_map[MAP_WIDE * 2][MAP_HEIGHT * 2]; //マップ描画用データ
static int minmap[MAP_WIDE * 2][MAP_HEIGHT * 2]; //ミニマップ描画用データ
//static int minmap_size = 2; //ミニマップの1マスの大きさ
static int p_map_x = 5, p_map_y = 10;

/****************************************************************************************/
//デバック用
//static int player.x = 5, player.y = 10;
//static int pla_x = 5 * MAP_TIP_SIZE, pla_y = 10 * MAP_TIP_SIZE;
static int key_check = -1;

/****************************************************************************************/

void floar_create(int count, int map[MAP_WIDE][MAP_HEIGHT], int road_x[FLOAR_N],
		int road_y[FLOAR_N][2]); //フロア生成
void road_create(int count, int map[MAP_WIDE][MAP_HEIGHT], int road_x[FLOAR_N],
		int road_y[FLOAR_N][2]); //部屋と通路をつなぐ
void connect_floar(); //フロア同士をつなぐ通路生成
void update_player_place(); //プレイヤーの場所をマス目座標に
void minmap_draw(); //ミニマップの描画
void minmap_transfer(); //描画サーフェスへ転送
void search_around_player(); //プレイヤー周辺のミニマップ探索

void wall_change(); //壁のマップチップの値を変える

int get_can_walk(int x, int y); //歩けるかどうかの値を返す
/****************************************************************************************/

//初期化
void map_initialize() {
	map_buf = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH, WINDOW_HEIGHT,
			32, 0, 0, 0, 0);

	map_surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
			MAP_WIDE * 2 * MAP_TIP_SIZE, MAP_HEIGHT * 2 * MAP_TIP_SIZE, 32, 0,
			0, 0, 0);
	minmap_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH,
			WINDOW_HEIGHT, 32, 0, 0, 0, 0);

	srand((unsigned int) time(NULL)); //乱数初期化

	/************画像ロード**************/
//	road_img = IMG_Load("pictures/road.jpg");
//	wall_img = IMG_Load("pictures/wall.jpg");
	map_tip_img[0] = IMG_Load("pictures/map/road.jpg");
	map_tip_img[1] = IMG_Load("pictures/map/kabe1.jpg");
	map_tip_img[2] = IMG_Load("pictures/map/kabe2.jpg");
	map_tip_img[3] = IMG_Load("pictures/map/kabe3.jpg");
	map_tip_img[4] = IMG_Load("pictures/map/kabe4.jpg");
	map_tip_img[5] = IMG_Load("pictures/map/kabe5.jpg");
	map_tip_img[6] = IMG_Load("pictures/map/kabe6.jpg");
	map_tip_img[7] = IMG_Load("pictures/map/kabe7.jpg");
	map_tip_img[8] = IMG_Load("pictures/map/kabe8.jpg");
	map_tip_img[9] = IMG_Load("pictures/map/kabe9.jpg");
	map_tip_img[10] = IMG_Load("pictures/map/kabe10.jpg");
	map_tip_img[11] = IMG_Load("pictures/map/kabe11.jpg");
	map_tip_img[12] = IMG_Load("pictures/map/kabe12.jpg");
	map_tip_img[13] = IMG_Load("pictures/map/kabe13.jpg");
	map_tip_img[14] = IMG_Load("pictures/map/kabe14.jpg");
	map_tip_img[15] = IMG_Load("pictures/map/kabe15.jpg");
	map_tip_img[16] = IMG_Load("pictures/map/kabe16.jpg");
	map_tip_img[17] = IMG_Load("pictures/map/kabe17.jpg");
	map_tip_img[18] = IMG_Load("pictures/map/kabe18.jpg");
	map_tip_img[19] = IMG_Load("pictures/map/kabe19.jpg");
	map_tip_img[20] = IMG_Load("pictures/map/kabe20.jpg");
	map_tip_img[21] = IMG_Load("pictures/map/kabe21.jpg");
	map_tip_img[22] = IMG_Load("pictures/map/kabe22.jpg");
	map_tip_img[23] = IMG_Load("pictures/map/kabe23.jpg");
	map_tip_img[24] = IMG_Load("pictures/map/kabe24.jpg");
	map_tip_img[25] = IMG_Load("pictures/map/kabe25.jpg");
	map_tip_img[26] = IMG_Load("pictures/map/kabe26.jpg");
	map_tip_img[27] = IMG_Load("pictures/map/kabe27.jpg");
	map_tip_img[28] = IMG_Load("pictures/map/kabe28.jpg");
	map_tip_img[29] = IMG_Load("pictures/map/kabe29.jpg");
	map_tip_img[30] = IMG_Load("pictures/map/kabe30.jpg");
	map_tip_img[31] = IMG_Load("pictures/map/kabe31.jpg");
	map_tip_img[32] = IMG_Load("pictures/map/kabe32.jpg");
	map_tip_img[33] = IMG_Load("pictures/map/kaidan.jpg");

	map_reinitialize(); //再初期化
}

//再初期化
void map_reinitialize() {
	int i, j;
	for (i = 0; i < MAP_WIDE; i++) {
		for (j = 0; j < MAP_HEIGHT; j++) {
			minmap[i][j] = 0;
		}
	}

	map_create(); //マップ生成

	//SDL_FillRect(map_surface, NULL,SDL_MapRGB(map_surface->format, 0, 0, 0));
	for (i = 0; i < MAP_WIDE * 2; i++) {
		for (j = 0; j < MAP_HEIGHT * 2; j++) {
			SDL_Rect place = { i * MAP_TIP_SIZE, j * MAP_TIP_SIZE, MAP_TIP_SIZE,
					MAP_TIP_SIZE };
//			switch (main_map[i][j]) {
//			case road:
//				SDL_FillRect(map_surface, &place,
//						SDL_MapRGB(map_surface->format, 255, 255, 255));
////				SDL_BlitSurface(road_img,NULL,map_surface,&place);
//				break;
//			case 1:
//				SDL_BlitSurface(map_tip_img[main_map[i][j]],NULL,map_surface,&place);
//				break;
			if (main_map[i][j] == room)
				SDL_BlitSurface(map_tip_img[road], NULL, map_surface, &place);
			else
				SDL_BlitSurface(map_tip_img[main_map[i][j]], NULL, map_surface,
						&place);

//			}
		}
	}

}
static int count = 0;
//計算等
void map_update() {

	count++;

	update_player_place(); //プレイヤーの場所をマス目座標に

	search_around_player(); //プレイヤー周辺のミニマップ探索

//	switch (key_check) {
//	case SDLK_UP:
//		pla_y -= 2;
//		break;
//	case SDLK_DOWN:
//		pla_y += 2;
//		break;
//	case SDLK_LEFT:
//		pla_x -= 2;
//		break;
//	case SDLK_RIGHT:
//		pla_x += 2;
//		break;
//	}
}

//描画
void map_draw() {

	SDL_BlitSurface(map_surface, &MainCamera, map_buf, NULL);

//	/***************************/
//	SDL_Rect player = { pla_x, pla_y, MAP_TIP_SIZE, MAP_TIP_SIZE };
//	SDL_FillRect(window, &player, SDL_MapRGB(window->format, 255, 0, 0));
//	/****************************/

	minmap_draw(); //ミニマップの描画
	minmap_transfer(); //描画サーフェスへ転送

	SDL_BlitSurface(map_buf, NULL, window, NULL);

}

//リモコン操作
void map_wii() {
	if (SDL_PollEvent(&event)) {
		switch (event.type) { // 押されたキーごとに処理
		case SDL_QUIT:
			// SDLの利用を終了する時
			SDL_Quit();
			exit(0);
			break;
		case SDL_KEYDOWN: // キーボードのキーが押された時
			// 押されたキーごとに処理
//			switch (event.key.keysym.sym) {
//			case SDLK_ESCAPE: // Escキーが押された時
//				printf("ESCAPE key -> EXIT.\n");
//				SDL_Quit();
//				exit(0);
//				break;
//
//			case SDLK_UP:
//				player.y -= 1;
//				break;
//			case SDLK_DOWN:
//				player.y += 1;
//				break;
//			case SDLK_LEFT:
//				player.x -= 1;
//				break;
//			case SDLK_RIGHT:
//				player.x += 1;
//				break;
//			}
			key_check = event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			key_check = -1;
			break;
		}

	}

}

//終了処理（あれば）
void map_finalize() {

}

//マップの値を返す
int map_value(int x, int y) {

	return main_map[x][y];
}

//フロア生成
void floar_create(int count, int map[MAP_WIDE][MAP_HEIGHT], int road_x[FLOAR_N],
		int road_y[FLOAR_N][2]) {

	int i, j;
	for (i = 0; i < MAP_WIDE; i++) {
		for (j = 0; j < MAP_HEIGHT; j++) {
			if (count == 0
					&& (i == 0 || j == 0 || i == MAP_WIDE - 1
							|| j == MAP_HEIGHT - 1))
				map[i][j] = wall; //外周の壁生成
			else if (count == 1
					&& (j == 0 || i == MAP_HEIGHT - 1 || j == MAP_WIDE - 1))
				map[i][j] = wall; //外周の壁生成
			else if (count == 2
					&& (i == 0 || j == MAP_WIDE - 1 || i == MAP_HEIGHT - 1))
				map[i][j] = wall; //外周の壁生成
			else if (count == 3 && (i == MAP_WIDE - 1 || j == MAP_HEIGHT - 1))
				map[i][j] = wall; //外周の壁生成
			else if (i == road_x[count] || i == road_x[count] + 1)
				map[i][j] = road; //通路生成
			else if ((j == road_y[count][0] || j == road_y[count][0] + 1)
					&& i > 0 && i < road_x[count])
				map[i][j] = road; //通路生成
			else if ((j == road_y[count][1] || j == road_y[count][1] + 1)
					&& i > road_x[count]&& i < MAP_WIDE)
				map[i][j] = road; //通路生成
			else if (i == road_x[count] - 1 || i == road_x[count] + 2)
				map[i][j] = wall; //通路周りの壁生成
			else if ((j == road_y[count][0] - 1 || j == road_y[count][0] + 2)
					&& i > 0 && i < road_x[count])
				map[i][j] = wall; //通路周りの壁生成
			else if ((j == road_y[count][1] - 1 || j == road_y[count][1] + 2)
					&& i > road_x[count]&& i < MAP_WIDE)
				map[i][j] = wall; //通路周りの壁生成

			else
				map[i][j] = road;
		}
	}
}

//マップの生成
void map_create() {
	int i, j;
	int count;
	int map[MAP_WIDE][MAP_HEIGHT]; //マップ生成の中継用
	int road_x[FLOAR_N], road_y[FLOAR_N][2]; //通路生成用

	for (count = 0; count < FLOAR_N; count++) {
		road_x[count] = rand() % (MAP_WIDE - 10) + 5;
		road_y[count][0] = rand() % (MAP_HEIGHT - 10) + 5;
		road_y[count][1] = rand() % (MAP_HEIGHT - 10) + 5;

		floar_create(count, map, road_x, road_y); //フロア生成

		road_create(count, map, road_x, road_y); //部屋と通路をつなぐ

		for (i = 0; i < MAP_WIDE; i++) {
			for (j = 0; j < MAP_HEIGHT; j++) {
				main_map[i + count % 2 * MAP_WIDE][j + count / 2 * MAP_HEIGHT] =
						map[i][j];
			}
		}
	}

	connect_floar(); //フロア同士をつなぐ通路生成
	wall_change(); //壁のマップチップの値と対応させる
}

//部屋と通路をつなぐ
void road_create(int count, int map[MAP_WIDE][MAP_HEIGHT], int road_x[FLOAR_N],
		int road_y[FLOAR_N][2]) {

	int x, y;
	//	int ran = rand() % 2;
	//	if(ran == 0)
	y = rand() % (road_y[count][0] - 2) + 1;
	map[road_x[count] - 1][y] = road;
	if (map[road_x[count] - 1][y + 1] == wall
			&& map[road_x[count] - 1][y + 2] == wall)
		map[road_x[count] - 1][y + 1] = road;
	else if (map[road_x[count] - 1][y - 1] == wall
			&& map[road_x[count] - 1][y - 2] == wall)
		map[road_x[count] - 1][y - 1] = road;

	//	else if(ran == 1)
	x = rand() % (road_x[count] - 2) + 1;
	map[x][road_y[count][0] - 1] = road;
	if (map[x + 1][road_y[count][0] - 1] == wall
			&& map[x + 2][road_y[count][0] - 1] == wall)
		map[x + 1][road_y[count][0] - 1] = road;
	else if (map[x - 1][road_y[count][0] - 1] == wall
			&& map[x - 2][road_y[count][0] - 1] == wall)
		map[x - 1][road_y[count][0] - 1] = road;

	//	ran = rand() % 2;
	//	if(ran == 0)
	y = rand() % (road_y[count][1] - 2) + 1;
	map[road_x[count] + 2][y] = road;
	if (map[road_x[count] + 2][y + 1] == wall
			&& map[road_x[count] + 2][y + 2] == wall)
		map[road_x[count] + 2][y + 1] = road;
	else if (map[road_x[count] + 2][y - 1] == wall
			&& map[road_x[count] + 2][y - 2] == wall)
		map[road_x[count] + 2][y - 1] = road;

	//	else if(ran == 1)
	x = rand() % (MAP_WIDE - road_x[count] - 4) + road_x[count] + 3;
	map[x][road_y[count][1] - 1] = road;
	if (map[x + 1][road_y[count][1] - 1] == wall
			&& map[x + 2][road_y[count][1] - 1] == wall)
		map[x + 1][road_y[count][1] - 1] = road;
	else if (map[x - 1][road_y[count][1] - 1] == wall
			&& map[x - 2][road_y[count][1] - 1] == wall)
		map[x - 1][road_y[count][1] - 1] = road;

	//	ran = rand() % 2;
	//	if(ran == 0)
	y = rand() % (MAP_HEIGHT - road_y[count][0] - 4) + road_y[count][0] + 3;
	map[road_x[count] - 1][y] = road;
	if (map[road_x[count] - 1][y + 1] == wall
			&& map[road_x[count] - 1][y + 2] == wall)
		map[road_x[count] - 1][y + 1] = road;
	else if (map[road_x[count] - 1][y - 1] == wall
			&& map[road_x[count] - 1][y - 2] == wall)
		map[road_x[count] - 1][y - 1] = road;
	//	else if(ran == 1)
	x = rand() % (road_x[count] - 2) + 1;
	map[x][road_y[count][0] + 2] = road;
	if (map[x + 1][road_y[count][0] + 2] == wall
			&& map[x + 2][road_y[count][0] + 2] == wall)
		map[x + 1][road_y[count][0] + 2] = road;
	else if (map[x - 1][road_y[count][0] + 2] == wall
			&& map[x - 2][road_y[count][0] + 2] == wall)
		map[x - 1][road_y[count][0] + 2] = road;

	//	ran = rand() % 2;
	//	if(ran == 0)
	y = rand() % (MAP_HEIGHT - road_y[count][1] - 4) + road_y[count][1] + 3;
	map[road_x[count] + 2][y] = road;
	if (map[road_x[count] + 2][y + 1] == wall
			&& map[road_x[count] + 2][y + 2] == wall)
		map[road_x[count] + 2][y + 1] = road;
	else if (map[road_x[count] + 2][y - 1] == wall
			&& map[road_x[count] + 2][y - 2] == wall)
		map[road_x[count] + 2][y - 1] = road;
	//	else if(ran == 1)
	x = rand() % (MAP_WIDE - road_x[count] - 4) + road_x[count] + 3;
	map[x][road_y[count][1] + 2] = road;
	if (map[x + 1][road_y[count][1] + 2] == wall
			&& map[x + 2][road_y[count][1] + 2] == wall)
		map[x + 1][road_y[count][1] + 2] = road;
	else if (map[x - 1][road_y[count][1] + 2] == wall
			&& map[x - 2][road_y[count][1] + 2] == wall)
		map[x - 1][road_y[count][1] + 2] = road;

}

//フロア同士をつなぐ通路生成
void connect_floar() {

	// for(i = 0; i < FLOAR_N; i++){
	int break_wall, flag = 0;
	;
	do { //上２つの通路
		break_wall = rand() % (MAP_HEIGHT - 3) + 1;
		if (main_map[MAP_WIDE - 2][break_wall] == 0
				&& main_map[MAP_WIDE + 1][break_wall] == 0
				&& main_map[MAP_WIDE - 2][break_wall + 1] == 0
				&& main_map[MAP_WIDE + 1][break_wall + 1] == 0) {
			main_map[MAP_WIDE - 1][break_wall] = 0;
			main_map[MAP_WIDE][break_wall] = 0;
			main_map[MAP_WIDE - 1][break_wall + 1] = 0;
			main_map[MAP_WIDE][break_wall + 1] = 0;
			flag = 1;
		}
	} while (flag == 0);
	// }

	flag = 0;
	do { //左２つの通路
		break_wall = rand() % (MAP_WIDE - 3) + 1;
		if (main_map[break_wall][MAP_HEIGHT - 2] == 0
				&& main_map[break_wall][MAP_HEIGHT + 1] == 0
				&& main_map[break_wall + 1][MAP_HEIGHT - 2] == 0
				&& main_map[break_wall + 1][MAP_HEIGHT + 1] == 0) {
			main_map[break_wall][MAP_HEIGHT - 1] = 0;
			main_map[break_wall][MAP_HEIGHT] = 0;
			main_map[break_wall + 1][MAP_HEIGHT - 1] = 0;
			main_map[break_wall + 1][MAP_HEIGHT] = 0;
			flag = 1;
		}
	} while (flag == 0);

	flag = 0;
	do { //右２つの通路
		break_wall = rand() % (MAP_WIDE - 3) + 1 + MAP_WIDE;
		if (main_map[break_wall][MAP_HEIGHT - 2] == 0
				&& main_map[break_wall][MAP_HEIGHT + 1] == 0
				&& main_map[break_wall + 1][MAP_HEIGHT - 2] == 0
				&& main_map[break_wall + 1][MAP_HEIGHT + 1] == 0) {
			main_map[break_wall][MAP_HEIGHT - 1] = 0;
			main_map[break_wall][MAP_HEIGHT] = 0;
			main_map[break_wall + 1][MAP_HEIGHT - 1] = 0;
			main_map[break_wall + 1][MAP_HEIGHT] = 0;
			flag = 1;
		}
	} while (flag == 0);

	flag = 0;
	do { //下２つの通路
		break_wall = rand() % (MAP_HEIGHT - 3) + 1 + MAP_HEIGHT;
		if (main_map[MAP_WIDE - 2][break_wall] == 0
				&& main_map[MAP_WIDE + 1][break_wall] == 0
				&& main_map[MAP_WIDE - 2][break_wall + 1] == 0
				&& main_map[MAP_WIDE + 1][break_wall + 1] == 0) {
			main_map[MAP_WIDE - 1][break_wall] = 0;
			main_map[MAP_WIDE][break_wall] = 0;
			main_map[MAP_WIDE - 1][break_wall + 1] = 0;
			main_map[MAP_WIDE][break_wall + 1] = 0;
			flag = 1;
		}
	} while (flag == 0);

}

//プレイヤーの場所をマス目座標に
void update_player_place() {
	int i, j;
	for (i = 0; i < MAP_WIDE * 2; i++) {
		for (j = 0; j < MAP_HEIGHT * 2; j++) {
			if (player.x >= i * MAP_TIP_SIZE
					&& player.x < (i + 1) * MAP_TIP_SIZE
					&& player.y >= j * MAP_TIP_SIZE
					&& player.y < (j + 1) * MAP_TIP_SIZE) {
				p_map_x = i;
				p_map_y = j;
			}
		}
	}
}

//ミニマップの描画
void minmap_draw() {

	int i, j;
	for (i = 0; i < MAP_WIDE * 2; i++) {
		for (j = 0; j < MAP_HEIGHT * 2; j++) {
			SDL_Rect place = { i * MINMAP_SIZE, j * MINMAP_SIZE, MINMAP_SIZE,
					MINMAP_SIZE };
			switch (minmap[i][j]) {
			case 0:
				SDL_FillRect(minmap_surface, &place,
						SDL_MapRGB(minmap_surface->format, 0, 0, 0));

				break;
			case 1:
				SDL_FillRect(minmap_surface, &place,
						SDL_MapRGB(minmap_surface->format, 255, 255, 255));

				break;
			}
		}
	}
}

//描画サーフェスへ転送
void minmap_transfer() {
	int MINMAP_SIZE_x = 160 ,MINMAP_SIZE_y = 160;

	int minmap_center_x = p_map_x * MINMAP_SIZE - MINMAP_SIZE_x / 2;
	if (minmap_center_x < 0)
		minmap_center_x = 0;

	int minmap_center_y = p_map_y * MINMAP_SIZE - MINMAP_SIZE_y / 2;
	if (minmap_center_y < 0)
		minmap_center_y = 0;

	SDL_Rect minmap_scope_src = { minmap_center_x, minmap_center_y,
			MINMAP_SIZE_x, MINMAP_SIZE_y };
	SDL_Rect minmap_scope_dst = { WINDOW_WIDTH - MINMAP_SIZE_x, 480,
			MINMAP_SIZE_x, MINMAP_SIZE_y };
	SDL_BlitSurface(minmap_surface, &minmap_scope_src, map_buf,
			&minmap_scope_dst);

}

//プレイヤー周辺のミニマップ探索
void search_around_player() {

//	if (main_map[p_map_x][p_map_y] == 0)
//			minmap[p_map_x][p_map_y] = 1;
	int i, j;
	for (i = p_map_x - 1; i <= p_map_x + 1; i++) {
		for (j = p_map_y - 1; j <= p_map_y + 1; j++) {
			if (main_map[i][j] == road || main_map[i][j] == room)
				minmap[i][j] = 1;
		}
	}
}

//壁のマップチップの値を変える
void wall_change() {
	int i, j;
//	int change_map[MAP_WIDE * 2 + 2][MAP_HEIGHT * 2 + 2];
//	for (i = 0; i < MAP_WIDE * 2 + 2; i++) {
//		for (j = 0; j < MAP_HEIGHT * 2 + 2; j++) {
//			if (i == 0 || i == MAP_WIDE * 2 - 1 || j == 0
//					|| j == MAP_HEIGHT * 2 - 1)
//				change_map[i][j] = 1;
//			else
//				change_map[i][j] = main_map[i - 1][j - 1];
//		}
//	}
	for (i = 1; i < MAP_WIDE * 2 + 1; i++) {
		for (j = 1; j < MAP_HEIGHT * 2 + 1; j++) {
			if (get_can_walk(i, j) == wall) {
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 19;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 3;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 6;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 2;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 8;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 5;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 1;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 11;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 4;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 9;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 10;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) == road)
					main_map[i - 1][j - 1] = 7;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) == road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 13;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) == road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 14;
				if (get_can_walk(i - 1, j) == road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) != road)
					main_map[i - 1][j - 1] = 12;
				if (get_can_walk(i - 1, j) != road
						&& get_can_walk(i + 1, j) != road
						&& get_can_walk(i, j - 1) != road
						&& get_can_walk(i, j + 1) != road) {
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 19;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 28;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 29;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 27;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 30;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 25;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 24;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 31;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 32;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 26;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 23;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) == road)
						main_map[i - 1][j - 1] = 17;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) == road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 18;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) == road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 16;
					if (get_can_walk(i - 1, j - 1) == road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 15;
					if (get_can_walk(i - 1, j - 1) != road
							&& get_can_walk(i - 1, j + 1) != road
							&& get_can_walk(i + 1, j - 1) != road
							&& get_can_walk(i + 1, j + 1) != road)
						main_map[i - 1][j - 1] = 22;
				}
			}

		}
	}
}

//歩けるかどうかの値を返す
int get_can_walk(int x, int y) {
	int i, j;
	int change_map[MAP_WIDE * 2 + 2][MAP_HEIGHT * 2 + 2];
	for (i = 0; i < MAP_WIDE * 2 + 2; i++) {
		for (j = 0; j < MAP_HEIGHT * 2 + 2; j++) {
			if (i == 0 || i == MAP_WIDE * 2 - 1 || j == 0
					|| j == MAP_HEIGHT * 2 - 1)
				change_map[i][j] = 1;
			else
				change_map[i][j] = main_map[i - 1][j - 1];
		}
	}
	switch (change_map[x][y]) {
	case room:
	case road:
		return road;
	default:
		return wall;
	}
}
