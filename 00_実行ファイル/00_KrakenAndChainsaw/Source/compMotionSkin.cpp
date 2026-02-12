//*******************************************************************************************
// 
// モーション(スキンメッシュ)
// Author Tetsuji Yamamoto
// 
//*******************************************************************************************
#include "compMotionSkin.h"
#include "manager.h"
#include "texture.h"
#include "myMath.h"
#include "component.h"
#include "compWeapon.h"
#include "compCollisionModel.h"
#include "compInfo.h"
#include "quaternion.h"
#include "vector.h"
#include "shader.h"
#include "ShaderManager.h"
#include "light.h"

using namespace std;
using namespace nlohmann;
using namespace Quaternion;
using namespace my_math;
using namespace my_vector;

//*******************************************************************************************
// モーション(スキンメッシュ)コンストラクタ
//*******************************************************************************************
CMotionSkin::CMotionSkin(std::string motionFilePath):
	m_motionFIlePath(motionFilePath)
{
	m_pEntity = nullptr;								//エンティティ
	m_nNumBone = 0;										// ボーン数
	m_nNumVtx = 0;										// 頂点数
	m_nNumMotion = 0;									// モーションの総数
	ZeroMemory(&m_motionInfo, sizeof(MOTIONINFO));		// モーションで使う情報
	ZeroMemory(&m_motionInfoBlend, sizeof(MOTIONINFO));	// モーションで使う情報ブレンド用
	m_bBlend = false;									// モーションブレンドの有無
	m_bStopMotion = false;								// モーションストップの有無
	m_nBlendTime = 0;									// ブレンド時間
	m_nCntBlend = 0;									// ブレンドカウンター
	m_nTypeBlendMotion = 0;								// ブレンドするモーションタイプ
	m_Quat = InitValu::QUAT_NULL;						// クォータニオン
	m_pShader = nullptr;
}

//*******************************************************************************************
// モーション(スキンメッシュ)デストラクタ
//*******************************************************************************************
CMotionSkin::~CMotionSkin()
{

}

//*******************************************************************************************
// モーション(スキンメッシュ)初期化
//*******************************************************************************************
void CMotionSkin::Init(void)
{
#ifdef _DEBUG
	InitText(m_motionFIlePath.c_str());
#else
	InitText(m_motionFIlePath.c_str());
	//InitBinary(m_motionFIlePath.c_str());
#endif
}

