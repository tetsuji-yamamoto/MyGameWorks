//**********************************************
// 
// レンダラー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

// 前方宣言
class CScreen;

// レンダラークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd,BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void ChangeTarget(int nIdxTexture, D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(int nIdx) { return m_apTextureMT[nIdx]; }
	LPDIRECT3DSURFACE9 GetZBuffMT(void) { return m_pZBuffMT; }
	void ClearTextureMT(void);
	void SetBlur(D3DXCOLOR bulrColor);
	void SetBlurUp(int nBlurTime, float fScaleValue, D3DXCOLOR bulrColor);
	void UpdateBulrUp(void);

private:
	LPDIRECT3D9 m_pD3D;						// Direct3Dオブジェクトへのポインタ
	static LPDIRECT3DDEVICE9 m_pD3DDevice;	// Direct3Dデバイスへのポインタ

	bool m_bBlur;		// ぼかしの有無
	bool m_bBlurUp;		// 拡大ぼかしの有無
	int m_nBlurTime;	// ぼかす時間
	int m_nCntBulur;	// ぼかしカウンター
	LPDIRECT3DTEXTURE9 m_apTextureMT[2];	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[2];		// テクスチャレンダリング用インターーフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;			// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewPortMT;				// テクスチャレンダリング用ビューポート

	CScreen* m_apScreen[2];	// 画面用ポリゴン
};
#endif