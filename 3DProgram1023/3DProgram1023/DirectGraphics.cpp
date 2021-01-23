#include "DirectGraphics.h"

//XFileデータ保存用の構造体
/*
	なぜ、ヘッダファイルではなく、ソースファイルに構造体を宣言したか？
*/
struct XFile
{
	DWORD MaterialNum;				//!< メッシュ数
	LPD3DXMESH Meshes;				//!< メッシュ
	LPD3DXBUFFER Materials;			//!< メッシュマテリアル
	LPDIRECT3DTEXTURE9* Textures;	//!< テクスチャデータ
};

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// テクスチャデータ
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

XFile g_XFile;

/*
	TexIDUnityChanIdle,	// UnityChanIdle.png
	TexIDUnityChanRun,	// UnityChanRun.png
	TexIDUnityChanJump,	// UnityChanJump.png
*/
LPCWSTR g_TextureNameList[] =
{
	TEXT("Image_3692d50.jpg"),
};


bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9生成関数

			引数：
				DirectXのバージョン(D3D_SDK_VERSION固定)

			戻り値：
				成功：
					IDirect3D9*
				失敗：
					nullptr
	*/
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// 初期化失敗
		return false;
	}

	// Deviceの情報を設定するための構造体
	D3DPRESENT_PARAMETERS parameters;

	// 指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数
	parameters.BackBufferCount = 1;

	// バックバッファのフォーマット
	// D3DFMT_UNKNOWN => 知りません
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// ウィンドウモード指定
	/*
		true => ウィンドウモード

		false => フルスクリーンモード
					※フルスクリーンは別途情報を指定する必要あり
	*/
	parameters.Windowed = true;

	/*
		バックバッファとフロントバッファの切り替え方法

			D3DSWAPEFFECT_DISCARD => 自動設定
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
		Zバッファ法を使う場合PresentParametersで
		使うことを通知する必要あり
	*/
	//ZバッファON
	//EnableAutoDepthStencil => デプス、ステンシルバッファの使用フラグ
	parameters.EnableAutoDepthStencil = TRUE;
	//AutoDepthStencilFormat => デプス、ステンシルバッファのフォーマット
	//D3DFMT_D16 => デプスバッファの1データを16ビットで作成する
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		// ディスプレイアダプタの種類
		D3DADAPTER_DEFAULT,
		// デバイスの種類
		D3DDEVTYPE_HAL,
		// デバイスが使用するウィンドウのハンドル
		window_handle,
		// デバイス制御の方法
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Deviceの保存変数指定
		&g_Device
	);

	/*
		FAILED => HRESULTの結果を判定してくれるマクロ
					trueなら失敗
					falseなら成功
	*/
	if (FAILED(result))
	{
		return false;
	}
/*
	// 成功判定
	if (SUCCEEDED(result))
	{

	}
*/
	return true;
}

void ReleaseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => 解放関数
		g_Device->Release();
		// 解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		// Release => 解放関数
		g_Interface->Release();
		// 解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}


void StartDrawing()
{
	//テクスチャブレンドの設定(以下の設定では頂点カラーとテクスチャのαのブレンドを許可している)
	/*
		SetTextureStageState
			第一：ステージのID
			第二：設定の種類
			第三：設定の情報
	*/
	/*
		SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			第一：0
			第二：D3DTSS_ALPHAOP <= α値の合成に関する設定をする
			第三：D3DTOP_MODULATE <= 合成は乗算を使用する
	*/
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			第一：0
			第二：D3DTSS_COLORARG1 <= 乗算の値の設定
			第三：D3DTA_TEXTURE <= テクスチャカラーを使用する
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			第一：0
			第二：D3DTSS_COLORARG2 <= 乗算の値の設定
			第三：D3DTA_DIFFUSE <= Stageに送信されてきた合成カラー
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	//α値を有効にする
	/*
		SetRenderState <= 描画の設定を変更する
			第一：変更したい設定の種類
			第二：設定の詳細
	*/
	/*
		SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			第一：D3DRS_ALPHABLENDENABLE	<= αブレンドの設定を変更する
			第二：true						<= 有効(falseで無効)
	*/
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	/*
		SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			第一：D3DRS_SRCBLEND			<= バッファに送信される色の設定
			第二：D3DBLEND_SRCALPHA			<= αをそのまま使う
	*/
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	/*
		SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			第一：D3DRS_DESTBLEND			<= バッファの色の設定
			第二：D3DBLEND_INVSRCALPHA		<= SRC(バッファ煮送信される色)のアルファを利用する(1 - srcalpha)
	*/
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ライティングを無効にする
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zバッファの使用をONにする
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// バックバッファをクリアする
	g_Device->Clear(
		0,										// 0固定
		nullptr,								// nullptr固定
		//D3DCLEAR_ZBUFFER => Zバッファもクリア対象に加える
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// D3DCLEAR_TARGET固定
		D3DCOLOR_XRGB(255, 255, 0),				// 塗りつぶす色
		1.0f,									// 1.0f固定
		0										// 0固定
	);

	// DirectXで描画を開始する
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirecXの描画を終了する
	g_Device->EndScene();

	// バックバッファをフロントバッファに転送する
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 1.0f, 0xffffff },
		{ 500.0f, 300.0f, 0.0f, 1.0f, 0xffffff },
		{ 50.0f, 300.0f, 0.0f, 1.0f, 0xffffff },
	};

	// DirectXに頂点構造情報を通知
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			// ポリゴンの作り方
		1,							// ポリゴンの数
		vertices,					// 頂点リスト
		sizeof(CustomVertex)		// 1頂点のサイズ
	);
}