//*******************************************************************************************
// モーション(スキンメッシュ)初期化バイナリ
//*******************************************************************************************
void CMotionSkin::InitBinary(std::string motionFilePathBinary)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 読み取り用としてファイルを開く
	ifstream ifs(motionFilePathBinary, ios::binary);

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CMotionSkin::InitBinary() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// エンティティ破棄
	m_pEntity->Uninit();

	// エンティティクリエイト
	m_pEntity = CEntity::Create();

	// ファイルが開けたら
	string str;				// 文字列->
	CBone::BONE boneRead;	// 読込用のボーン
	VERTEXSKIN vtxSkinRead;	// 頂点読込
	MOTION motionRead;		// モーション読込
	KEYSET keySetRead;		// モーション読込
	KEY keyRead;			// モーション読込

	// Xファイルの相対パスの文字数
	int nXfilePathSize;
	ifs.read((char*)&nXfilePathSize, sizeof(int));

	// 武器コンポーネント数読込
	int nNumCompWeapon;
	ifs.read((char*)&nNumCompWeapon, sizeof(int));

	//	当たり判定用モデルコンポーネント数読込
	int nNumCompCollModel;
	ifs.read((char*)&nNumCompCollModel, sizeof(int));

	// ボーンの数
	ifs.read((char*)&m_nNumBone, sizeof(int));

	// 頂点数
	ifs.read((char*)&m_nNumVtx, sizeof(int));

	// モーション数
	ifs.read((char*)&m_nNumMotion, sizeof(int));

	// Xファイルの相対パス
	for (int nCntFilePath = 0; nCntFilePath < nXfilePathSize; nCntFilePath++)
	{
		char cData;
		ifs.read((char*)&cData, sizeof(char));
		m_sXFilePass += cData;
	}

	// 武器コンポーネント読込
	for (int nCntCompWeapon = 0; nCntCompWeapon < nNumCompWeapon; nCntCompWeapon++)
	{
		CWeapon* weaponRead = new CWeapon;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName, sizeof(int));

		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));

		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		weaponRead->SetName(name);
		weaponRead->SetXFilePath(XFilePath);
		weaponRead->SetIdxBone(nIdxBone);
		weaponRead->SetPosition(pos);
		weaponRead->SetQuat(quat);

		// 初期化
		weaponRead->Init();

		// コンポーネントに要素追加
		m_pEntity->AddComponentPushBack(weaponRead);

		// 武器リストに追加
		m_vWeapon.push_back(weaponRead);
	}

	// 当たり判定用モデルコンポーネント読込
	for (int nCntCompCollModel = 0; nCntCompCollModel < nNumCompCollModel; nCntCompCollModel++)
	{
		CCollisionModel* collModelRead = new CCollisionModel;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName, sizeof(int));

		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));

		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		collModelRead->SetName(name);
		collModelRead->SetXFilePath(XFilePath);
		collModelRead->SetIdxBone(nIdxBone);
		collModelRead->SetPosition(pos);
		collModelRead->SetQuat(quat);

		// 初期化
		collModelRead->Init();

		// コンポーネントに要素追加
		m_pEntity->AddComponentPushBack(collModelRead);

		// 当たり判定用モデルリストに追加
		m_vCollModel.push_back(collModelRead);
	}

	// ボーン情報
	for (int nCntBone = 0; nCntBone < m_nNumBone; nCntBone++)
	{
		CBone::BONE bone;
		ifs.read((char*)&bone, sizeof(CBone::BONE));

		// 要素追加
		m_vBone.push_back(bone);
	}

	// 頂点情報
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		VERTEXSKIN vtxEdit;
		ifs.read((char*)&vtxEdit, sizeof(VERTEXSKIN));

		// 要素追加
		m_vVtxSkin.push_back(vtxEdit);
	}

	// モーション情報
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		MOTION motion;

		// ループ
		int nLoop = 0;
		ifs.read((char*)&nLoop, sizeof(int));
		motion.bLoop = (bool)nLoop;

		// キーセットの数
		ifs.read((char*)&motion.nNumKeyInfo, sizeof(int));

		// キーセット
		for (int nCntKeySet = 0; nCntKeySet < motion.nNumKeyInfo; nCntKeySet++)
		{
			KEYSET keySet;

			// フレーム数
			ifs.read((char*)&keySet.nFlame, sizeof(int));

			// キー数
			ifs.read((char*)&keySet.nNumkey, sizeof(int));

			// キー
			for (int nCntKey = 0; nCntKey < keySet.nNumkey; nCntKey++)
			{
				KEY key;
				ifs.read((char*)&key.fPosX, sizeof(float));
				ifs.read((char*)&key.fPosY, sizeof(float));
				ifs.read((char*)&key.fPosZ, sizeof(float));
				ifs.read((char*)&key.quat, sizeof(D3DXQUATERNION));

				// 要素追加
				keySet.vKey.push_back(key);
			}

			// 要素追加
			motion.vKeySet.push_back(keySet);
		}

		// 要素追加
		m_vMotion.push_back(motion);
	}

	// ファイルを閉じる
	ifs.close();

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		m_sXFilePass.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.dwNumMat,
		&m_model.pMesh
	);

	if (FAILED(hr))
	{// 失敗したら
		MessageBox(NULL, "CMotionSkin::Init() > モデルを読み込めませんでした", "Error", MB_OK);

		// 結果を返す
		return;
	}

	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		string texturePath = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
		m_model.vsTexturePath.push_back(texturePath);
	}

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	for (auto& iterBone : m_vBone)
	{
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pEntity->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)初期化
//*******************************************************************************************
void CMotionSkin::InitText(std::string motionFilePath)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 読み取り用としてファイルを開く
	ifstream ifs(motionFilePath);

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CMotionSkin::Init() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// エンティティ破棄
	if (m_pEntity != nullptr)
	{
		m_pEntity->Uninit();
	}

	// エンティティクリエイト
	m_pEntity = CEntity::Create();

	// ファイルが開けたら
	string str;				// 文字列
	char cData;				// 一文字
	CBone::BONE boneRead;	// 読込用のボーン
	VERTEXSKIN vtxSkinRead;	// 頂点読込
	MOTION motionRead;		// モーション読込
	KEYSET keySetRead;		// モーション読込
	KEY keyRead;			// モーション読込
	int nCntVtx = 0;		// 頂点カウンター
	D3DXVECTOR3 posRead;		// 読込位置
	D3DXQUATERNION quatRead;	// 読込向き
	string nameRead;			// 名前読込文字列
	string filePathRead;		// ファイルパス読込文字列
	int nDataRead;				// int読込用

	// 一文字ずつ読み取る
	while (ifs >> cData)
	{
		// コメント文字
		if (cData == '#')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// 改行文字
		else if (cData == '\n')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// タブ文字
		else if (cData == '\t')
		{
			continue;
		}

		// 文字列に一文字追加
		str += cData;

		// Xファイル
		if (str == "XFILEPASS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// Xファイルパス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// Xファイル読込
			HRESULT hr;
			hr = D3DXLoadMeshFromX(
				str.c_str(),
				D3DXMESH_MANAGED,
				pDevice,
				NULL,
				&m_model.pBuffMat,
				NULL,
				&m_model.dwNumMat,
				&m_model.pMesh
			);

			if (FAILED(hr))
			{// 失敗したら
				MessageBox(NULL, "CMotionSkin::Init() > モデルを読み込めませんでした", "Error", MB_OK);

				// 結果を返す
				return;
			}

			//マテリアル辺のポインタを取得
			D3DXMATERIAL* pMat;
			pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

			// マテリアルの数分回す
			for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
			{
				string texturePath = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
				m_model.vsTexturePath.push_back(texturePath);
			}

			// ファイルパス設定
			m_sXFilePass = str;

			// 文字列削除
			str.clear();
		}
		// 武器セット
		else if (str == "WEAPON_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 武器の名前
		else if (str == "WEAPON_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 武器のファイルパス
		else if (str == "WEAPON_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 武器のボーンインデックス
		else if (str == "WEAPON_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 武器の位置
		else if (str == "WEAPON_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の向き
		else if (str == "WEAPON_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の設定終わり
		else if (str == "WEAPON_END")
		{
			// コリジョン用モデルインスタンス生成
			CWeapon* pWeapon = new CWeapon;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pWeapon->SetName(nameRead);
			pWeapon->SetXFilePath(filePathRead);
			pWeapon->SetIdxBone(nDataRead);
			pWeapon->SetPosition(posRead);
			pWeapon->SetQuat(quatRead);

			// 初期化
			pWeapon->Init();

			// コンポーネントリストに追加
			m_pEntity->AddComponentPushBack(pWeapon);

			// 武器リストに追加
			m_vWeapon.push_back(pWeapon);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルセット
		else if (str == "COLLISION_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの名前
		else if (str == "COLLISION_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのファイルパス
		else if (str == "COLLISION_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのボーンインデックス
		else if (str == "COLLISION_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの位置
		else if (str == "COLLISION_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの向き
		else if (str == "COLLISION_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの設定終わり
		else if (str == "COLLISION_END")
		{
			// コリジョン用モデルインスタンス生成
			CCollisionModel* pCollModel = new CCollisionModel;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pCollModel->SetName(nameRead);
			pCollModel->SetXFilePath(filePathRead);
			pCollModel->SetIdxBone(nDataRead);
			pCollModel->SetPosition(posRead);
			pCollModel->SetQuat(quatRead);

			// 初期化
			pCollModel->Init();

			// コンポーネントリストに追加
			m_pEntity->AddComponentPushBack(pCollModel);

			// 当たり判定用モデルリストに追加
			m_vCollModel.push_back(pCollModel);

			// 文字列削除
			str.clear();
		}
		// ボーンの数読込
		else if (str == "NUMBONE")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			m_nNumBone = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点の数読込
		else if (str == "NUMVTX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点の数読込読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			m_nNumVtx = stoi(str);

			// 読み取った頂点の数分回す
			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{
				// 頂点情報初期化
				VERTEXSKIN vtxSkin;
				ZeroMemory(&vtxSkin, sizeof(vtxSkin));
				for (int nCntvtxSkin = 0; nCntvtxSkin < IDX_BONE_NUM; nCntvtxSkin++)
				{
					vtxSkin.aIdxBone[nCntvtxSkin] = -1;
					vtxSkin.aVtxWaght[nCntvtxSkin] = 0.0f;
				}

				// 要素を追加
				m_vVtxSkin.push_back(vtxSkin);
			}

			// 文字列削除
			str.clear();
		}
		// ボーン開始読込
		else if (str == "BONE_SET")
		{
			// 文字列削除
			str.clear();
		}
		// ボーンインデックス
		else if (str == "BONE_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdx = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(子)
		else if (str == "BONE_CHILD_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(子)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdxChild = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(親)
		else if (str == "BONE_PARENT_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(親)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// intに変換
			boneRead.nIdxParent = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_POS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_OFFSETPOS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// ボーン終了
		else if (str == "BONE_END")
		{
			// 要素を追加する
			// ワールドマトリックス初期化
			D3DXMatrixIdentity(&boneRead.mtxWorld);
			D3DXMatrixIdentity(&boneRead.mtxRot);
			boneRead.pBoneChild = NULL;
			boneRead.pBoneParent = NULL;
			D3DXQuaternionIdentity(&boneRead.quat);
			m_vBone.push_back(boneRead);

			// 読込情報リセット
			ZeroMemory(&boneRead, sizeof(boneRead));

			// 文字列削除
			str.clear();
		}
		// 頂点開始
		else if (str == "VTX_SET")
		{
			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス0
		else if (str == "VTX_IDXBONE_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aIdxBone[0] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス1
		else if (str == "VTX_IDXBONE_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aIdxBone[1] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ0
		else if (str == "VTX_WAGHT_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aVtxWaght[0] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ1
		else if (str == "VTX_WAGHT_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxSkinRead.aVtxWaght[1] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点法線オフセット
		else if (str == "VTX_NOROFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.norOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点位置オフセット
		else if (str == "VTX_POSOFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxSkinRead.posOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点終了
		else if (str == "VTX_END")
		{
			// 編集用頂点のイテレータ取得
			auto iterVtxEdit = m_vVtxSkin.begin() + nCntVtx;

			// 読み取った情報設定
			(*iterVtxEdit) = vtxSkinRead;

			// カウンター加算
			nCntVtx++;

			// 文字列削除
			str.clear();
		}
		// モーション開始
		else if (str == "MOTION_SET")
		{
			// モーションの総数増加
			m_nNumMotion++;

			// モーション読込情報初期化
			motionRead.bLoop = false;
			motionRead.nNumKeyInfo = 0;
			motionRead.vKeySet.clear();

			// 文字列削除
			str.clear();
		}
		// ループの有無
		else if (str == "MOTION_LOOP")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ループ読み取り
			CMyMath::ReadToSpace(ifs, str);

			// bool型に変換
			motionRead.bLoop = (bool)stoi(str);

			// 文字列削除
			str.clear();
		}
		// キーセットの数
		else if (str == "MOTION_NUMKEYINFO")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// キーセット読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			motionRead.nNumKeyInfo = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キーセット開始
		else if (str == "KEYSET_SET")
		{
			// 読み取り用キーセット初期化
			keySetRead.nFlame = 10;
			keySetRead.nNumkey = 0;
			keySetRead.vKey.clear();

			// 文字列削除
			str.clear();
		}
		// フレーム数
		else if (str == "KEYSET_FLAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// フレーム数読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			keySetRead.nFlame = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キー数
		else if (str == "KEYSET_NUMKEY")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// キー読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			keySetRead.nNumkey = stoi(str);

			// 文字列削除
			str.clear();
		}
		// キー開始
		else if (str == "KEY_SET")
		{
			// 読込用キー初期化
			keyRead.fPosX = 0.0f;
			keyRead.fPosY = 0.0f;
			keyRead.fPosZ = 0.0f;
			D3DXQuaternionIdentity(&keyRead.quat);

			// 文字列削除
			str.clear();
		}
		// 位置
		else if (str == "KEY_POS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosX = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosY = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.fPosZ = stof(str);

			// 文字列削除
			str.clear();
		}
		// 向き
		else if (str == "KEY_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			keyRead.quat.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// キー終了
		else if (str == "KEY_END")
		{
			// キーセットに要素追加
			keySetRead.vKey.push_back(keyRead);

			// 文字列削除
			str.clear();
		}
		// キーセット終了
		else if (str == "KEYSET_END")
		{
			// モーションに要素追加
			motionRead.vKeySet.push_back(keySetRead);

			// 文字列削除
			str.clear();
		}
		// モーション終了
		else if (str == "MOTION_END")
		{
			// モーション情報に要素追加
			m_vMotion.push_back(motionRead);

			// 文字列削除
			str.clear();
		}
	}

	// ファイルを閉じる
	ifs.close();

	// ボーンの親と子の設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pEntity->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}
}

//*******************************************************************************************
// モーション(スキンメッシュ)終了
//*******************************************************************************************
void CMotionSkin::Uninit(void)
{
	// エンティティ破棄
	m_pEntity->Uninit();

	// モーション情報破棄
	m_vMotion.clear();

	// ボーン破棄
	m_vBone.clear();

	// 頂点スキンメッシュ用破棄
	m_vVtxSkin.clear();

	// マテリアルバッファの破棄
	if (m_model.pBuffMat != nullptr)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = nullptr;
	}

	// メッシュの破棄
	if (m_model.pMesh != nullptr)
	{
		m_model.pMesh->Release();
		m_model.pMesh = nullptr;
	}

	// 自身の破棄
	delete this;
}

//*******************************************************************************************
// モーション(スキンメッシュ)更新
//*******************************************************************************************
void CMotionSkin::Update(void)
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	//// モーションが動く状態であれば処理
	//if (m_bStopMotion == false)
	//{
	//	// モーションの更新
	//	UpdateMotion();
	//}

	// 計算用マトリックス
	D3DXMATRIX mtxTrans, mtxRot;

	// モデルのワールドマトリックス計算
	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_model.mtxWorld);

	// 向きを追加
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_Quat);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pInfo->rot.y, pInfo->rot.x, pInfo->rot.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, pInfo->pos.x, pInfo->pos.y, pInfo->pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);

	// ボーンのワールドマトリックス計算
	for (auto& iterBone : m_vBone)
	{
		// ワールドマトリックス初期化
		D3DXMatrixIdentity(&iterBone.mtxWorld);

		// 向きを追加
		D3DXMatrixRotationQuaternion(&iterBone.mtxRot, &iterBone.quat);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.mtxRot);

		// 位置を追加
		D3DXMatrixTranslation(&mtxTrans, iterBone.pos.x, iterBone.pos.y, iterBone.pos.z);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxTrans);

		// 親がいれば親のマトリックスを掛ける
		if (iterBone.pBoneParent != NULL)
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.pBoneParent->mtxWorld);
		}
		// いなければモデルのマトリックスを掛ける
		else
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &m_model.mtxWorld);
		}
	}

	// 頂点バッファのポインタ
	BYTE* pVtxBuff = NULL;

	// 頂点フォーマットのサイズデータ
	WORD sizeFVF;

	// 頂点フォーマットのサイズを取得
	sizeFVF = (WORD)D3DXGetFVFVertexSize(m_model.pMesh->GetFVF());

	// 頂点バッファのロック
	m_model.pMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVtxBuff);

	// 頂点情報のイテレーター
	auto iterVtxSkin = m_vVtxSkin.begin();

	// モデルの頂点数分回す
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, iterVtxSkin++)
	{
		// 計算用変数
		D3DXVECTOR3 posHope, offSet, offSetEX, posLocal;
		D3DXVECTOR3 aNorOffset[IDX_BONE_NUM];
		D3DXMATRIX mtx, mtxOffset;

		// 一回でも設定下したかどうか
		bool bSet = false;

		// 位置をゼロにする
		posLocal = InitValu::VEC3_NULL;

		// 位置のポインタ設定
		D3DXVECTOR3* pPos = (D3DXVECTOR3*)pVtxBuff;

		offSet.x = (*iterVtxSkin).posOffSet.x;
		offSet.y = (*iterVtxSkin).posOffSet.y;
		offSet.z = (*iterVtxSkin).posOffSet.z;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			aNorOffset[nCntIdxBone].x = (*iterVtxSkin).norOffSet.x;
			aNorOffset[nCntIdxBone].y = (*iterVtxSkin).norOffSet.y;
			aNorOffset[nCntIdxBone].z = (*iterVtxSkin).norOffSet.z;

			int nIdxBone = (*iterVtxSkin).aIdxBone[nCntIdxBone];
			float fWeight = (*iterVtxSkin).aVtxWaght[nCntIdxBone];

			// 親ボーンが存在しないのであれば処理しない
			if (nIdxBone == -1)
			{
				aNorOffset[nCntIdxBone] = InitValu::VEC3_NULL;
				continue;
			}

			// セットした状態にする
			bSet = true;

			// ボーン情報取得
			auto iterBone = m_vBone.begin() + nIdxBone;

			// ボーンのマトリックス
			mtx = iterBone->mtxWorld;

			// 原点に移動
			offSetEX.x = offSet.x - (*iterBone).posOffset.x;
			offSetEX.y = offSet.y - (*iterBone).posOffset.y;
			offSetEX.z = offSet.z - (*iterBone).posOffset.z;

			// ボーンの回転を掛ける
			posHope.x = (offSetEX.x * mtx._11) + (offSetEX.y * mtx._21) + (offSetEX.z * mtx._31) + mtx._41;
			posHope.y = (offSetEX.x * mtx._12) + (offSetEX.y * mtx._22) + (offSetEX.z * mtx._32) + mtx._42;
			posHope.z = (offSetEX.x * mtx._13) + (offSetEX.y * mtx._23) + (offSetEX.z * mtx._33) + mtx._43;

			aNorOffset[nCntIdxBone].x = (aNorOffset[nCntIdxBone].x * mtx._11) + (aNorOffset[nCntIdxBone].y * mtx._21) + (aNorOffset[nCntIdxBone].z * mtx._31);
			aNorOffset[nCntIdxBone].y = (aNorOffset[nCntIdxBone].x * mtx._12) + (aNorOffset[nCntIdxBone].y * mtx._22) + (aNorOffset[nCntIdxBone].z * mtx._32);
			aNorOffset[nCntIdxBone].z = (aNorOffset[nCntIdxBone].x * mtx._13) + (aNorOffset[nCntIdxBone].y * mtx._23) + (aNorOffset[nCntIdxBone].z * mtx._33);

			// 重さを掛ける
			posHope *= fWeight;

			// 頂点座標に足す
			posLocal += posHope;
		}

		if (bSet == true)
		{
			// 計算した値を代入
			*pPos = posLocal;
		}
		else
		{
			// オフセット位置を設定
			*pPos = offSet;
		}

		// 位置のサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);

		// 法線のポインタを取得
		D3DXVECTOR3* pNor = (D3DXVECTOR3*)pVtxBuff;

		// ゼロに設定して
		*pNor = InitValu::VEC3_NULL;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			// 計算した値を足していく
			*pNor += aNorOffset[nCntIdxBone];
		}

		// 正規化する
		D3DXVec3Normalize(pNor, pNor);

		// 残りのサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);
		pVtxBuff += sizeof(D3DXVECTOR2);
	}

	// 頂点バッファのアンロック
	m_model.pMesh->UnlockVertexBuffer();

	// エンティティ更新
	CEntity::Update(m_pEntity);
}

//*******************************************************************************************
// モーション(スキンメッシュ)描画
//*******************************************************************************************
void CMotionSkin::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// マネージャーの取得
	CManager* pMana = CManager::GetInstance();

	// ライト取得
	CLight* pLight = CManager::GetInstance()->GetLight();
	
	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// シェーダーの取得
	int nIDShader = CManager::GetInstance()->GetIDShader();
	CShader* pShaderClass = CShaderManager::GetInstance()->GetShader(nIDShader);
	LPD3DXEFFECT pShader = pShaderClass->GetShader();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &InitValu::MTX_NULL);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// シェーダー起動
	pShader->Begin(0,0);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// 色取得
		D3DXVECTOR4 defuuse;
		defuuse = D3DXVECTOR4(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, pMat[nCntMat].MatD3D.Diffuse.a);

		// パラメータ(グローバル変数の設定)
		int nIDMtxWorld	= pMana->GetIDPramHandlMtxWorld();
		int nIDDeffuse	= pMana->GetIDPramHandlDeffuse();
		pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxWorld),	&InitValu::MTX_NULL);
		pShader->SetVector(pShaderClass->GetParamaterHandleHandle(nIDDeffuse),	&defuuse);

		// テクスチャの取得
		LPDIRECT3DTEXTURE9 pTex = pTexture->GetAddress(m_model.vsTexturePath[nCntMat].c_str());

		// テクスチャの設定
		pDevice->SetTexture(0, pTex);

		// パス起動,テクスチャの有無によりパスを変更
		if (pTex != nullptr)pShader->BeginPass(3);
		else pShader->BeginPass(2);

		// 変更をGPUに反映
		pShader->CommitChanges();

		// モデル(パーツの描画)
		m_model.pMesh->DrawSubset(nCntMat);

		// パス終了
		pShader->EndPass();
	}

	// シェーダー終了
	pShader->End();

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// エンティティ描画
	CEntity::Draw(m_pEntity);
}

////*******************************************************************************************
//// モーション(スキンメッシュ)生成
////*******************************************************************************************
//CMotionSkin* CMotionSkin::Create(std::string motionFilePath)
//{
//	CMotionSkin* pMotionSkin = new CMotionSkin;
//
//	if (pMotionSkin != NULL)
//	{
//		// 初期化処理
//		pMotionSkin->Init(motionFilePath);
//	}
//
//	return  pMotionSkin;
//}
//
////*******************************************************************************************
//// モーション(スキンメッシュ)生成バイナリ
////*******************************************************************************************
//CMotionSkin* CMotionSkin::CreateBinary(std::string motionFilePathBinary)
//{
//	CMotionSkin* pMotionSkin = new CMotionSkin;
//
//	if (pMotionSkin != NULL)
//	{
//		// 初期化処理
//		pMotionSkin->InitBinary(motionFilePathBinary);
//	}
//
//	return  pMotionSkin;
//}

//*************************************
// モーションクラスモーションの設定
//*************************************
void CMotionSkin::SetMotion(int nTypeMotion, int nBlendTime)
{
	// モーションが無ければ設定しない
	if (nTypeMotion < 0 || nTypeMotion >= m_nNumMotion)
	{
		return;
	}

	// モーション情報取得
	auto iterMotionInfo = m_vMotion.begin() + m_motionInfo.nTypeMotionNow;

	// ループの有無を取得する
	bool bLoop = (*iterMotionInfo).bLoop;

	// モーションの情報を受け継ぎ
	m_motionInfoBlend = m_motionInfo;

	m_motionInfo.bEndMotion = false;			// 終っていない状態にする
	m_motionInfo.nCounterMotion = 0;			// モーションカウンターリセット
	m_motionInfo.nKeyNow = 0;					//　今のキーリセット 
	m_motionInfo.nKeySetNext = 0;				//　次のキーセットリセット 
	m_motionInfo.nKeySetNow = 0;				//　今ののキーセットリセット 
	m_motionInfo.nTypeMotionNow = nTypeMotion;	//　今ののキーセットリセット 

	m_nCntBlend = 0;						// ブレンドカウンターゼロ
	m_nBlendTime = nBlendTime;				// ブレンド時間
	m_bBlend = true;						// ブレンドする状態にする

	// ブレンド時間は下限を超えないようにする
	if (m_nBlendTime <= 0)
	{
		m_nBlendTime = 1;
	}
}

//**************************************************************************
// モーションクラスキーのモデルの更新
//**************************************************************************
void CMotionSkin::UpdateModel(void)
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	// 計算用マトリックス
	D3DXMATRIX mtxTrans, mtxRot;

	// モデルのワールドマトリックス計算
	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_model.mtxWorld);

	// 向きを追加
	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quat);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, pInfo->pos.x, pInfo->pos.y, pInfo->pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);
}

