/*
 * item.c
 *
 *  Created on: 2016/08/28
 *      Author: c501506038
 */



//当たり判定なし


#include "header.h"
#include "player_header.h"


void item_type_blit(int item_ID,SDL_Rect *pos);
void item_surface_create();
void KeyItem_spawn(int client_num);



int item_place_flag[MAP_WIDE*2][MAP_HEIGHT*2];	//各マップチップにアイテムがあるか否か

int Xchip_num,Ychip_num;

SDL_Surface *item_surf; //アイテムを描画するやつ
SDL_Surface *key_item_chip;

extern SDL_Rect MainCamera;


//初期化
void item_initialize()
{

	Xchip_num=MAP_WIDE*2;
	Ychip_num=MAP_HEIGHT*2;

	item_surf = SDL_CreateRGBSurface(SDL_HWSURFACE,Xchip_num * MAP_TIP_SIZE, Ychip_num* MAP_TIP_SIZE, 32, 0, 0, 0, 0);
	SDL_SetColorKey(item_surf, SDL_SRCCOLORKEY, SDL_MapRGB(item_surf->format, 0, 0, 0));

	key_item_chip = IMG_Load("pictures/key_item_chip.png");
	if(key_item_chip==NULL){
		printf("item_img load error!");
		exit(1);
	}

}

//再初期化
void item_reinitialize()
{
	KeyItem_spawn(4);
	item_surface_create();
}


//計算等
void item_update(){

}

//描画
void item_draw()
{
	SDL_BlitSurface(item_surf,&MainCamera,window,NULL);
}

//リモコン操作
void item_wii(){

}

//終了処理（あれば）
void item_finalize(){

}





/* キーアイテムをクライアントの数だけ配置する関数
 * client_num :クライアント数
 */
void KeyItem_spawn(int client_num)
{
	int i,x,y;

	srand((unsigned)time(NULL));

	for(i=0; i<client_num; i++)
	{
		do{ //配置できる場所になるまでループ
			x = rand() % Xchip_num;
			y = rand() % Ychip_num;
		}while( map_value(x,y)!=0 && item_place_flag[x][y] !=0 );

		//設定された場所にIDを格納
		item_place_flag[x][y] = i+1;
	}

}




//アイテムの描かれたサーフェスを作成する
void item_surface_create()
{
	int i,j,ID;
	SDL_Rect map_pos = {0,0,32,32};


	for(i=0; i<Xchip_num; i++)
		for(j=0; j<Ychip_num; j++)
		{
			if(item_place_flag[i][j] != 0)
			{
				ID = item_place_flag[i][j];
				map_pos.x = i*MAP_TIP_SIZE;
				map_pos.y = j*MAP_TIP_SIZE;

				item_type_blit(ID,&map_pos);
			}

		}

}




//引数のIDに応じたアイテムをitem_surfの引数posの場所に描画する
void item_type_blit(int item_ID,SDL_Rect *pos)
{
	SDL_Rect img_pos={0,0,32,32};

	if( 1<= item_ID && item_ID<=10)
	{//キーアイテムの描画
		img_pos.x= (item_ID-1) *MAP_TIP_SIZE;
		SDL_BlitSurface(key_item_chip,&img_pos,item_surf,pos);
	}
}



//引数の座標のアイテムのIDを調べ、アイテムがあれば効果を発動する
void item_hit_judge(int x,int y)
{
	int id;
	id = item_place_flag[x/MAP_TIP_SIZE][y/MAP_TIP_SIZE];

	if( id != 0 )
	{
		item_effect(id);
		item_place_flag[x/MAP_TIP_SIZE][y/MAP_TIP_SIZE] = 0;
	}

}

void item_effect(int item_ID)
{
	if( 1<=item_ID && item_ID<=10 )
	{ //キーアイテムの効果

	}
}

