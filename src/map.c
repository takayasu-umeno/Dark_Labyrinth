/*
 * map.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */


#include "header.h"

#define MAP_WIDE 40
#define MAP_HEIGHT 40
#define MAP_TIP_SIZE 5
#define FLOAR_N 4

enum{road, kabe};

SDL_Event event; // SDLによるイベントを検知するための構造体

static int main_map[MAP_WIDE * 2][MAP_HEIGHT * 2];
static int map[MAP_WIDE][MAP_HEIGHT];
static int road_x[FLOAR_N], road_y[FLOAR_N][2];
static int move_x = 0, move_y = 0;

//初期化
void map_initialize(){

	int i,j;
	for(i = 0; i < MAP_WIDE; i++){
		for(j = 0; j < MAP_HEIGHT; j++){
			map[i][j] = (i + j) % 2;
		}
	}

	srand((unsigned int) time(NULL)); //乱数初期化

	map_create();//マップ生成
}


//再初期化
void map_reinitialize(){

}


//計算等
void map_update(){
 
}

//描画
void map_draw(){

	int i,j;
	for(i = 0; i < MAP_WIDE * 2; i++){
		for(j = 0; j < MAP_HEIGHT * 2; j++){
			SDL_Rect place = {i * MAP_TIP_SIZE + move_x, j * MAP_TIP_SIZE + move_y, MAP_TIP_SIZE, MAP_TIP_SIZE};
			switch(main_map[i][j]){
			case road:
				SDL_FillRect(window, &place, SDL_MapRGB(window->format, 255, 255, 255));

				break;
			case kabe:
				SDL_FillRect(window, &place, SDL_MapRGB(window->format, 0, 0, 255));

				break;

			}
		}
	}
}


//リモコン操作
void map_wii(){
	if (SDL_PollEvent(&event)) {
		switch (event.type) { // 押されたキーごとに処理
		case SDL_QUIT:
			// SDLの利用を終了する時
			SDL_Quit();
			exit(0);
			break;
		case SDL_KEYDOWN:	// キーボードのキーが押された時
		  // 押されたキーごとに処理
		/*  switch(event.key.keysym.sym){
		  case SDLK_ESCAPE:	// Escキーが押された時
		    printf("ESCAPE key -> EXIT.\n");
		    SDL_Quit();
		    exit(0);
		    break;
		  
		  case SDLK_UP:
		    move_y -= MAP_TIP_SIZE;
		    break;
		  case SDLK_DOWN:
		    move_y += MAP_TIP_SIZE;
		    break;
		  case SDLK_LEFT:
		    move_x -= MAP_TIP_SIZE;
		    break;
		  case SDLK_RIGHT:
		    move_x += MAP_TIP_SIZE;
		    break;
		  }*/
		  break;
		}
	}

}

//終了処理（あれば）
void map_finalize(){

}

//マップの値を返す
int map_value(int x, int y){

	return main_map[x][y];
}