//**************************************************************************
// モーションクラスキーの更新
//**************************************************************************
void CMotionSkin::UpdateMotionGetKey(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutRot, D3DXQUATERNION* pOutQuat, MOTIONINFO* pMotionInfo)
{
	auto iterMotionInfo = m_vMotion.begin() + pMotionInfo->nTypeMotionNow;				// モーション情報取得
	auto iterKeyInfoNow = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNow;	// キー情報取得
	bool bLoop = (*iterMotionInfo).bLoop;												// ループの有無取得
	int nNumKeyInfo = (*iterMotionInfo).nNumKeyInfo;									// キーの総数取得
	int nFrame = (*iterKeyInfoNow).nFlame;												// フレーム数取得

	pMotionInfo->nKeySetNext = pMotionInfo->nKeySetNow + 1;					// 次のキー設定
	float fMugFlame = (float)pMotionInfo->nCounterMotion / (float)nFrame;	// フレーム倍率を求める
	D3DXVECTOR3 posKeyNow;													// 今のキーの位置
	D3DXVECTOR3 posKeyNext;													// 次のキーの位置
	D3DXVECTOR3 posDiff;													// キーの位置の差分
	D3DXQUATERNION quatNow, quatNext;										// 今と次のクォータニオン

	// キーセットの限界を越したら
	if (pMotionInfo->nKeySetNext >= nNumKeyInfo)
	{
		// ゼロにする
		pMotionInfo->nKeySetNext = 0;

		// ループしない場合
		if (bLoop == false)
		{
			// 次のモーションは今と同じモーションにする
			pMotionInfo->nKeySetNext = pMotionInfo->nKeySetNow;
		}
	}

	// モーションカウンターがフレーム数を超えたら次のキーにする
	if (pMotionInfo->nCounterMotion > nFrame)
	{
		// ループしなくて、次のモーションが今のモーションと同じ場合
		if (bLoop == false && pMotionInfo->nKeySetNext == pMotionInfo->nKeySetNow)
		{
			// モーションカウンターを変えない
			pMotionInfo->nCounterMotion = nFrame;

			// モーションが終わった状態にする
			pMotionInfo->bEndMotion = true;
		}
		else
		{
			// モーションカウンターをリセット
			pMotionInfo->nCounterMotion = 0;

			// 次のモーションを設定
			pMotionInfo->nKeySetNow = pMotionInfo->nKeySetNext;
		}
	}

	// 今と先のキーを取得
	iterKeyInfoNow = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNow;
	auto iterKeyInfoNext = (*iterMotionInfo).vKeySet.begin() + pMotionInfo->nKeySetNext;
	auto iterKeyNow = (*iterKeyInfoNow).vKey.begin() + pMotionInfo->nKeyNow;
	auto iterKeyNext = (*iterKeyInfoNext).vKey.begin() + pMotionInfo->nKeyNow;

	posKeyNow.x = (*iterKeyNow).fPosX;	// 今の位置X取得
	posKeyNow.y = (*iterKeyNow).fPosY;	// 今の位置Y取得
	posKeyNow.z = (*iterKeyNow).fPosZ;	// 今の位置Z取得
	quatNow = (*iterKeyNow).quat;			// 今のクォータニオン

	posKeyNext.x = (*iterKeyNext).fPosX;	// 次の位置X取得
	posKeyNext.y = (*iterKeyNext).fPosY;	// 次の位置Y取得
	posKeyNext.z = (*iterKeyNext).fPosZ;	// 次の位置Z取得
	quatNext = (*iterKeyNext).quat;			// 次のクォータニオン

	// モーションが終わっている場合は値をそのまま返す
	if (pMotionInfo->bEndMotion == true)
	{
		*pOutPos = posKeyNow * fMugFlame;
		*pOutQuat = quatNow;
		return;
	}

	// 位置と向きの差分を求める
	posDiff = posKeyNext - posKeyNow;

	// 位置と向きの差分にフレーム倍率を掛けそれを足し位置を求める
	*pOutPos = posKeyNow + (posDiff * fMugFlame);

	// クォータニオンの球面線形補間の計算
	CMyMath::QuatSlerp(pOutQuat, quatNow, quatNext, fMugFlame);
}

