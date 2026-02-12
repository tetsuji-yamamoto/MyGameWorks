//*******************************************************
// 
// テストシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _TEST_SCENE_H_
#define _TEST_SCENE_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CTestEntity;

// ゲームシーンクラス
class CTestScene : public CScene	// CScene継承
{
public:

	CTestScene();
	~CTestScene();

	HRESULT Init(void) { return S_OK; }
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CTestEntity*	m_pEntity;
};

#endif