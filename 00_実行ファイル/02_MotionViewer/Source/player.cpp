//**********************************************
// 
// プレイヤー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "player.h"
#include "manager.h"
#include "input.h"
#include "particle.h"
#include "texture.h"

// 静的メンバー変数宣言
int CPlayer::m_nPlayerNum = 0;					//プレイヤーの総数


//**********************************************
// プレイヤーコンストラクタ
//**********************************************
CPlayer::CPlayer(int nPriority) : CObject2D(nPriority)
{
	m_nCntNextPattern = 0;
	m_nMaxTexPattern = 0;
	m_nNowPattern = 0;
	m_nLife = 0;
	m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pState = NULL;
	m_nPlayerNum++;	// プレイヤー総数カウントアップ
}

//**********************************************
// プレイヤーデストラクタ
//**********************************************
CPlayer::~CPlayer()
{
}

//**********************************************
// プレイヤー初期化処理
//**********************************************
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nMaxTexPattern,int nLife)
{

	// 初期化処理
	CObject2D::Init( pos, rot, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),fWidth,fHeight);

	// 種類設定
	CObject2D::SetType(CObject::TYPE_NONE);

	// 各種変数初期化
	m_nCntNextPattern = 0;
	m_nMaxTexPattern = nMaxTexPattern;
	m_nNowPattern = 0;
	m_nLife = nLife;

	// テクスチャンのインデックス設定
	CObject2D::SetIdxTexture(CTexture::TEXTURE_RANNINGMAN);

	// 状態クリエイト
	m_pState = CState::Create();

	return S_OK;
}

//**********************************************
//  プレイヤー終了処理
//**********************************************
void CPlayer::Uninit()
{
	m_pState->Uninit();
	m_pState = NULL;

	CObject2D::Uninit();
}

//**********************************************
//  プレイヤー更新処理
//**********************************************
void CPlayer::Update()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();	// キーボード取得
	CInputMouse* pInputMouce = CManager::GetInputMouse();			// マウス取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();		// ジョイパッド取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	D3DXVECTOR3 moveMouse = pInputMouce->GetVelocity();
	D3DXVECTOR3 rot = CObject2D::GetRotation();
	float moveBullet = 20.0f;
	float fAddRot = D3DX_PI * 0.01f;

	m_nCntNextPattern++;	// パターンカウンター加算

	if (m_nCntNextPattern >= 6)
	{
		m_nCntNextPattern = 0;
		m_nNowPattern++;
		if (m_nNowPattern >= 10)
		{
			m_nNowPattern = 0;
		}
	}

	int nLine = m_nNowPattern / 5;
	float fTexX = 1.0f / (m_nMaxTexPattern / 2);

	// 上下移動-------------------------------------------------------------------------------
	if (pInputKeyboard->GetPress(DIK_W) == true || 
		pInputJoypad->GetkeyRepeat(CInputJoypad::JOYKEY_UP) == true)
	{
		m_move.y -= 2.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true ||
		pInputJoypad->GetkeyRepeat(CInputJoypad::JOYKEY_DOWN) == true)
	{
		m_move.y += 2.0f;
	}

	// 左右移動-------------------------------------------------------------------------------
	if (pInputKeyboard->GetPress(DIK_A) == true ||
		pInputJoypad->GetkeyRepeat(CInputJoypad::JOYKEY_LEFT) == true)
	{
		m_move.x -= 2.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pInputJoypad->GetkeyRepeat(CInputJoypad::JOYKEY_RIGHT) == true)
	{
		m_move.x += 2.0f;
	}

	// 回転操作-------------------------------------------------------------------------------
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{
		rot.z += fAddRot;
	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		rot.z -= fAddRot;
	}

	// 弾発射---------------------------------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		rot.z += D3DX_PI * 0.125f;
		rot.z -= D3DX_PI * 0.125f;
		rot.z -= D3DX_PI * 0.125f;
		rot.z += D3DX_PI * 0.125f;
	}

	// マウス移動-------------------------------------------------------------------------------
	if (pInputMouce->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		m_move += moveMouse;
	}

	m_move.x += (-m_move.x * 0.3f);
	m_move.y += (-m_move.y * 0.3f);

	CObject2D::SetPosition(pos + m_move);
	CObject2D::SetRotation(rot);

	// 状態処理
	m_pState->StateCountDown();

	int nState = m_pState->GetState();

	switch (nState)
	{
	case CState::STATE_NONE:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case CState::STATE_DAMAGE:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		break;

	default:
		break;
	}

	CObject2D::Update(fTexX, 0.5f, m_nNowPattern, nLine);
}

//**********************************************
// プレイヤー描画処理
//**********************************************
void CPlayer::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	this->CObject2D::Draw();
}

//**********************************************
// プレイヤー
//**********************************************
void  CPlayer::SetTexAnim(int nPatternTex, int nCntPattern)
{
	m_nMaxTexPattern = nPatternTex;
	m_nCntNextPattern = nCntPattern;
}

//**********************************************
// プレイヤー生成処理
//**********************************************
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPatternTex, int nLife)
{
	CPlayer* pPlayer = NULL;
	int nIdxPlayerNum = m_nPlayerNum;


	// オブジェクト2Dの生成
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{
			// 初期化処理
			pPlayer->Init(pos, rot, fWidth, fHeight, nPatternTex,nLife);
		}
		else
		{
			MessageBox(NULL, " CObject2D* CObject2D::Create() > if (pObject2D != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CObject2D* CObject2D::Create() > if (pObject2D == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pPlayer;

}

//****************************************************************************
// プレイヤー
//****************************************************************************
void CPlayer::Hit(int nDamage)
{
	m_nLife -= nDamage;
	m_pState->SetStateCnt(30);					// 状態カウンターセット
	m_pState->SetState(CState::STATE_DAMAGE);	// 状態セット

	if (m_nLife <= 0)
	{// 死んでしまったら
		D3DXVECTOR3 pos = CObject2D::GetPosition();
		float fWCenter = SCREEN_WIDTH * 0.5f;
		float fHCenter = SCREEN_HEIGHT * 0.5f;
		D3DXVECTOR3 pos2 = { fWCenter,fHCenter,0.0f };
		D3DXVECTOR3 rot0 = { 0.0f,0.0f,0.0f };

		CParticle::Create(pos, rot0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 50.0f, 50.0f, 60, 5.0f, 150);

		Uninit();
	}
}