//**************************************************************************
// モーションクラスモーションの更新
//**************************************************************************
void CMotionSkin::UpdateMotion(CMotionSkin* pBlendMotion)
{
	// モーションが動く状態であれば処理
	if (m_bStopMotion == true)return;

	// モーションが無ければ更新しない
	if (m_vMotion.empty() == true)return;

	// モデルカウンター
	int nCntModel = 0;

	// ボーンの数分回す
	for (auto iterBone = m_vBone.begin(); iterBone != m_vBone.end(); iterBone++, nCntModel++)
	{
		// モーション用ベクトル
		D3DXVECTOR3 outPos, outRot;

		// モーション用クォータニオン
		D3DXQUATERNION outQuat;

		// ブレンドモーション用ベクトル
		D3DXVECTOR3 blendPos, blendRot;

		// ブレンドモーション用クォータニオン
		D3DXQUATERNION blendQuat;

		// ボーンの位置と向きを取得
		D3DXVECTOR3 pos = iterBone->pos;
		D3DXQUATERNION quat = iterBone->quat;

		// モーションのアップデート
		UpdateMotionGetKey(&outPos, &outRot, &outQuat, &m_motionInfo);
		UpdateMotionGetKey(&blendPos, &blendRot, &blendQuat, &m_motionInfoBlend);

		// モーションブレンドをする状態であれば
		if (m_bBlend == true)
		{
			// ブレンド時間の倍率を出す
			float fMugBlendTime = (float)m_nCntBlend / (float)m_nBlendTime;

			outPos *= fMugBlendTime;
			outRot *= fMugBlendTime;
			outQuat *= fMugBlendTime;
			blendPos *= 1.0f - fMugBlendTime;
			blendRot *= 1.0f - fMugBlendTime;
			blendQuat *= 1.0f - fMugBlendTime;

			// 算出された値を追加
			pos = outPos + blendPos;
			quat = outQuat + blendQuat;
		}
		else
		{
			// 取得した値を追加
			pos = outPos;
			quat = outQuat;
		}

		// 位置と向きを設定
		(*iterBone).pos = pos;
		(*iterBone).quat = quat;

		// キー番号プラス
		m_motionInfo.nKeyNow++;
		m_motionInfoBlend.nKeyNow++;
	}

	// キー番号リセット
	m_motionInfo.nKeyNow = 0;
	m_motionInfoBlend.nKeyNow = 0;

	// モーションカウンターをカウントアップ
	m_motionInfo.nCounterMotion++;;
	m_motionInfoBlend.nCounterMotion++;

	// ブレンドモーションカウンタがブレンド時間より大きければ
	if (m_nCntBlend > m_nBlendTime)
	{
		// モーションブレンドをしない状態にする
		m_bBlend = false;
	}
	// ブレンドモーションカウンタがあれば
	else
	{
		// モーションブレンドカウントアップ
		m_nCntBlend++;
	}
}

