#include "header.h"

/*
	使用例
	ループ内｛
	gpUpdateKey();		宣言

	if(key[2] == 1){	2ボタンが押された瞬間
		flag = 2;	フラグを2に変える
	}

	if(key[3] >= 1){	aボタンが押されている間
		flag = 2;	
	}
*/
wiimote_t wiimote = WIIMOTE_INIT; 		//Wiiリモコン変数
int key[11];



/*
 * Wiiリモコンの初期化を行う
 * 引数：wiiリモコンの変数へのアドレス、main関数の引数に渡されている値
 */
int  wii_initialize(int argc, char* argv[])
{


	if (argc < 2)
	{	// Wiiリモコン識別情報がコマンド引数で与えられなければ
		printf("WiiリモコンのIDを実行時に与えてください\n");
		return 0;
	}

	// Wiiリモコンの接続（１つのみ）
	if (wiimote_connect(&wiimote, argv[1]) < 0)
	{	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
		printf("指定されたwiiリモコンに接続できませんでした: %s\n", wiimote_get_error());
		return 0;
	}

	wiimote.led.one  = 1;	// WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）

	// センサからのデータを受け付けるモードに変更
	wiimote.mode.acc = 1;


	return wiimote_is_open(&wiimote);

}

int gpUpdateKey(wiimote_t wii)
{//key判定のため

	//Wiiリモコンの状態を更新
	if ( wiimote_update(&wiimote) < 0 )
		{
			wiimote_disconnect(&wiimote);
			printf("wiiリモコンの接続が切れました\n");
			return 0;
		}

	char wiikey[] = {
	wiimote.keys.bits, //0
	wiimote.keys.one,  //1
	wiimote.keys.two,  //2
	wiimote.keys.a,    //3
	wiimote.keys.b,    //4
	wiimote.keys.left, //5
	wiimote.keys.right,//6
	wiimote.keys.up,   //7
	wiimote.keys.down, //8
	wiimote.keys.home, //9
	wiimote.keys.plus, //10
	wiimote.keys.minus //11
	};

	int i;

	for(i=0; i<12; i++ ){
		if( wiikey[i] != 0 ){
			key[i]++;
		} else {
			key[i] = 0;
		}
	}
	return 1;
}

void Wii_finalize()
{
	wiimote_disconnect(&wiimote);		// Wiiリモコンとの接続を解除


}
