#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphicsで必要な関数が定義されているヘッダファイル
#include <d3d9.h>
#include <d3dx9.h>

struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

struct CustomVertexTex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;

	float Tu;
	float Tv;
};
/*
	DrawPrimitiveUpなどのDirectXのCustyomVertexを使用した関数について
		実際は3D描画を行っている
		3D描画なので本来はワールド座標変換、ビュー座標変換、プロジェクション座標変換のための
		設定を行う必要がある
		
		しかし、2D描画を行いたい場合はわざわざ上記の変換設定をするのが面倒
		そこで、RHW情報を使用して指定されたCostomVertexの情報を一気にスクリーン座標に変換している
*/
struct CustomVertex3D
{
	//座標情報
	float X;
	float Y;
	float Z;

	//カラー
	DWORD Color;
};

//FVF設定(2D用テクスチャなし)
#define FVF_2D_PORIGON (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
//FVF設定(2D用テクスチャあり)
#define FVF_2D_PORIGON_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//FVF設定(3D用)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/*
	
*/
#define ABC (1.0f)

// テクスチャの種類分のenumを作成
enum TextureID
{
	TexIDBackground,
	TexIDMax,
};

/**
* @brief DirectGraphics初期化関数
* @details 描画を使用するための初期化を行う
* @retval true 初期化成功
* @retval false 初期化失敗
* @param[in] window_handle 描画に使用するwindowハンドル
*/
bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();

void FinishDrawing();

void DrawPorigon();

void DrawPorigonWithTriangleList();
void DrawPorigonWithTriangleStrip();
void DrawPorigonWithTriangleFan(TextureID tex_id);

//View座標変換の設定
void SetUpView();
//プロジェクション座標変換の設定
void SetUpProjection();
//3Dポリゴン描画
void Draw3DPorigon();

void UpdateTPSCamera();

void DrawXFile(float pos_x, float pos_y, float pos_z, float angle);

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle);

bool LoadTexture(TextureID tex_id);
void ReleaseTexture();
void ReleaseXFile();

bool LoadXFile(LPCWSTR file_name);

#endif
