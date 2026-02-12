//*******************************************************
// 
// ゲームシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"
#include <vector>

// 前方宣言
class CTimer;		// タイマー
class CScore;		// スコア
class CSphereSky;	// 空
class COcean;		// 海
class CPlayer;		// プレイヤー
class CShip;		// 船
class CKraken;		// クラーケン
class CWord;		// ワード

// ゲームシーンクラス
class CGame : public CScene
{
public:
	
	// 読み取り情報
	typedef struct Info
	{
		int		MAXTIME				= 0;	// 最大タイム
		float	CAMERA_DISTANCE		= 0.0f;	// カメラ距離
		float	CAMERA_HEIGHT		= 0.0f;	// カメラ高さ
		float	CAMERA_DIPS			= 0.0f;	// カメラ奥行き
		float	CAMERA_ROT_X		= 0.0f;	// カメラ角度X
		float	SCORE_WIDTH			= 0.0f;	// スコア幅
		float	SCORE_HEIGHT		= 0.0f;	// スコア高さ
		int		SCORE_ADDPOINT_A	= 0;	// スコアの加算地
		int		SCORE_ADDPOINT_B	= 0;	// スコアの加算地
		int		SCORE_ADDPOINT_C	= 0;	// スコアの加算地
	}Info;

	CGame();
	~CGame();

	HRESULT Init(void);
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetScoreAddPointA(void){return m_info.SCORE_ADDPOINT_A;}
	int GetScoreAddPointB(void){return m_info.SCORE_ADDPOINT_B;}
	int GetScoreAddPointC(void){return m_info.SCORE_ADDPOINT_C;}

private:
	void ReadInfo(void);
	void CreateSky(void);
	void CreateOcean(void);
	void CreateShip(void);
	void CreatePlayer(void);
	void CreateKraken(void);
	void CreateTimer(void);
	void CreateScore(void);

	void UpdatePlyerAttackLevel(void);
	void CameraSet(void);

	Info m_info;	// 読み取り情報
};

#endif // !_GAME_H_