void DrawPorigonWithTriangleList()
{
	CustomVertex vertices[] =
	{
		// 1つ目
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0xff0000 },

		// 2つ目
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		2,						// 描画するポリゴン数 => 2
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleStrip()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 0.0f, 0.0f, 1.0f, 0xffffff },
		{ 350.0f, 0.0f, 0.0f, 1.0f, 0x808080 },
		{ 300.0f, 200.0f, 0.0f, 1.0f, 0x808080 },
		{ 350.0f, 200.0f, 0.0f, 1.0f, 0x000000 }
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// D3DPT_TRIANGLESTRIPでポリゴン作成
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleFan(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertexTex vertices[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0x80ffffff, 0.0f, 0.0f },
		{ 250.0f, 0.0f, 0.0f, 1.0f, 0x80ffffff, 1.0f, 0.0f },
		{ 250.0f, 450.0f, 0.0f, 1.0f, 0x80ffffff, 1.0f, 1.0f },
		{ 0.0f, 450.0f, 0.0f, 1.0f, 0x80ffffff, 0.0f, 1.0f },
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	/*
		DirectX(Computer)に今回のポリゴン描画で使用するテクスチャを教える
	*/
	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void SetUpView()
{
	D3DXMATRIX matrix;//カメラ情報を保存するための行列
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//LH => Left Hand => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH( //カメラ情報を持つ行列を作成する
		&matrix,
		&camera_pos,
		&eye_pos,
		&up_vec
	);

	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

void SetUpProjection()
{
	D3DXMATRIX matrix;//プロジェクション情報を保存するための行列
	float aspect = 640.0f / 480.0f;//アスペクト比(ウインドウサイズの横/縦)

	//Perspective => 透視投影
	D3DXMatrixPerspectiveFovLH(
		&matrix,		//結果保存用の行列の設定
		D3DXToRadian(60.0f),
		aspect,			//アスペクト比
		0.1f,			//視錐台のnear値
		1000.0f			//視錐台のfar値
	);

	//SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void Draw3DPorigon()
{
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	CustomVertex3D vertices[]
	{
		{-1.0f, -1.0f, 0.0f, 0xffff0000},
		{0.0f, 1.0f, 0.0f, 0xffff0000},
		{1.0f, 1.0f, 0.0f, 0xffff0000}
	};

	g_Device->SetFVF(FVF_3D_VERTEX);

	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9のデバイス
		g_TextureNameList[tex_id],	// ファイル名
		&g_Textures[tex_id]			// 読み込まれたテクスチャ情報が保存される変数
	);

	// FAILED => HRESULT型の変数を指定して、その値が失敗していたらtrue
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMax; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				//ファイル名
		D3DXMESH_SYSTEMMEM,		//メッシュ作成のオプション(D3DXMESH_SYSTEMMEM固定)
		g_Device,				//DirectGraphicsのデバイス
		nullptr,				//現状はnullptrで良し
		&g_XFile.Materials,		//マテリアル保存用
		nullptr,				//現状はnullptrで良し
		&g_XFile.MaterialNum,	//マテリアルの数保存用
		&g_XFile.Meshes			//メッシュデータ保存用
	);
	if (FAILED(hr))
	{
		return false;
	}

	//マテリアルの数だけテクスチャを保存できるようにする
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];

	//バッファの先頭ポインタをD3DXMATERIALにキャストして保存
	/*
		Materialsの型を確認するとわかるが、MaterialsはLPD3DXBUFFER型になっている
		Bufferという汎用的にデータを保存する方にマテリアルが保存されているため、
		Material本来の型に戻す必要がある
		
		GetBufferPointerを使用すればBufferの先頭のポインタが取得できる
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (materials[i].pTextureFilename != nullptr)
		{
			LPSTR char_file_name = materials[i].pTextureFilename;

			//LPSTR(char*) => LPWSTR(wchar_t*)変換
			wchar_t wtext[256];
			size_t ret;
			mbstowcs_s(&ret, wtext, char_file_name, strlen(char_file_name) + 1);
			LPWSTR file_name = wtext;

			D3DXCreateTextureFromFile(
				g_Device,
				file_name,
				&g_XFile.Textures[i]
			);
		}
		else
		{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void DrawXFile(float pos_x, float pos_y, float pos_z, float angle)
{
	/*float pos_x = 0.0f;
	float pos_y = 0.0f;
	float pos_z = 0.0f;*/

	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float scale_z = 1.0f;

	float rad_x = D3DXToRadian(0.0f);
	float rad_y = D3DXToRadian(angle);
	float rad_z = D3DXToRadian(0.0f);

	D3DXMATRIX matrix;
	D3DXMATRIX scale_matrix;
	D3DXMATRIX trans_matrix;

	D3DXMATRIX rotate_x_matrix;
	D3DXMATRIX rotate_y_matrix;
	D3DXMATRIX rotate_z_matrix;

	D3DXMatrixIdentity(&matrix);

	//拡縮
	D3DXMatrixScaling(&scale_matrix, scale_x, scale_y, scale_z);

	//回転
	D3DXMatrixRotationX(&rotate_x_matrix, rad_x);
	D3DXMatrixRotationY(&rotate_y_matrix, rad_y);
	D3DXMatrixRotationZ(&rotate_z_matrix, rad_z);

	//移動
	D3DXMatrixTranslation(&trans_matrix, pos_x, pos_y, pos_z);

	matrix *= scale_matrix;

	matrix *= trans_matrix;

	matrix *= rotate_x_matrix;
	matrix *= rotate_y_matrix;
	matrix *= rotate_z_matrix;

	g_Device->SetTransform(D3DTS_WORLD, &matrix);

	//描画処理
	/*
		XFileの描画はマテリアル単位で行う
		各マテリアル単位でマテリアルの設定とそのマテリアルで使われているテクスチャの設定を行う
		その設定が完了したらメッシュを描画する
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		//マテリアルの設定
		g_Device->SetMaterial(&materials[i].MatD3D);
		//テクスチャの設定
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		//メッシュを描画
		g_XFile.Meshes->DrawSubset(i);
	}
}

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle)
{
	// ターゲットの向きベクトルを求める
	float vec_x = sinf(D3DXToRadian(angle));
	float vec_z = cosf(D3DXToRadian(angle));

	// ベクトルの反転
	vec_x *= -1.0f;
	vec_z *= -1.0f;

	// ベクトルに距離を掛けて長さを変化させる
	float distance = 200.0f;
	vec_x *= distance;
	vec_z *= distance;

	// ターゲットの位置にベクトルを加算した値を新しいカメラの座標とする
	float new_campos_x = target_pos_x + vec_x;
	float new_campos_y = target_pos_y;
	float new_campos_z = target_pos_z + vec_z;

	D3DXMATRIX matrix; // カメラ情報を保存するための行列
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(new_campos_x, new_campos_y, new_campos_z);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(target_pos_x, target_pos_y, target_pos_z);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			カメラ情報をもつ行列を作成する
	*/
	// L => Left H => Hand LH => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH(
		&matrix,		// 結果保存用行列の指定
		&camera_pos,	// カメラ座標
		&eye_pos,		// 注視点座標
		&up_vec			// ワールド座標上におけるカメラの上向きのベクトル
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

void ReleaseXFile()
{
	//テクスチャの開放
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	//テクスチャ配列も開放する
	delete[] g_XFile.Textures;

	//マテリアルの開放
	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}

	//メッシュの開放
	if (g_XFile.Meshes != nullptr)
	{
		g_XFile.Meshes->Release();
		g_XFile.Meshes = nullptr;
	}
}
