//**********************************************
// 
// スクリーン処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "main.h"

// スクリーンクラス
class CScreen
{
public:

	CScreen();
	~CScreen();

	void Init(D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(LPDIRECT3DTEXTURE9 pTexture);
	void DrawAlpherTest(LPDIRECT3DTEXTURE9 pTexture);
	void UpdateScale(void);

	// セッター
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetColor(D3DXCOLOR col);
	void SetScaleSize(float fScaleValue,int nTimeScale);

	// ゲッター
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetScaleValue(void) { return m_fScaleValue; }
	D3DXCOLOR GetColor(void) { return m_col; }

	static CScreen* Create(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	float m_fSize;						// 大きさ

	bool m_bScale;			// 拡大の有無
	float m_fScaleValue;	// 拡大率
	float m_fScaleValueNow;	// 今の拡大率
	int m_nFrameScale;		// 拡大する時間
	int m_nCntScale;		// 拡大カウンター
};

#endif
