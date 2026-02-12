//*******************************************************
// 
// テストシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "sceneTest.h"
#include "testEntity.h"
#include "camera.h"
#include "shader.h"

//********************************************************
// コンストラクタ
//********************************************************
CTestScene::CTestScene()
{
	m_pEntity = nullptr;
}

//********************************************************
// デストラクタ
//********************************************************
CTestScene::~CTestScene()
{

}

//********************************************************
// 初期化
//********************************************************
void CTestScene::Init(MODE mode)
{
	m_pEntity = new CTestEntity;
	m_pEntity->Init();

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラ位置設定
	pCamera->SetDistance(500.0f);
	pCamera->SetRotation(D3DXVECTOR3(D3DX_PI * -0.35f, D3DX_PI * 0.0f, 0.0f));
	pCamera->SetPositionRDest(InitValu::VEC3_NULL);
	pCamera->ArrangeVDest();
}

//********************************************************
// 終了
//********************************************************
void CTestScene::Uninit(void)
{
	if (m_pEntity != nullptr)
	{
		m_pEntity->Uninit();
		m_pEntity = nullptr;
	}

	// 自身の破棄
	CScene::Uninit();
}

//********************************************************
// 更新
//********************************************************
void CTestScene::Update(void)
{
}

//********************************************************
// 描画
//********************************************************
void CTestScene::Draw(void)
{
}