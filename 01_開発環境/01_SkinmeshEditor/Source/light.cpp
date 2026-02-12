//****************************************
// 
// ライト
// Author Tetsuji Yamamoto
// 
//****************************************
#include "light.h"
#include "renderer.h"

//****************************************
// ライトコンストラクタ
//****************************************
CLight::CLight()
{
	ZeroMemory(&m_light, sizeof(m_light));
}

//****************************************
// ライトデストラクタ
//****************************************
CLight::~CLight()
{

}

//****************************************
// ライト初期化処理
//****************************************
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//ライトの方向ベクトル
	D3DXVECTOR3 vecDir;

	//ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0);

	//正規化する(大きさ１のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_light.Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &m_light);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);

	return S_OK;
}

//****************************************
// ライト終了処理
//****************************************
void CLight::Uninit(void)
{

}

//****************************************
// ライト更新処理
//****************************************
void CLight::Update(void)
{

}