//*******************************************************
// 
// タイトルシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene.h"
#include <vector>

class CObject2D;	// スクリーン

// ゲームシーンクラス
class CTutorial : public CScene	// CScene継承
{
public:
	static constexpr float STAGEWIDTH = 3000.0f;	// ステージの幅
	static constexpr float STAGEDIPS = 1800.0f;		// ステージの奥行き
	static constexpr int CSVDIVISIONX = 40;			// CSVデータの分割数
	static constexpr int CSVDIVISIONY = 20;			// CSVデータの分割数

	CTutorial();
	~CTutorial();

	HRESULT Init(void) { return S_OK; }
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObject2D* m_pScreen;	// スクリーン
	CObject2D* m_pScreen2;	// スクリーン
	int m_nCnt;// カウンター
};

#endif