//**************************************************************************
// モーションクラスボーンの位置取得
//**************************************************************************
D3DXVECTOR3 CMotionSkin::GetBonePos(int nBoneIdx)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return InitValu::VEC3_NULL;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を返す
	return (*iterBone).pos;
}

//**************************************************************************
// モーションクラスボーンの位置取得
//**************************************************************************
D3DXVECTOR3 CMotionSkin::GetBonePosOffSet(int nBoneIdx)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return InitValu::VEC3_NULL;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を返す
	return (*iterBone).posOffset;
}

//**************************************************************************
// モーションクラスボーンからボーンのベクトル取得
//**************************************************************************
D3DXVECTOR3 CMotionSkin::GetVec3BoneToBone(int nBoneIdxA, int nBoneIdxB)
{
	D3DXVECTOR3 pos01 = GetBonePosOffSet(nBoneIdxA);
	D3DXVECTOR3 pos02 = GetBonePosOffSet(nBoneIdxB);
	return pos02 - pos01;
}

//**************************************************************************
// モーションクラスボーンの位置設定
//**************************************************************************
void CMotionSkin::SetBonePos(int nBoneIdx, D3DXVECTOR3 pos)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	(*iterBone).pos = pos;
}

//**************************************************************************
// モーションクラスボーンの向き設定
//**************************************************************************
void CMotionSkin::SetBoneRotate(int nBoneIdx, D3DXQUATERNION quat)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	(*iterBone).quat = quat;
}