//マップの生成
void map_create(){

  int count;
  for(count = 0; count < FLOAR_N; count++){
	road_x[count] = rand() % (MAP_WIDE - 10) + 5;
	road_y[count][0] = rand() % (MAP_HEIGHT - 10) + 5;
	road_y[count][1] = rand() % (MAP_HEIGHT - 10) + 5;

	int i,j;
	for(i = 0; i < MAP_WIDE; i++){
		for(j = 0; j < MAP_HEIGHT; j++){
			if(i == 0 || j == 0 || i == MAP_WIDE - 1 || j == MAP_HEIGHT - 1)
				map[i][j] = kabe;//外周の壁生成
			else if(i == road_x[count] || i == road_x[count] + 1)
				map[i][j] = road;//通路生成
			else if((j == road_y[count][0] || j == road_y[count][0] + 1) && i > 0 && i < road_x[count])
				map[i][j] = road;//通路生成
			else if((j == road_y[count][1] || j == road_y[count][1] + 1) && i > road_x[count] && i < MAP_WIDE)
				map[i][j] = road;//通路生成
			else if(i == road_x[count] - 1 || i == road_x[count] + 2)
				map[i][j] = kabe;//通路周りの壁生成
			else if((j == road_y[count][0] - 1 || j == road_y[count][0] + 2) && i > 0 && i < road_x[count])
				map[i][j] = kabe;//通路周りの壁生成
			else if((j == road_y[count][1] - 1 || j == road_y[count][1] + 2) && i > road_x[count] && i < MAP_WIDE)
				map[i][j] = kabe;//通路周りの壁生成

			else
				map[i][j] = road;
		}
	}

	//	int ran = rand() % 2;
	//	if(ran == 0)
		map[road_x[count] - 1][rand() % (road_y[count][0] - 2) + 1] = road;
	//	else if(ran == 1)
		map[rand() % (road_x[count] - 2) + 1][road_y[count][0] - 1] = road;

	//	ran = rand() % 2;
	//	if(ran == 0)
		map[road_x[count] + 2][rand() % (road_y[count][1] - 2) + 1] = road;
	//	else if(ran == 1)
		map[rand() % (MAP_WIDE - road_x[count] - 4) + road_x[count] + 3][road_y[count][1] - 1] = road;


	//	ran = rand() % 2;
	//	if(ran == 0)
		map[road_x[count] - 1][rand() % (MAP_HEIGHT - road_y[count][0] - 4) + road_y[count][0] + 3] = road;
	//	else if(ran == 1)
		map[rand() % (road_x[count] - 2) + 1][road_y[count][0] + 2] = road;


	//	ran = rand() % 2;
	//	if(ran == 0)
		map[road_x[count] + 2][rand() % (MAP_HEIGHT - road_y[count][1] - 4) + road_y[count][1] + 3] = road;
	//	else if(ran == 1)
		map[rand() % (MAP_WIDE - road_x[count] - 4) + road_x[count] + 3][road_y[count][1] + 2] = road;


	for(i = 0; i < MAP_WIDE; i++){
		for(j = 0; j < MAP_HEIGHT; j++){
		  main_map[i + count % 2 * MAP_WIDE][j + count / 2 * MAP_HEIGHT] = map[i][j]; 
		}
	}
  }	
  // for(i = 0; i < FLOAR_N; i++){
    int break_wall, flag = 0;;
    do{//上２つの通路
      break_wall  = rand() % (MAP_HEIGHT - 3) + 1;
      if(main_map[MAP_WIDE - 2][break_wall] == 0 && main_map[MAP_WIDE + 1][break_wall] == 0 && main_map[MAP_WIDE - 2][break_wall + 1] == 0 && main_map[MAP_WIDE + 1][break_wall + 1] == 0){
	main_map[MAP_WIDE - 1][break_wall] = 0;
	main_map[MAP_WIDE][break_wall] = 0;
	main_map[MAP_WIDE - 1][break_wall + 1] = 0;
	main_map[MAP_WIDE][break_wall + 1] = 0;
	flag = 1;
      }
    }while(flag == 0);
    // }
  
    flag = 0;
    do{//左２つの通路
      break_wall  = rand() % (MAP_WIDE - 3) + 1;
      if(main_map[break_wall][MAP_HEIGHT - 2] == 0 && main_map[break_wall][MAP_HEIGHT + 1] == 0 && main_map[break_wall + 1][MAP_HEIGHT - 2] == 0 && main_map[break_wall + 1][MAP_HEIGHT + 1] == 0){
	main_map[break_wall][MAP_HEIGHT - 1] = 0;
	main_map[break_wall][MAP_HEIGHT] = 0;
	main_map[break_wall + 1][MAP_HEIGHT - 1] = 0;
	main_map[break_wall + 1][MAP_HEIGHT] = 0;
	flag = 1;
      }
    }while(flag == 0);

    flag = 0;
    do{//右２つの通路
      break_wall  = rand() % (MAP_WIDE - 3) + 1 + MAP_WIDE;
      if(main_map[break_wall][MAP_HEIGHT - 2] == 0 && main_map[break_wall][MAP_HEIGHT + 1] == 0 && main_map[break_wall + 1][MAP_HEIGHT - 2] == 0 && main_map[break_wall + 1][MAP_HEIGHT + 1] == 0){
	main_map[break_wall][MAP_HEIGHT - 1] = 0;
	main_map[break_wall][MAP_HEIGHT] = 0;
	main_map[break_wall + 1][MAP_HEIGHT - 1] = 0;
	main_map[break_wall + 1][MAP_HEIGHT] = 0;
	flag = 1;
      }
    }while(flag == 0);


    flag = 0;
    do{//下２つの通路
      break_wall  = rand() % (MAP_HEIGHT - 3) + 1 + MAP_HEIGHT;
      if(main_map[MAP_WIDE - 2][break_wall] == 0 && main_map[MAP_WIDE + 1][break_wall] == 0 && main_map[MAP_WIDE - 2][break_wall + 1] == 0 && main_map[MAP_WIDE + 1][break_wall + 1] == 0){
	main_map[MAP_WIDE - 1][break_wall] = 0;
	main_map[MAP_WIDE][break_wall] = 0;
	main_map[MAP_WIDE - 1][break_wall + 1] = 0;
	main_map[MAP_WIDE][break_wall + 1] = 0;
	flag = 1;
      }
    }while(flag == 0);
}
