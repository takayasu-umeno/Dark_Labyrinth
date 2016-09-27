#include"header.h"

/*****************************************************************************************************************************/
SDL_Thread *wii_thr, *update_thr;

void draw();//描画
int wii_thread();//マルチスレッド
int update_thread();//マルチスレッド
void initialize();//初期化
void reinitialize();//再初期化
int Update_UpDate_FPS();
int Wait_Update_FPS();
void finalize();//終了処理
/*****************************************************************************************************************************/
struct FPS {
	int FrameCount; //経過フレーム
	int FPS; //設定FPS
	unsigned int mCount; //カウンタ
	unsigned int mStartTime; //測定開始時刻
	int N; //平均を取るサンプル数
	int WaitTime; //待機時間のサンプリングフレーム間での合計
	double WaitTime_Ave; //待機時間の平均
	double mFps; //平均FPS
};

struct FPS UpdateFPS;

/****グローバル変数*************************************************************************************************************************/
int layer = play;		//レイヤー
SDL_Surface *window; 	// ウィンドウ（画像）データへのポインタ
wiimote_t wiimote; 		//Wiiリモコン変数
/*****************************************************************************************************************************/

int main(int argc, char* argv[])
{


	initialize(); //初期化
	wii_initialize(&wiimote,argc,argv);

	wii_thr = SDL_CreateThread(wii_thread, NULL); //マルチスレッド生成（描画以外の処理）
	update_thr = SDL_CreateThread(update_thread, NULL); //マルチスレッド生成（描画以外の処理）
	while (1) {
		draw();
	}

	finalize();//終了処理

	return 0;
}


//マルチスレッド
int wii_thread() {

	while (1) {
		switch (layer) {
		case tytle:
			tytle_wii();
			break;
		case loading:
			loading_wii();
			break;
		case play:
			player_wii();
			map_wii();
			break;
		case result:
			result_wii();
			break;
		case final_result:
			result_wii();
			break;
		}
	}
	return 0;
}

//マルチスレッド
int update_thread() {

	while (1) {
		UpdateFPS.FrameCount++;
		Update_UpDate_FPS();
		Wait_Update_FPS();
		switch (layer) {
		case tytle:
			tytle_update();
			break;
		case loading:
			loading_update();
			break;
		case play:
			map_update();
			enemy_update();
			playgamen_update();
			player_update();
			break;
		case result:
			result_update();
			break;
		case final_result:
			result_update();
			break;
		}
	}
	return 0;
}

//
void draw(){
	switch (layer) {
	case tytle:
		tytle_draw();
		break;
	case loading:
		loading_draw();
		break;
	case play:
		map_draw();
		enemy_draw();
		playgamen_draw();
		player_draw();
		break;
	case result:
		result_draw();
		break;
	case final_result:
		result_draw();
		break;
	}

	SDL_Flip (window);

}


//初期化
void initialize() {
	UpdateFPS.FPS = 60;
	UpdateFPS.FrameCount = 0;
	UpdateFPS.mCount = 0;
	UpdateFPS.mStartTime = 0;
	UpdateFPS.N = 60;
	UpdateFPS.WaitTime = 0;
	UpdateFPS.WaitTime_Ave = 0;
	UpdateFPS.mFps = 0;
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	SDL_Init(SDL_INIT_EVERYTHING);

	if ((window = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,
			SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	playgamen_initialize();
	final_result_initialize();
	result_initialize();
	loading_initialize();
	item_initialize();
	map_initialize();
	enemy_initialize();
	player_initialize();
	tytle_initialize();

	reinitialize();//再初期化
}
//再初期化
void reinitialize() {
	playgamen_reinitialize();
	final_result_reinitialize();
	result_reinitialize();
	loading_reinitialize();
	item_reinitialize();
	map_reinitialize();
	enemy_reinitialize();
	player_reinitialize();
	tytle_reinitialize();
}


int Update_UpDate_FPS() {
	if (UpdateFPS.mCount == 0) {
		UpdateFPS.mStartTime = SDL_GetTicks();
	}
	if (UpdateFPS.mCount == UpdateFPS.N) {
		unsigned int t = SDL_GetTicks();
		UpdateFPS.mFps = 1000.
				/ ((t - UpdateFPS.mStartTime) / (double) UpdateFPS.N);
		UpdateFPS.WaitTime_Ave = UpdateFPS.WaitTime / (double) UpdateFPS.N;
		UpdateFPS.mCount = 0;
		UpdateFPS.WaitTime = 0;
		UpdateFPS.mStartTime = t;
	}
	UpdateFPS.mCount++;
	return 1;
}

int Wait_Update_FPS() {
	if (UpdateFPS.FPS > 0) {
		int tookTime = SDL_GetTicks() - UpdateFPS.mStartTime; //かかった時間
		int waitTime = UpdateFPS.mCount * 1000.f / UpdateFPS.FPS - tookTime; //待つべき時間
		if (waitTime > 0)
			UpdateFPS.WaitTime += waitTime;
		else
			UpdateFPS.WaitTime = waitTime;
		if (waitTime > 0) {
			SDL_Delay(waitTime); //待機
		}
	}
	return 0;
}

//終了処理
void finalize(){

	playgamen_finalize();
	final_result_finalize();
	result_finalize();
	loading_finalize();
	item_finalize();
	map_finalize();
	enemy_finalize();
	player_finalize();
	tytle_finalize();
}
