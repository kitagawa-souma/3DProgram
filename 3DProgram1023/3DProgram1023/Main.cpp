#include <Windows.h>
#include "DirectGraphics.h"
#include "DirectInput.h"

// pragma commentによるlibファイルの設定
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/*
	ウィンドウプロシージャ
		戻り値：
			LRESULT
				実行結果

		引数：
			HWND
				メッセージの対象となっているウィンドウのハンドル

				HWND
					ウィンドウハンドルを保存する型

			UINT
				メッセージ情報
					クリックされた等の情報が保存されている

			WPARAM
				メッセージ付加情報その１

			LPARAM
				メッセージ付加情報その２

			付加情報にはメッセージ情報の補足情報が保存されている
				例：キーの詳細情報(大文字、小文字など)
*/
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// メッセージを何も対応しない時に実行する関数
		// 引数にはウィンドウプロシージャで渡されている引数をそのまま渡す
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

/*
	WinMain
		Windowsアプリにおいてのエントリポイント

		戻り値：
			アプリの結果

		引数：
			HINSTANCE hInstance
				アプリのインスタンスハンドル
				H => Handleの略

			HINSTANCE hPrevInstance
				古い仕様の残りなので使わない
				情報としての価値はない

			LPSTR lpCmpLine
				コマンドラインのパラメータのリスト

			INT nCmdShow
				コマンドライン引数の数
*/
int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// ウィンドウの生成はメインループが始まる前に行う
	// ウィンドウ情報を初期化する
	// ●の部分は覚えておく必要あり
	// それ以外は固定値で問題ない
	WNDCLASS window_class =
	{
		CS_HREDRAW | CS_VREDRAW,			// クラスのスタイル(CS_HREDRAW と CS_VREDRAWは横と縦の変更許可設定)
		WindowProcedure,					// ●ウィンドウプロシージャ
		0,									// 補助メモリ(0固定で問題ない)
		0,									// 補助メモリ(0固定で問題ない)
		hInstance,							// ●このプログラムのインスタンスハンドル
		LoadIcon(nullptr, IDI_APPLICATION), // アイコン画像
		LoadCursor(nullptr, IDC_ARROW),		// カーソル画像
		nullptr,							// 背景ブラシ(背景色)
		nullptr,							// メニュー名
		// TEXT() char* => wchar_t* に変換してくれる
		TEXT("WindowClass"),				// ●クラス名
	};

	// 初期化したウィンドウ情報を登録する
	/*
		RegisterClass
			ウィンドウ情報を登録する関数
		
		戻り値
			非0 => 登録成功
			0 => 登録失敗
	*/
	if (RegisterClass(&window_class) == 0)
	{
		// 失敗
		return 0;
	}

	int width = 640;
	int height = 480;

	// ウィンドウ作成
	/*
		実行結果
			成功 => ウィンドウのハンドル
			失敗 => nullptr
	*/
	HWND window_handle = CreateWindow(
		// ●登録しているウィンドウクラス構造体の名前
		/*
			RegisterWindow関数で登録した構造体に設定したクラス名を指定する
		*/
		TEXT("WindowClass"),
		// ●ウィンドウ名(タイトルバーに表示される文字列)
		TEXT("ウィンドウ生成サンプル"),
		// ウィンドウスタイル
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// ●表示位置
		/*
			ウィンドウを表示する位置を指定する
			(CW_USEDEFAULT => OS任せ)
		*/
		CW_USEDEFAULT,	// x軸
		CW_USEDEFAULT,	// y軸
		// ●サイズ => ウィンドウのサイズ
		width,
		height,
		// 親ハンドル
		NULL,
		// メニューハンドル
		NULL,
		// ●インスタンスハンドル
		// WinMainの第一引数を使用
		hInstance,
		// WM_CREATEメッセージでlpparamに渡したい値
		NULL);

	RECT window_rect;

	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		// 失敗
		return 0;
	}

	RECT client_rect;

	if (GetClientRect(window_handle, &client_rect) == false)
	{
		// 失敗
		return 0;
	}

	// フレームサイズの算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// リサイズ用サイズの算出
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	// ウィンドウサイズの更新
	SetWindowPos(
		window_handle,
		nullptr,
		// 表示位置
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// ●リサイズの横幅
		resize_width,
		// ●リサイズの縦幅
		resize_height,
		// オプションの指定
		SWP_NOMOVE
	);

	// ウィンドウ表示
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	if (InitDirectInput() == false)
	{
		return 0;
	}

	if (LoadTexture(TextureID::TexIDBackground) == false)
	{
		return 0;
	}

	LoadXFile(TEXT("Witchwait.X"));

	float pos_x = 0.0f;
	float pos_y = 0.0f;
	float pos_z = 0.0f;
	float angle = 0.0f;

	// メインループ
	while (true)
	{
		if (IsKeyPushed(DIK_ESCAPE))
		{
			return 0;
		}
		// フレーム管理
		/*
			フレーム管理はループが一定の周期になるように管理する
			例えば60FPSでゲームが進行するなら
			1回のループは約0.016秒で実行する

			※DirectXが60FPSでフレーム管理を自動で行うので
			　今回はDirectXに管理は任せる
		*/

		// メッセージ対応
		/*
			Windows(OS)から送られてくるメッセージの
			対応を行う必要がある

			メッセージの例
				ウィンドウをクリックをした
				ウィンドウを移動させた
				ウィンドウの×ボタンをクリックした
		*/
		MSG message;
		/*
			PeekMessage
				メッセージの有無にかかわらず次の処理を実行する

				戻り値：
					true => メッセージ有り
					false => メッセージ無し

				第一引数：
					受信したメッセージ情報

				第二～第五引数：
					固定値で問題ない
		*/
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// 受信したデータを翻訳する
				TranslateMessage(&message);

				// ウィンドウプロシージャに送信する
				DispatchMessage(&message);
			}
		}
		else
		{
			UpdateDirectInput();

			if (IsKeyHeld(DIK_LEFTARROW))
			{
				angle -= 3.0f;
			}
			else if (IsKeyHeld(DIK_RIGHTARROW))
			{
				angle += 3.0f;
			}
			if (IsKeyHeld(DIK_UPARROW))
			{
				float vec_z = cosf(D3DXToRadian(angle));
				float vec_x = sinf(D3DXToRadian(angle));
				float speed = 2.0f;

				pos_x += vec_x * speed;
				pos_z += vec_z * speed;
			}
			// ゲームに関連する処理
			//	ゲーム処理と描画処理を実行する

			//カメラの設定
			//SetUpView();

			UpdateTPSCamera(pos_x, pos_y, pos_z, angle);

			StartDrawing();

			//プロジェクションの設定
			SetUpProjection();

			//3Dオブジェクトの描画
			Draw3DPorigon();

			//XFileの描画
			DrawXFile(pos_x, pos_y, pos_z, angle);

			//2Dオブジェクトの描画
			//DrawPorigon();

			//DrawPorigonWithTriangleList();

			//DrawPorigonWithTriangleStrip();

			//DrawPorigonWithTriangleFan(TextureID::TexIDBackground);

			FinishDrawing();
		}
	}

	// 解放
	ReleaseTexture();
	ReleaseDirectGraphics();
	ReleaseDirectInput();

	return 0;
}
