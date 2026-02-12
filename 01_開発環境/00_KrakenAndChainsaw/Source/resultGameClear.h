//*******************************************************
// 
// リザルトゲームオーバーシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _RESULT_GAMECLEAR_H_
#define _RESULT_GAMECLEAR_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CWord;	// 文字ポリゴン

// リザルトシーンクラス
class CResultGameClear : public CScene	// CScene継承
{
public:
	static constexpr int TIME_CHAGE = 60 * 5;	// 遷移タイム

	CResultGameClear();
	~CResultGameClear();

	HRESULT Init(void) { return S_OK; }
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CWord* m_pWord;	// ワード
	int m_nCntTime;	// タイマーカウンター
};

#endif // !_GAME_H_