//**************************************************************************
// モーションクラスボーンのマトリックス設定
//**************************************************************************
void CMotionSkin::SetBoneMatrix(int nBoneIdx, D3DXMATRIX mtx)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	(*iterBone).mtxWorld = mtx;
}

//**************************************************************************
// モーションクラスボーンのマトリックス設定
//**************************************************************************
void CMotionSkin::SetBoneRotationMatrix(int nBoneIdx, D3DXMATRIX mtxRot)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	(*iterBone).mtxRot = mtxRot;
}

//**************************************************************************
// モーションクラスボーンのマトリックス取得
//**************************************************************************
D3DXMATRIX CMotionSkin::GetBoneMatrix(int nBoneIdx)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		D3DXMATRIX mtx;
		D3DXMatrixIdentity(&mtx);
		return mtx;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	return (*iterBone).mtxWorld;
}

//**************************************************************************
// モーションクラスボーンのマトリックス取得
//**************************************************************************
D3DXMATRIX* CMotionSkin::GetBoneMatrixPointer(int nBoneIdx)
{
	// 値が範囲外であれば処理しない
	if ((int)m_vBone.size() < nBoneIdx || 0 > nBoneIdx)
	{
		return nullptr;
	}

	// ボーンの情報取得
	auto iterBone = m_vBone.begin() + nBoneIdx;

	// 位置を設定
	return &(*iterBone).mtxWorld;
}

