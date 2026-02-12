//*******************************************************
// 
// リザルトシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "result.h"
#include "debugproc.h"
#include "manager.h"
#include "button.h"
#include "myMath.h"
#include "resultRank.h"
#include "score.h"

// 静的メンバー変数宣言
std::vector<int> CResult::m_vScore;	// スコア
int CResult::m_nNewScore = 0;		// 新しいスコア

//*******************************************************
// リザルトシーンコンストラクタ
//*******************************************************
CResult::CResult()
{
	m_pButtonToTitle = nullptr;	// タイトルへ移行するボタン
	m_pButtonToGame = nullptr;	// ゲームへ移行するボタン
}

//*******************************************************
// リザルトシーンデストラクタ
//*******************************************************
CResult::~CResult()
{

}

//*******************************************************
// リザルトシーン初期化
//*******************************************************
void CResult::Init(MODE mode)
{
	// 親クラスを初期化
	CScene::Init(mode);
	
	// スコアを読み込む
	ReadScore();

	// 新しいスコアを追加
	m_vScore.push_back(m_nNewScore);

	// スコアをソート
	CMyMath::BubbleSort(m_vScore);

	// 後ろを消す
	m_vScore.erase(m_vScore.end() - 1);

	// スコアを書き込む
	WriteScore();

	// CSVDataの読込
	ReadCSV("data\\csv\\result.csv");
	
	// ランクのポリゴン
	auto iterScoreRank = m_vpResultRank.begin();
	// スコアの数分回して点滅させるものを探して設定する
	for (auto iterScore : m_vScore)
	{
		// 同じ値であれば
		if (iterScore == m_nNewScore)
		{
			// 点滅する状態にする
			(*iterScoreRank)->SetIsBlink(true);

			// 一個だけにしたいので処理を止める
			break;
		}

		// ランクポリゴンの要素をずらす
		iterScoreRank++;
	}

	// スコアの数分ポリゴンがあれば処理
	if (m_vScore.size() == m_vpResultRank.size())
	{
		// スコアカウンターゼロ
		int nCntScore = 0;

		// スコアの数分回す
		for (auto iterScore : m_vScore)
		{
			// スコアランクのイテレータ取得
			auto iterScoreRank = m_vpResultRank.begin() + nCntScore;

			// 文字列を作成
			std::string str = std::to_string(nCntScore + 1);
			str += ", ";
			str += std::to_string(iterScore);

			// 文字列を設定
			(*iterScoreRank)->SetWord(str);

			// スコアカウンターカウントアップ
			nCntScore++;
		}
	}
}

//*******************************************************
// リザルトシーン終了
//*******************************************************
void CResult::Uninit(void)
{
	// タイトルへ移行するボタン破棄
	if (m_pButtonToTitle != nullptr)
	{
		m_pButtonToTitle->Uninit();
		m_pButtonToTitle = nullptr;
	}

	// ゲームへ移行するボタン破棄
	if (m_pButtonToGame != nullptr)
	{
		m_pButtonToGame->Uninit();
		m_pButtonToGame = nullptr;
	}

	// ランクのポリゴン削除
	for (auto& iter : m_vpResultRank)
	{
		iter->Uninit();
	}
	m_vpResultRank.clear();

	// スコア削除
	m_vScore.clear();

	// 新しいスコアリセット
	m_nNewScore = 0;

	// 自身の破棄
	CScene::Uninit();
}

//*******************************************************
// リザルトシーン更新
//*******************************************************
void CResult::Update(void)
{
	// フェード移行状態であればこの先の処理をしない
	if (CManager::GetInstance()->GetFade() != CFade::FADE_NONE)
	{
		return;
	}

	// ボタンを押したらタイトルへ移行する処理
	if (m_pButtonToTitle != nullptr)
	{
		// 押されていたら
		if (m_pButtonToTitle->GetIsPress() == true)
		{
			// タイトルへ遷移
			CManager::GetInstance()->SetFade(CScene::MODE_TITLE);
		}

		// タイトルへ移行するボタン更新
		m_pButtonToTitle->Update();
	}

	// ボタンを押したらゲームへ移行する処理
	if (m_pButtonToGame != nullptr)
	{
		// 押されていたら
		if (m_pButtonToGame->GetIsPress() == true)
		{
			// タイトルへ遷移
			CManager::GetInstance()->SetFade(CScene::MODE_GAME);
		}

		// ゲームへ移行するボタン更新
		m_pButtonToGame->Update();
	}

	// スコアの数分ポリゴンがあれば処理
	if (m_vScore.size() == m_vpResultRank.size())
	{
		// スコアカウンターゼロ
		int nCntScore = 0;

		// スコアの数分回す
		for (auto iterScore : m_vScore)
		{
			// スコアランクのイテレータ取得
			auto iterScoreRank = m_vpResultRank.begin() + nCntScore;

			// 文字列を作成
			std::string str = std::to_string(nCntScore + 1);
			str += ", ";
			str += std::to_string(iterScore);

			// 文字列を設定
			(*iterScoreRank)->SetWord(str);
			
			// スコアカウンターカウントアップ
			nCntScore++;
		}
	}

#ifdef _DEBUG
	CDebugprocc::Print("SCene : [ RESULT ]\n");
#endif
}

//*******************************************************
// リザルトシーン描画
//*******************************************************
void CResult::Draw(void)
{
	// タイトルへ移行するボタン描画
	if (m_pButtonToTitle != nullptr)
	{
		m_pButtonToTitle->Draw();
	}

	// ゲームへ移行するボタン描画
	if (m_pButtonToGame != nullptr)
	{
		m_pButtonToGame->Draw();
	}
}

