////************************************************************
//// 
////	スキンメッシュ用オブジェクト
//// Author Tetsuji Yamamoto
//// 
////************************************************************
//#include"objectSkin.h"
//#include"myMath.h"
//#include"texture.h"
//#include"manager.h"
//#include"model.h"
//#include"input.h"
//
////************************************************************
////	スキンメッシュ用オブジェクトコンストラクタ
////************************************************************
//CObjectSkin::CObjectSkin() : CObject(CObject::PRIORITY_4)
//{
//	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
//	m_pIdxBuff = NULL;							// インデックスバッファへのポインタ
//	ZeroMemory(&m_mtxWorld,sizeof(D3DXMATRIX));	// ワールドマトリックス
//	m_pos = InitValu::VEC3_NULL;					// 位置
//	m_rot = InitValu::VEC3_NULL;					// 向き
//	m_nNumPolygon = 0;							// ポリゴン数
//	m_nNumVtx = 0;								// 頂点数
//	m_nNumIdx = 0;								// インデックス数
//	m_nDivisionX= 0;							// Xの分割数
//	m_nDivisionY= 0;							// Yの分割数
//}
//
////************************************************************
////	スキンメッシュ用オブジェクトデストラクタ
////************************************************************
//CObjectSkin::~CObjectSkin()
//{
//
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト初期化
////************************************************************
//HRESULT CObjectSkin::Init(const char* pLoadFIlePass)
//{ 
//	// 情報読み取り
//	Load(pLoadFIlePass);
//
//	// ボーンの親子を作る
//	for (auto& iterBone : m_vectorBone)
//	{
//		// 親がいれば設定する
//		if (iterBone.m_nIdxParent == -1)
//		{
//			iterBone.pBonePerent = NULL;
//			continue;
//		}
//
//		// インデックスカウンター
//		int nCntIdx = iterBone.m_nIdxParent;
//
//		// ボーンの親子を作る
//		for (auto& iterBonePerent : m_vectorBone)
//		{
//			// 値が0であれば親
//			if (nCntIdx == 0)
//			{
//				iterBone.pBonePerent = &iterBonePerent;
//				break;
//			}
//
//			// 値を減らす
//			nCntIdx--;
//		}
//	}
//
//	return S_OK; 
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト終了
////************************************************************
//void CObjectSkin::Uninit(void)
//{
//	//LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
//	//LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
//	//D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
//	//D3DXVECTOR3 m_pos;					// 位置
//	//D3DXVECTOR3 m_rot;					// 向き
//	//std::pVector<CModel*>m_pVectorBoneModel;	// ボーンのモデル
//	//std::pVector<BONE>m_vectorBone;		// ボーンのリスト
//	//std::pVector<VERTEXSET>m_vectorVtxSet;	// 頂点の情報
//	//int m_nIdxTexture;					// テクスチャへのインデックス
//	//int m_nNumPolygon;					// ポリゴン数
//	//int m_nNumVtx;						// 頂点数
//	//int m_nNumIdx;						// インデックス数
//	//int m_nDivisionX;					// Xの分割数
//	//int m_nDivisionY;					// Yの分割数
//
//	// 頂点バッファの破棄
//	if (m_pVtxBuff != NULL)
//	{
//		m_pVtxBuff->Release();
//		m_pVtxBuff = NULL;
//	}
//
//	// 頂点インデックスの破棄
//	if (m_pIdxBuff != NULL)
//	{
//		m_pIdxBuff->Release();
//		m_pIdxBuff = NULL;
//	}
//
//	// ボーンのモデル破棄
//	for (auto& iter : m_pVectorBoneModel)
//	{
//		iter->Uninit();
//	}
//	m_pVectorBoneModel.clear();
//
//	// ボーンのリスト破棄
//	m_vectorBone.clear();
//
//	// 頂点の情報の破棄
//	m_vectorVtxSet.clear();
//
//	//// モーションの削除
//	//for (auto& iterMotion : m_pVectorMotion)
//	//{
//	//	// キーセットの削除
//	//	for (auto& iterKeySet : iterMotion->pVectorKeySet)
//	//	{
//	//		// キーの削除
//	//		for (auto& iterKeySet : iterKeySet->pVectorKey)
//	//		{
//	//			// キーメモリの破棄
//	//			delete iterKeySet;
//	//		}
//	//		iterKeySet->pVectorKey.clear();
//
//	//		// キーセットメモリの破棄
//	//		delete iterKeySet;
//	//	}
//	//	iterMotion->pVectorKeySet.clear();
//
//	//	// モーションメモリの破棄
//	//	delete iterMotion;
//	//}
//	//m_pVectorMotion.clear();
//
//	// 自身の破棄
//	Release();
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト更新
////************************************************************
//void CObjectSkin::Update(void)
//{
//	// キーボード入力取得
//	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
//
//	if (m_vectorBone.size() != 0)
//	{
//		auto iterBone = m_vectorBone.begin();
//
//		float fAddRot = D3DX_PI * 0.01f;
//
//		if (pInputKeyboard->GetPress(DIK_F) == true)
//		{
//			iterBone->rot.z += fAddRot;
//		}
//		else if (pInputKeyboard->GetPress(DIK_G) == true)
//		{
//			iterBone->rot.z -= fAddRot;
//		}
//
//		iterBone++;
//
//		if (pInputKeyboard->GetPress(DIK_V) == true)
//		{
//			iterBone->rot.z += fAddRot;
//		}
//		else if (pInputKeyboard->GetPress(DIK_B) == true)
//		{
//			iterBone->rot.z -= fAddRot;
//		}
//	}
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト描画
////************************************************************
//void CObjectSkin::Draw(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//
//	// テクスチャインスタンス取得
//	CTexture* pTexture = CManager::GetInstance()->GetTexture();
//
//	// 計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans;
//
//	// 頂点バッファへのポインタ
//	VERTEX_3D* pVtx = NULL;
//
//	// ワールドマトリックス初期化
//	D3DXMatrixIdentity(&m_mtxWorld);
//
//	// 向きを追加
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//
//	// 位置を追加
//	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
//
//	// ボーンの数分回す
//	for (auto& iterBone : m_vectorBone)
//	{
//		// ワールドマトリックス初期化
//		D3DXMatrixIdentity(&iterBone.mtxWorld);
//
//		// 向きを追加
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, iterBone.rot.y, iterBone.rot.x, iterBone.rot.z);
//		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxRot);
//
//		// 位置を追加
//		D3DXMatrixTranslation(&mtxTrans, iterBone.pos.x, iterBone.pos.y, iterBone.pos.z);
//		//D3DXMatrixIdentity(&mtxTrans);
//		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxTrans);
//
//		// 親がいれば親のマトリックスを掛ける
//		if (iterBone.pBonePerent != NULL)
//		{
//			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.pBonePerent->mtxWorld);
//		}
//		// いなければこのオブジェクトのマトリックスを掛ける
//		else
//		{
//			D3DXMatrixMultiply(&iterBone.mtxWorld, &m_mtxWorld, &iterBone.mtxWorld);
//		}
//	}
//	
//	// インデックスバッファをロック
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (auto& iterVtxSet : m_vectorVtxSet)
//	{
//		// 計算用変数
//		D3DXVECTOR3 posHope, offSet, offSetEX;
//		D3DXMATRIX mtx, mtxOffset;
//
//		// 位置をゼロにする
//		pVtx->pos = InitValu::VEC3_NULL;
//
//		// ボーンインデックスの数分回す
//		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
//		{
//			int nIdxBone = iterVtxSet.vtx[nCntIdxBone].nIdxBone;
//			float fWeight = iterVtxSet.vtx[nCntIdxBone].fWeight;
//
//			// 親ボーンが存在しないのであれば処理しない
//			if (nIdxBone == -1)
//			{
//				continue;
//			}
//
//			// ボーン情報取得
//			auto iterBone = m_vectorBone.begin();
//			std::advance(iterBone, nIdxBone);
//
//			mtx = iterBone->mtxWorld;
//
//			offSet.x = iterVtxSet.posOffset.x;
//			offSet.y = iterVtxSet.posOffset.y;
//			offSet.z = iterVtxSet.posOffset.z;
//
//			// 原点に移動
//			offSetEX.x = offSet.x - iterBone->pos.x;
//			offSetEX.y = offSet.y - iterBone->pos.y;
//			offSetEX.z = offSet.z - iterBone->pos.z;
//
//			// ボーンの回転を掛ける
//			posHope.x = (offSetEX.x * mtx._11) + (offSetEX.y * mtx._21) + (offSetEX.z * mtx._31) + mtx._41;
//			posHope.y = (offSetEX.x * mtx._12) + (offSetEX.y * mtx._22) + (offSetEX.z * mtx._32) + mtx._42;
//			posHope.z = (offSetEX.x * mtx._13) + (offSetEX.y * mtx._23) + (offSetEX.z * mtx._33) + mtx._43;
//
//			// 重さを掛ける
//			posHope *= fWeight;
//
//			// 位置に足す
//			pVtx->pos += posHope;
//		}
//
//		// 頂点情報を一つ進める
//		pVtx++;
//	}
//
//	// 頂点バッファをアンロック
//	m_pVtxBuff->Unlock();
//
//	// テクスチャを設定
//	pDevice->SetTexture(0, pTexture->GetAddress(NULL));
//
//	// 頂点バッファをデバイスのデータストリームに設定
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
//
//	// インデックスバッファをデータストリームに設定
//	pDevice->SetIndices(m_pIdxBuff);
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ポリゴンの描画
//	pDevice->DrawIndexedPrimitive(
//		D3DPT_TRIANGLESTRIP,
//		0,
//		0,
//		m_nNumVtx,
//		0,
//		m_nNumPolygon
//	);
//
//	// ボーンのモデル描画
//	for (auto& iterBoneModel : m_pVectorBoneModel)
//	{
//		// ボーンの数分回す
//		auto iterBone = m_vectorBone.begin();
//
//		std::advance(iterBone, iterBoneModel->GetIdxModel());
//
//		iterBoneModel->Draw(iterBone->mtxWorld);
//	}
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト生成
////************************************************************
// CObjectSkin* CObjectSkin::Create(const char* pLoadFIlePass)
//{
//	 //	ポリゴンUIのインスタンス生成
//	 CObjectSkin* pObjectSkin = new CObjectSkin;
//
//	 // 確保できた
//	 if (pObjectSkin != NULL)
//	 {
//		 // 初期化処理
//		 pObjectSkin->Init(pLoadFIlePass);
//	 }
//	 // 出系なかった
//	 else
//	 {
//		 MessageBox(NULL, " CObjectSkin::Create() > メモリを確保できませんでした", "Error", MB_OK);
//	 }
//
//	 return pObjectSkin;
//}
//
////************************************************************
////	スキンメッシュ用オブジェクト読込
////************************************************************
//void CObjectSkin::Load(const char* pLoadFIlePass)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//
//	// 読み取り用としてファイルを開く
//	FILE* pFile = fopen(pLoadFIlePass, "r");
//
//	// ファイルが開けなかった
//	if (pFile == NULL)
//	{
//		MessageBox(NULL, "CObjectSkin::Load() > ファイルが開けませんでした", "Error", MB_OK);
//		return;
//	}
//
//	// ファイルが開けたら
//	char aStr[1024] = {};	// 文字列
//	bool bComment = false;	// コメントの有無
//	int nIdxStr = 0;		// 文字列インデックス
//	int nIdxFileName = 0;	// ファイル名インデックス
//	char aData[4] = {};		// =読み取り用
//
//	// 各クラスポインタと初期化に必要な変数
//	D3DXVECTOR3 pos = InitValu::VEC3_NULL;
//	char aStrLoad[128] = {};
//
//	// 頂点バッファへのポインタ
//	VERTEX_3D* pVtx = NULL;
//
//	// インデック情報へのポインタ
//	WORD* pIdx = NULL;
//
//	// 読込用モーション変数
//	MOTION motionLoad;
//
//	while (1)
//	{// ループ
//
//		// 一文字読み取る
//		char cData = fgetc(pFile);
//
//		if (cData == '#')
//		{// コメント文字を読んだらそのあとの文字は読み飛ばす
//			bComment = true;				// コメント有の状態にする
//			memset(aStr, 0, sizeof(aStr));	// 文字列を空にする
//			nIdxStr = 0;					// 文字列インデックスリセット
//			continue;						// whileから
//		}
//		else if (cData == '\n')
//		{// 改行よんだら
//			bComment = false;				// コメント無の状態にする
//			memset(aStr, 0, sizeof(aStr));	// 文字列を空にする
//			nIdxStr = 0;					// 文字列インデックスリセット
//			continue;						// whileから
//		}
//		else if (cData == '\t')
//		{// タブ文字よんだら
//			continue;	// whileから
//		}
//		else if (cData == EOF)
//		{// タブ文字よんだら
//			break;	// whileから抜け出す
//		}
//
//		if (bComment == true)
//		{// コメント機能がオン
//			continue;	// whileから
//		}
//
//		aStr[nIdxStr] = cData;	// 一文字代入
//		nIdxStr++;				// インデックスカウントアップ
//
//		// メッシュフィールド読込開始
//		if (strcmp(aStr, "SET_MESHFIELD") == 0)
//		{
//
//		}
//		// 縦の分割数読込
//		else if (strcmp(aStr, "DIVISION_X") == 0)
//		{
//			// = の読込
//			fscanf(pFile, "%s", aStr);
//
//			// 値の読込
//			fscanf(pFile, "%d", &m_nDivisionX);
//		}
//		// 縦の分割数読込
//		else if (strcmp(aStr, "DIVISION_Y") == 0)
//		{
//			// = の読込
//			fscanf(pFile, "%s", aStr);
//
//			// 値の読込
//			fscanf(pFile, "%d", &m_nDivisionY);
//		}
//		// テクスチャ読込
//		else if (strcmp(aStr, "TEXTUREPASS") == 0)
//		{
//		}
//		// メッシュフィールド読込終わり
//		else if (strcmp(aStr, "END_MESHFIELD") == 0)
//		{
//			// フィールド部分のポリゴン数を求める
//			m_nNumPolygon = CMyMath::SeekFieldNumPolygonAll(m_nDivisionX, m_nDivisionY);
//
//			// フィールドの頂点数を求める
//			m_nNumVtx = CMyMath::SeekFieldNumVertexAll(m_nDivisionX, m_nDivisionY);
//
//			// フィールドのインデックス数を求める
//			m_nNumIdx = CMyMath::SeekFieldNumIndexAll(m_nNumPolygon);
//
//			// 頂点を生成
//			HRESULT hr = pDevice->CreateVertexBuffer(
//				sizeof(VERTEX_3D) * m_nNumVtx,
//				D3DUSAGE_WRITEONLY,
//				FVF_VERTEX_3D,
//				D3DPOOL_MANAGED,
//				&m_pVtxBuff,
//				NULL);
//
//			// エラーメッセージ
//			if (FAILED(hr))
//			{
//				MessageBox(NULL, "CObjectSkin::Load > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
//				return;
//			}
//
//			// インデックスの生成
//			hr = pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
//				D3DUSAGE_WRITEONLY,
//				D3DFMT_INDEX16,
//				D3DPOOL_DEFAULT,
//				&m_pIdxBuff,
//				NULL);
//
//			// エラーメッセージ
//			if (FAILED(hr))
//			{
//				MessageBox(NULL, "CObjectSkin::Load > インデックスバッファをクリエイトできませんでした", "Error", MB_OK);
//				return;
//			}
//
//			// 頂点バッファをアンロック
//			m_pVtxBuff->Unlock();
//
//			// インデック情報へのポインタ
//			WORD* pIdx;
//
//			// インデックスバッファをロック
//			m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
//
//			int nIdxA = m_nDivisionX + 1;	// 左側インデックス
//			int nIdxB = 0;				// 右側インデックス
//			int nCntIdx = 0;			// インデックスカウンター
//
//			// インデックスを求める
//			for (int nCntA = 0; nCntA < m_nDivisionY; nCntA++)
//			{// Zの分割数分回す
//				for (int nCntB = 0; nCntB <= m_nDivisionX; nCntB++)
//				{// Xの分割数分回す
//					pIdx[nCntIdx] = nIdxA;
//					pIdx[nCntIdx + 1] = nIdxB;
//					nCntIdx += 2;
//					nIdxA++;
//					nIdxB++;
//				}
//
//				if (nCntA < m_nDivisionY - 1)
//				{// 安全装置
//					pIdx[nCntIdx] = nIdxB - 1;
//					pIdx[nCntIdx + 1] = nIdxA;
//					nCntIdx += 2;
//				}
//			}
//
//			// インデックスバッファのアンロック
//			m_pIdxBuff->Unlock();
//		}
//		// 縦の分割数読込
//		else if (strcmp(aStr, "SET_BONE") == 0)
//		{
//			// ボーン情報
//			BONE bone;
//
//			// = の読込
//			fscanf(pFile, "%s", aStr);
//
//			// インデックスの読込
//			fscanf(pFile, "%d", &bone.m_nIdxParent);
//			
//			// | の読込
//			fscanf(pFile, "%s", aStr);
//			
//			// 位置の読込
//			fscanf(pFile, "%f", &bone.pos.x);
//			fscanf(pFile, "%f", &bone.pos.y);
//			fscanf(pFile, "%f", &bone.pos.z);
//
//			// | の読込
//			fscanf(pFile, "%s", aStr);
//
//			// 向きの読込
//			fscanf(pFile, "%f", &bone.rot.x);
//			fscanf(pFile, "%f", &bone.rot.y);
//			fscanf(pFile, "%f", &bone.rot.z);
//
//			bone.m_nIdxModel = (int)m_vectorBone.size();
//
//			m_vectorBone.push_back(bone);
//
//			CModel* pModel = CModel::Create(bone.m_nIdxModel,-1,InitValu::VEC3_NULL, InitValu::VEC3_NULL, "data\\model\\bone_001.x");
//			m_pVectorBoneModel.push_back(pModel);
//		}
//		// 頂点情報読込終了
//		else if (strcmp(aStr, "VERTEXSET") == 0)
//		{
//			// 頂点情報インスタンス生成
//			VERTEXSET vtxSet;
//
//			// ボーンのインデックスの最大数分回す
//			for (int nCntVtxSet = 0; nCntVtxSet < IDX_BONE_NUM; nCntVtxSet++)
//			{
//				// 重さの読込
//				fscanf(pFile, "%f", &vtxSet.vtx[nCntVtxSet].fWeight);
//
//				// インデックスの読込
//				fscanf(pFile, "%d", &vtxSet.vtx[nCntVtxSet].nIdxBone);
//			}
//
//			// 要素追加
//			m_vectorVtxSet.push_back(vtxSet);
//		}
//		// 頂点バッファ情報読込開始
//		else if (strcmp(aStr, "SET_VERTEXBUFF") == 0)
//		{
//			// 頂点バッファをロック
//			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//		}
//		// 頂点バッファ情報読込
//		else if (strcmp(aStr, "VERTEXBUFF") == 0)
//		{
//			if (pVtx != NULL)
//			{
//				// 位置の読込
//				fscanf(pFile, "%f", &pVtx->pos.x);
//				fscanf(pFile, "%f", &pVtx->pos.y);
//				fscanf(pFile, "%f", &pVtx->pos.z);
//
//				// |を読み込む
//				fscanf(pFile, "%s", aStr);
//
//				// 法線の読込
//				fscanf(pFile, "%f", &pVtx->nor.x);
//				fscanf(pFile, "%f", &pVtx->nor.y);
//				fscanf(pFile, "%f", &pVtx->nor.z);
//
//				// |を読み込む
//				fscanf(pFile, "%s", aStr);
//
//				// 色の読込
//				D3DXCOLOR col;
//				fscanf(pFile, "%f", &col.r);
//				fscanf(pFile, "%f", &col.g);
//				fscanf(pFile, "%f", &col.b);
//				fscanf(pFile, "%f", &col.a);
//				pVtx->col = col;
//
//				// |を読み込む
//				fscanf(pFile, "%s", aStr);
//
//				// 色の読込
//				fscanf(pFile, "%f", &pVtx->tex.x);
//				fscanf(pFile, "%f", &pVtx->tex.y);
//
//				// 頂点情報を一つずらす
//				pVtx++;
//			}
//		}
//		// 頂点バッファ情報読込終了
//		else if (strcmp(aStr, "END_VERTEXBUFF") == 0)
//		{
//			// 頂点バッファをアンロック
//			m_pVtxBuff->Unlock();
//
//			// インデックスバッファをロック
//			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//			for (auto& iter : m_vectorVtxSet)
//			{
//				iter.posOffset = pVtx->pos;
//				pVtx++;
//			}
//
//			// 頂点バッファをアンロック
//			m_pVtxBuff->Unlock();
//
//			pVtx = NULL;
//		}
//		// 頂点情報読込終了
//		else if (strcmp(aStr, "VERTEXSET") == 0)
//		{
//		// 頂点情報インスタンス生成
//		VERTEXSET vtxSet;
//
//			// ボーンのインデックスの最大数分回す
//			for (int nCntVtxSet = 0; nCntVtxSet < IDX_BONE_NUM; nCntVtxSet++)
//			{
//				// 重さの読込
//				fscanf(pFile, "%f", &vtxSet.vtx[nCntVtxSet].fWeight);
//
//				// インデックスの読込
//				fscanf(pFile, "%d", &vtxSet.vtx[nCntVtxSet].nIdxBone);
//			}
//
//			// 要素追加
//			m_vectorVtxSet.push_back(vtxSet);
//		}
//		// モーション読込
//		else if (strcmp(aStr, "SET_MOTION") == 0)
//		{
//		}
//		// ループの有無読込
//		else if (strcmp(aStr, "LOOP") == 0)
//		{
//			//int nData = 0;
//
//			//// = を読み込む
//			//fscanf(pFile, "%s", aStr);
//
//			//// 値を読み込む
//			//fscanf(pFile, "%d", &nData);
//
//			//// 値をキャストして代入
//			//motionLoad.bLoop = (int)nData;
//		}
//		// キーセット報読込開始
//		else if (strcmp(aStr, "SET_KEYSET") == 0)
//		{
//		}
//		// フレーム数込開始
//		else if (strcmp(aStr, "FRAME") == 0)
//		{
//		}
//		// キー情報読込
//		else if (strcmp(aStr, "KEY") == 0)
//		{
//		}
//		// キーセット読込終了
//		else if (strcmp(aStr, "END_KEYSET") == 0)
//		{
//		}
//		// モーション込終了
//		else if (strcmp(aStr, "END_MOTION") == 0)
//		{
//			// 要素追加
//			//m_pVectorMotion.push_back(motionLoad);
//
//			// 値リセット
//			//ZeroMemory(&motionLoad, sizeof(MOTION));
//		}
//	}
//
//	// ファイルを閉じる
//	fclose(pFile);
//}