//**************************************************************************
// キーの取得
//**************************************************************************
CMotionSkin::KEY CMotionSkin::GetKey(const int nTypeMotion, const int nKeySet,const int nBone, const int nFrame)
{
	// 値返す用のキー
	KEY key,keyNext,keyReturn;

	// 値が範囲外であれば処理を切り上げる
	if (0 > nTypeMotion)return keyReturn;
	if (0 > nKeySet)return keyReturn;
	if (0 > nBone)return keyReturn;
	if ((int)m_vMotion.size() < nTypeMotion)return keyReturn;
	if((int)m_vMotion[nTypeMotion].vKeySet.size() < nKeySet)return keyReturn;
	if ((int)m_vBone.size() < nBone)return keyReturn;

	// 次のキーが範囲を超えないようにする
	int nNextKey = nKeySet + 1;
	if ((int)m_vMotion[nTypeMotion].vKeySet.size() < nNextKey)nNextKey = nKeySet;

	// キーを取得
	key = m_vMotion[nTypeMotion].vKeySet[nKeySet].vKey[nBone];
	keyNext = m_vMotion[nTypeMotion].vKeySet[nNextKey].vKey[nBone];

	// 計算用変数
	D3DXVECTOR3 posKeyNow, posKeyNext,posDiff,posOut;
	D3DXQUATERNION quatNow, quatNext,quatOut;
	
	posKeyNow.x = key.fPosX;		// 今の位置X取得
	posKeyNow.y = key.fPosY;		// 今の位置Y取得
	posKeyNow.z = key.fPosZ;		// 今の位置Z取得
	posKeyNext.x = keyNext.fPosX;	// 次の位置X取得
	posKeyNext.y = keyNext.fPosY;	// 次の位置Y取得
	posKeyNext.z = keyNext.fPosZ;	// 次の位置Z取得
	quatNow = key.quat;				// 今のクォータニオン
	quatNext = keyNext.quat;		// 次のクォータニオン

	// フレーム倍率を求める
	float fPalamFrame = Palam(nFrame, m_vMotion[nTypeMotion].vKeySet[nKeySet].nFlame);

	// 位置と向きの差分を求める
	posDiff = posKeyNext - posKeyNow;

	// 位置を線形補間する
	posOut = Vec3Lerp(posKeyNow, posKeyNext, fPalamFrame);

	// クォータニオンの球面線形補間
	quatOut = QuatSlerp(quatNow, quatNext, fPalamFrame);

	// 値を代入
	keyReturn.fPosX = posOut.x;
	keyReturn.fPosY = posOut.y;
	keyReturn.fPosZ = posOut.z;
	keyReturn.quat = quatOut;
	return keyReturn;
}

//**************************************************************************
// 現在のボーンのキーの取得
//**************************************************************************
CMotionSkin::KEY CMotionSkin::GetKeyBoneNow(const int nBone)
{
	// 値返す用のキー
	KEY keyOut;

	// 値が範囲外であれば処理を切り上げる
	if (0 > nBone)return keyOut;
	if ((int)m_vBone.size() < nBone)return keyOut;
	
	keyOut.fPosX = m_vBone[nBone].pos.x;
	keyOut.fPosY = m_vBone[nBone].pos.y;
	keyOut.fPosZ = m_vBone[nBone].pos.z;
	keyOut.quat = m_vBone[nBone].quat;

	return keyOut;

	//int nTypeMotion = m_motionInfo.nTypeMotionNow;							// モーションタイプ
	//int nKeySet = m_motionInfo.nKeySetNow;									// キーセット数
	//int nKeySetNext = m_motionInfo.nKeySetNext;								// キーセット数
	//int nFrame = m_vMotion[nTypeMotion].vKeySet[nKeySet].nFlame;			// フレーム数取得
	//int nCntMotion = m_motionInfo.nCounterMotion;							// モーションカウンタ
	//KEY key = m_vMotion[nTypeMotion].vKeySet[nKeySet].vKey[nBone];			// キーを取得
	//KEY keyNext = m_vMotion[nTypeMotion].vKeySet[nKeySetNext].vKey[nBone];	// 次のキー

	//// 計算用変数
	//D3DXVECTOR3 posKeyNow, posKeyNext, posDiff, posOut;
	//D3DXQUATERNION quatNow, quatNext, quatOut;

	//posKeyNow.x = key.fPosX;		// 今の位置X取得
	//posKeyNow.y = key.fPosY;		// 今の位置Y取得
	//posKeyNow.z = key.fPosZ;		// 今の位置Z取得
	//quatNow = key.quat;				// 今のクォータニオン

	//posKeyNext.x = key.fPosX;		// 次の位置X取得
	//posKeyNext.y = key.fPosY;		// 次の位置Y取得
	//posKeyNext.z = key.fPosZ;		// 次の位置Z取得
	//quatNext = key.quat;				// 次のクォータニオン

	//// フレーム倍率を求める
	//float fPalamFrame = Palam(nCntMotion, nFrame);

	//// 位置と向きの差分を求める
	//posDiff = posKeyNext - posKeyNow;

	//// 位置を線形補間する
	//posOut = Vec3Lerp(posKeyNow, posKeyNext, fPalamFrame);

	//// クォータニオンの球面線形補間
	//quatOut = QuatSlerp(quatNow, quatNext, fPalamFrame);

	//// 値を代入して返す
	//keyOut.fPosX = posOut.x;
	//keyOut.fPosY = posOut.y;
	//keyOut.fPosZ = posOut.z;
	//keyOut.quat = quatOut;
	//return keyOut;
}

