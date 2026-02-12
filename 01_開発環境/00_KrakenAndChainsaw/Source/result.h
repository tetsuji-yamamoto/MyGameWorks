//*******************************************************
// 
// リザルトシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CButton;		// ボタン
class CResultRank;	// ランク用ポリゴン

// リザルトシーンクラス
class CResult : public CScene	// CScene継承
{
public:
	static constexpr int CSVDIVISIONX = 64;	// CSVデータの分割数
	static constexpr int CSVDIVISIONY = 36;	// CSVデータの分割数
	static constexpr int MAXRANK = 5;		// 最大ランク数

	CResult();
	~CResult();

	HRESULT Init(void) { return S_OK; }
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ReadCSV(const char* pCSVFilePass);
	static void AddNewScore(int nNewScore);
	static void WriteScore(void);
	static void ReadScore(void);

private:
	CButton* m_pButtonToTitle;					// タイトルへ移行するボタン
	CButton* m_pButtonToGame;					// ゲームへ移行するボタン
	std::vector<CResultRank*> m_vpResultRank;	// ランクポリゴン
	static std::vector<int> m_vScore;			// スコア
	static int m_nNewScore;
};

#endif // !_GAME_H_