//************************************************************************
// リザルトシーン新しいスコアの追加
//************************************************************************
void CResult::AddNewScore(int nNewScore)
{
	m_nNewScore = nNewScore;
}

//************************************************************************
// リザルトシーンスコアの書き込み
//************************************************************************
void CResult::WriteScore(void)
{
	// ファイルを読込として開く
	FILE* pFile = fopen("data\\ranking\\ranking.txt", "w");

	// ファイルが開けなかったら処理しない
	if (pFile == NULL)
	{
		return;
	}

	for (auto& iterScore : m_vScore)
	{
		fprintf(pFile,"%d\n",iterScore);
	}

	// ファイルを閉じる
	fclose(pFile);
}

//************************************************************************
// リザルトシーンスコアの読込
//************************************************************************
void CResult::ReadScore(void)
{
	// ファイルを読込として開く
	FILE* pFile = fopen("data\\ranking\\ranking.txt", "r");

	// ファイルが開けなかったら処理しない
	if (pFile == NULL)
	{
		return;
	}

	// スコアを読み込む
	while(1)
	{
		int nScore = 0;
		int nResult;
		nResult = fscanf(pFile, "%d", &nScore);
		
		// 終端まで読んだら終わり
		if (nResult == EOF)
		{
			break;
		}

		// 要素追加
		m_vScore.push_back(nScore);
	}

	// ファイルを閉じる
	fclose(pFile);
}

//************************************************************************
// リザルトシーンCSVデータの読込
//************************************************************************
void CResult::ReadCSV(const char* pCSVFilePass)
{
	// 読み取り用としてファイルを開く
	FILE* pFile = fopen(pCSVFilePass, "r");

	// ファイルが開けなかった
	if (pFile == nullptr)
	{
		MessageBox(NULL, "CGameManager::ReadCSV() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// ファイルが開けたら
	bool bComment = false;			// コメントの有無
	int nXRow = 0;					// 列
	int nYLine = 0;					// 行
	D3DXVECTOR3 sizeDivision;		// 分割した幅
	sizeDivision.x = SCREEN_WIDTH / CSVDIVISIONX;
	sizeDivision.y = SCREEN_HEIGHT / CSVDIVISIONY;
	sizeDivision.z = 0.0f;

	while (1)
	{// ループ

		// 一文字読み取る
		char cData = fgetc(pFile);

		// コメント文字を読んだらそのあとの文字は読み飛ばす
		if (cData == '#')
		{
			bComment = true;				// コメント有の状態にする
			continue;						// whileから
		}
		// 改行よんだら
		else if (cData == '\n')
		{
			// 行をカウントアップ
			nYLine++;

			// 列をリセット
			nXRow = 0;
			bComment = false;				// コメント無の状態にする
			continue;						// whileから
		}
		// タブ文字よんだら
		else if (cData == '\t')
		{
			continue;	// whileから
		}
		// タブ文字よんだら
		else if (cData == EOF)
		{
			break;	// whileから抜け出す
		}
		// ,をよんだら
		else if (cData == ',')
		{
			continue;						// whileから
		}

		if (bComment == true)
		{// コメント機能がオン
			continue;	// whileから
		}

		// 列をカウントアップ
		nXRow++;

		// 現在位置を設定
		D3DXVECTOR3 pos = InitValu::VEC3_NULL;
		pos.x = sizeDivision.x * (float)nXRow;
		pos.y = sizeDivision.y * (float)nYLine;

		// タイトル移行生成
		if (cData == '2')
		{
			// タイトルへ移行するボタン生成
			if (m_pButtonToTitle == nullptr)
			{
				CButton::INFO info;
				info.colPolygon = Color::_FULL;
				info.colPolygonEmp = Color::_GRAY;
				info.colWord = Color::_BLACK;
				info.colWordEmp = Color::_GRAY;
				info.fHeight = 50.0f;
				info.fWidth = 100.0f;
				info.pos = pos;
				info.rot = InitValu::VEC3_NULL;
				m_pButtonToTitle = CButton::Create(info, "Title");
			}
		}
		// ゲームへ移行するボタン生成
		else if (cData == '3')
		{
			// ゲームへ移行するボタン生成
			if (m_pButtonToGame == nullptr)
			{
				CButton::INFO info;
				info.colPolygon = Color::_FULL;
				info.colPolygonEmp = Color::_GRAY;
				info.colWord = Color::_BLACK;
				info.colWordEmp = Color::_GRAY;
				info.fHeight = 50.0f;
				info.fWidth = 100.0f;
				info.pos = pos;
				info.rot = InitValu::VEC3_NULL;
				m_pButtonToGame = CButton::Create(info, "Game");
			}
		}
		// ランク生成
		else if (cData == '4')
		{
			// ランク以上の個数にならないようにする
			if (m_vpResultRank.size() < m_vScore.size())
			{
				// ランク表示生成
				CResultRank::INFO info;
				info.colPolygon = Color::_FULL;
				info.colWord = Color::_BLACK;
				info.colWordEmp = Color::_RED;
				info.fHeight = 50.0f;
				info.fWidth = 200.0f;
				info.pos = pos;
				info.rot = InitValu::VEC3_NULL;
				info.strWord = "0, ";
				info.strWord += std::to_string(CScore::MAXSORE);
				CResultRank* pResultRank = CResultRank::Create(info,NULL,DT_LEFT);

				// 要素を追加
				m_vpResultRank.push_back(pResultRank);
			}
		}
	}
}