//**************************************************************************
// 最後のキーセット取得
//**************************************************************************
CMotionSkin::KEYSET CMotionSkin::GetKeySetEnd(const int nTypeMotion)
{
	// 値返す用のキー
	KEYSET keySetReturn;

	// 値が範囲外であれば処理を切り上げる
	if (0 > nTypeMotion)return keySetReturn;
	if ((int)m_vMotion.size() < nTypeMotion)return keySetReturn;

	// キーセットを返す
	return *m_vMotion[nTypeMotion].vKeySet.end();
}

//**************************************************************************
// モーションクラスメッシュの当たり判定
//**************************************************************************
bool CMotionSkin::CollisionMesh(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutNor, D3DXMATRIX* pOutMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius, float fMovement, float fAngleNorLimit)
{
	// 出力先の位置
	D3DXVECTOR3 posOut = pos;

	// 乗っていない状態
	bool bRanding = false;

	// 出力先の情報設定
	*pOutPos = pos;
	*pOutNor = { 0.0f,1.0f,0.0f };
	pOutMtx = nullptr;

	// 計算時に使う変数
	D3DXVECTOR3 posNew;
	D3DXVECTOR3 posHit;
	D3DXVECTOR3 nor;
	float fLength = 0.0f;

	// メッシュとの当たり判定を取る
	if (CMyMath::CollisionMesh(&posHit, &nor, m_model.pMesh, posOut, posOld, fRadius) == true)
	{
		// 法線の角度を図る
		float fAngleNor = CMyMath::VectorAngleToRadian(D3DXVECTOR3(0.0f, -1.0f, 0.0f), nor);

		// 一定の角度を越したら滑る押し出し
		if (fAngleNor <= fAngleNorLimit)
		{
			// 法線方向へ半径分伸ばした位置
			posNew = posOut - nor * fRadius;

			// 押し出した分を取る
			D3DXVECTOR3 vecPush = CMyMath::ExtrusionPolygonSlide(&posNew, posHit, nor, posNew);

			// 元の位置に足す
			posOut += vecPush;
		}
		// 滑らない押し出し
		else
		{
			// 法線方向へ半径分伸ばした位置
			posNew = posOut - nor * fRadius;

			// 押し出した分を取る
			float fPush = CMyMath::ExtrusionPolygonNoSlip(&posNew, posHit, nor, posNew);

			// 元の位置に足す
			posOut.y += fPush;
			posOut.y += 0.2f;
		}

		// 移動させた方向と距離を計算
		D3DXVECTOR3 vecNH = posNew - posHit;
		float fLengthNNN = D3DXVec3Length(&vecNH);
		D3DXVec3Normalize(&vecNH, &vecNH);

		// 移動量を越して移動した抑える
		if (fLengthNNN >= fMovement)
		{
			posOut -= vecNH * (fLengthNNN - fMovement);
		}

		// 出力情報を設定
		*pOutPos = posOut;
		*pOutNor = nor;

		// 乗っている状態とする
		bRanding = true;
	}

	return bRanding;
}

//**************************************************************************
// モーションクラス当たり判定モデルの当たり判定
//**************************************************************************
bool CMotionSkin::CollisionModel(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutNor, D3DXMATRIX* pOutMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius, float fMovement, float fAngleNorLimit)
{
	// 出力先の位置
	D3DXVECTOR3 posOut = pos;

	// 乗っていない状態
	bool bRanding = false;

	// 出力先の情報設定
	*pOutPos = pos;
	*pOutNor = { 0.0f,1.0f,0.0f };
	pOutMtx = nullptr;

	// 計算時に使う変数
	D3DXVECTOR3 posNew;
	D3DXVECTOR3 posHit;
	D3DXVECTOR3 nor;
	float fLength = 0.0f;

	// 当たり判定用モデル分回す
	for (auto& iterCollModel : m_vCollModel)
	{
		// メッシュとの当たり判定を取る
		if (CMyMath::CollisionMesh(&posHit, &nor, iterCollModel->GetMesh(), posOut, posOld, fRadius) == true)
		{
			// 法線の角度を図る
			float fAngleNor = CMyMath::VectorAngleToRadian(D3DXVECTOR3(0.0f, -1.0f, 0.0f), nor);

			// 一定の角度を越したら滑る押し出し
			if (fAngleNor <= fAngleNorLimit)
			{
				// 法線方向へ半径分伸ばした位置
				posNew = posOut - nor * fRadius;

				// 押し出した分を取る
				D3DXVECTOR3 vecPush = CMyMath::ExtrusionPolygonSlide(&posNew, posHit, nor, posNew);

				// 元の位置に足す
				posOut += vecPush;
			}
			// 滑らない押し出し
			else
			{
				// 法線方向へ半径分伸ばした位置
				posNew = posOut - nor * fRadius;

				// 押し出した分を取る
				float fPush = CMyMath::ExtrusionPolygonNoSlip(&posNew, posHit, nor, posNew);

				// 元の位置に足す
				posOut.y += fPush;
				posOut.y += 0.2f;
			}

			// 移動させた方向と距離を計算
			D3DXVECTOR3 vecNH = posNew - posHit;
			float fLengthNNN = D3DXVec3Length(&vecNH);
			D3DXVec3Normalize(&vecNH, &vecNH);

			// 移動量を越して移動した抑える
			if (fLengthNNN >= fMovement)
			{
				posOut -= vecNH * (fLengthNNN - fMovement);
			}

			// 出力情報を設定
			*pOutPos = posOut;
			*pOutNor = nor;

			// 乗っている状態とする
			bRanding = true;
		}
	}

	return bRanding;
}

//**************************************************************************
// モーションクラスモーション取得
//**************************************************************************
CMotionSkin::MOTION CMotionSkin::GetMotion(const int nTypeMotion)
{
	MOTION motion;

	// 範囲外の値であれば処理しない
	if ((int)m_vMotion.size() < nTypeMotion || 0 > nTypeMotion)return motion;

	// モーションを返す
	return m_vMotion[nTypeMotion];
}