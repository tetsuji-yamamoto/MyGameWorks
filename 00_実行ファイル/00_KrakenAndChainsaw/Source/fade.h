//*************************************************
// 
// フェード処理
// Author Tetsuji Yamamoto
// 
//*************************************************
#ifndef _FADE_H_
#define  _FADE_H_

#include "main.h"
#include "scene.h"

#define FADE_FRAME (120)	// フェードにかかるフレーム数

// フェードクラス
class CFade
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,	//何もしていない状態
		FADE_IN,		//フェードイン状態
		FADE_OUT,		//フェードアウト状態
		FADE_MAX
	}FADE;

	CFade();
	~CFade();

	void Init(CScene::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CScene::MODE modeNext);
	static CFade* Create(CScene::MODE modeNext);

	// ゲッター
	FADE GetFade(void) { return m_fade; }

private:
	CScreen* m_pScreen;			// スクリーンポリゴン
	FADE m_fade;				//フェードの状態
	CScene::MODE m_modeNext;	//次の画面(モード)
	int m_nCntFrame;			// フレームカウンター
};

#endif // !_FADE_H_