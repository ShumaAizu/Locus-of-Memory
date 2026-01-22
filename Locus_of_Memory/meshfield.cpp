//=============================================================================
//
//	メッシュフィールド処理 [meshfield.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

// メモ
// フィールド全体の情報

#include "main.h"
#include "meshfield.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_SPLIT_WIDHT		(30 + 1)		// 横の分割数
#define MESHFIELD_SPLIT_DEPTH		(30 + 1)		// 縦の分割数
#define MAX_MESHFIELD				(32)			// メッシュフィールドの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureMeshField[MESHFIELDTYPE_MAX] = {};		// テクスチャへのポインタ
MeshField g_ameshfield[MAX_MESHFIELD];									// メッシュフィールドの情報

const char* c_pMeshFieldTextureName[MESHFIELDTYPE_MAX] =
{
	"data\\TEXTURE\\road000.jpg",
	"data\\TEXTURE\\road000.jpg",
	"data\\TEXTURE\\road000.jpg",
};

//=============================================================================
//	メッシュフィールドの初期化処理
//=============================================================================
void InitMeshField(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	PMESHFIELD pMeshField = &g_ameshfield[0];	// 先頭アドレス

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < MESHFIELDTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pMeshFieldTextureName[nCntTex],
			&g_apTextureMeshField[nCntTex]);
	}

	// 数分初期化(後でmemsetに変えてもいいかも)
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++, pMeshField++)
	{
		pMeshField->pVtxBuff = NULL;
		pMeshField->pIdxBuff = NULL;
		pMeshField->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pMeshField->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pMeshField->type = MESHFIELDTYPE_MAX;
		pMeshField->bUse = false;
	}

	// いったん直
	SetMeshField(D3DXVECTOR3(-5000.0f, -50.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, 20, 20, MESHFIELDTYPE_000);
}

//=============================================================================
//	メッシュフィールドの終了処理
//=============================================================================
void UninitMeshField(void)
{
	PMESHFIELD pMeshField = &g_ameshfield[0];		// 先頭アドレス

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MESHFIELDTYPE_MAX; nCntTex++)
	{
		if (g_apTextureMeshField[nCntTex] != NULL)
		{
			g_apTextureMeshField[nCntTex]->Release();
			g_apTextureMeshField[nCntTex] = NULL;
		}
	}

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++, pMeshField++)
	{
		// 頂点バッファの破棄
		if (pMeshField->pVtxBuff != NULL)
		{
			pMeshField->pVtxBuff->Release();
			pMeshField->pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (pMeshField->pIdxBuff != NULL)
		{
			pMeshField->pIdxBuff->Release();
			pMeshField->pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュフィールドの描画処理
//=============================================================================
void DrawMeshField(void)
{
	PMESHFIELD pMeshField = &g_ameshfield[0];	// 先頭アドレス

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++, pMeshField++)
	{
		if (pMeshField->bUse == false || pMeshField->bMesh == true)
		{// 使っていないもしくは当たり判定用メッシュなら弾く
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMeshField->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshField->rot.y, pMeshField->rot.x, pMeshField->rot.z);
		D3DXMatrixMultiply(&pMeshField->mtxWorld, &pMeshField->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pMeshField->pos.x, pMeshField->pos.y, pMeshField->pos.z);
		D3DXMatrixMultiply(&pMeshField->mtxWorld, &pMeshField->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMeshField->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pMeshField->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pMeshField->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMeshField[pMeshField->type]);

		// メッシュフィールドの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			pMeshField->nSplitWidth * pMeshField->nSplitDepth,
			0,
			((pMeshField->nSplitWidth - 1) * (pMeshField->nSplitDepth - 1) * 2) + ((pMeshField->nSplitDepth - 2) * 4));
	}
}

//=============================================================================
//	メッシュフィールドの更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
//	メッシュフィールドの設定処理
//=============================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type)
{
	PMESHFIELD pMeshField = &g_ameshfield[0];	// 先頭アドレスポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++, pMeshField++)
	{
		if (pMeshField->bUse == true)
		{// 使ってたら弾く
			continue;
		}

		// 分割数を頂点数に合わせる
		nSplitWidth++;
		nSplitDepth++;

		// メッシュフィールドの設定
		pMeshField->pos = pos;
		pMeshField->rot = rot;
		pMeshField->tex = D3DXVECTOR2(1.0f, 1.0f);
		pMeshField->type = type;
		pMeshField->nSplitWidth = nSplitWidth;
		pMeshField->nSplitDepth = nSplitDepth;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nSplitWidth * nSplitDepth,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshField->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxDepth = 0; nCntVtxDepth < nSplitDepth; nCntVtxDepth++)
		{
			for (int nCntVtxWidht = 0; nCntVtxWidht < nSplitWidth; nCntVtxWidht++)
			{
				pVtx[nCntVtxWidht].pos = D3DXVECTOR3(fWidth * nCntVtxWidht, 0.0f, -fDepth * nCntVtxDepth);
				pVtx[nCntVtxWidht].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVtxWidht].tex = D3DXVECTOR2(pMeshField->tex.x * nCntVtxWidht, pMeshField->tex.y * nCntVtxDepth);
			}

			pVtx += nSplitWidth;
		}

		// 頂点バッファをアンロックする
		pMeshField->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * (nSplitWidth * 2 * (nSplitDepth - 1) + ((nSplitDepth - 2) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pMeshField->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pMeshField->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntVtxDepth = 0; nCntVtxDepth < nSplitDepth - 1; nCntVtxDepth++)
		{
			for (int nCntVtxWidht = 0; nCntVtxWidht < nSplitWidth; nCntVtxWidht++)
			{
				if (nCntVtxDepth != 0 && nCntVtxWidht == 0)
				{// 奥行きが最初の行以外かつ幅が最初の行なら
					pIdx[0] = (nSplitWidth * (nCntVtxDepth + 1)) + nCntVtxWidht;
					pIdx++;
				}

				pIdx[0] = (nSplitWidth * (nCntVtxDepth + 1)) + nCntVtxWidht;
				pIdx[1] = nSplitWidth * nCntVtxDepth + nCntVtxWidht;

				pIdx += 2;

				if (nCntVtxWidht == nSplitWidth - 1 && nCntVtxDepth != nSplitDepth - 2)
				{// 幅が最後の行だったら
					pIdx[0] = nSplitWidth * nCntVtxDepth + nCntVtxWidht;
					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		pMeshField->pIdxBuff->Unlock();

		pMeshField->bUse = true;
		break;
	}
}

//=============================================================================
//	メッシュフィールドの当たり判定処理
//=============================================================================
PMESHFIELD CollisionMeshField(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove)
{
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXVECTOR3 vecNor, VecMove;				// 各種ベクトル
	PMESHFIELD pMeshField = &g_ameshfield[0];	// 先頭アドレスポインタ
	PMESHFIELD pRideMeshField = NULL;			// 乗っているメッシュ
	float fRidePosY = 0.0f;						// 乗っているメッシュの高さ

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++, pMeshField++)
	{
		if (pMeshField->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMeshField->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshField->rot.y, pMeshField->rot.x, pMeshField->rot.z);
		D3DXMatrixMultiply(&pMeshField->mtxWorld, &pMeshField->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pMeshField->pos.x, pMeshField->pos.y, pMeshField->pos.z);
		D3DXMatrixMultiply(&pMeshField->mtxWorld, &pMeshField->mtxWorld, &mtxTrans);

		if (pMeshField->mtxParent != NULL)
		{// もし親のマトリックスがあればかけ合わせる
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMeshField->mtxWorld);
			D3DXMatrixMultiply(&pMeshField->mtxWorld, &pMeshField->mtxWorld, pMeshField->mtxParent);
		}

		for (int nCntVtxDepth = 0; nCntVtxDepth < pMeshField->nSplitDepth - 1; nCntVtxDepth++)
		{
			for (int nCntVtxWidht = 0; nCntVtxWidht < pMeshField->nSplitWidth - 1; nCntVtxWidht++)
			{
				D3DXVECTOR3 PosA, PosB, PosC;		// 各頂点位置格納用

				// 位置と向きを反映した頂点座標を入れる
				D3DXVec3TransformCoord(&PosA, &pVtx[nCntVtxWidht].pos, &pMeshField->mtxWorld);
				D3DXVec3TransformCoord(&PosB, &pVtx[nCntVtxWidht + pMeshField->nSplitWidth].pos, &pMeshField->mtxWorld);
				D3DXVec3TransformCoord(&PosC, &pVtx[nCntVtxWidht + pMeshField->nSplitWidth + 1].pos, &pMeshField->mtxWorld);

				// 3辺の境界線ベクトルを算出
				D3DXVECTOR3 vecLineA = PosB - PosA;
				D3DXVECTOR3 vecLineB = PosC - PosB;
				D3DXVECTOR3 vecLineC = PosA - PosC;

				// それぞれの始点から位置へのベクトルを算出
				D3DXVECTOR3 vecToPosA = *pPos - PosA;
				D3DXVECTOR3 vecToPosB = *pPos - PosB;
				D3DXVECTOR3 vecToPosC = *pPos - PosC;

				// 正規化
				D3DXVec3Normalize(&vecLineA, &vecLineA);
				D3DXVec3Normalize(&vecLineB, &vecLineB);
				D3DXVec3Normalize(&vecLineC, &vecLineC);

				if ((vecLineA.z * vecToPosA.x) - (vecLineA.x * vecToPosA.z) < 0 &&
					(vecLineB.z * vecToPosB.x) - (vecLineB.x * vecToPosB.z) < 0 &&
					(vecLineC.z * vecToPosC.x) - (vecLineC.x * vecToPosC.z) < 0)
				{// もし全ての境界線ベクトルの内側にいたら

					// 面の法線
					D3DXVECTOR3 vecNor = { (-vecLineA.y * vecLineB.z) - (-vecLineA.z * vecLineB.y), (-vecLineA.z * vecLineB.x) - (-vecLineA.x * vecLineB.z), (-vecLineA.x * vecLineB.y) - (-vecLineA.y * vecLineB.x) };

					D3DXVec3Normalize(&vecNor, &vecNor);	// 正規化

					// 高さ
					float fPosY = PosA.y - (((vecNor.x * pPos->x) - (vecNor.x * PosA.x)) + ((vecNor.z * pPos->z) - (vecNor.z * PosA.z)) / vecNor.y);

					if (pPos->y <= fPosY)
					{
						if (pRideMeshField != NULL)
						{
							if (fRidePosY > fPosY)
							{
								break;
							}
						}

						fRidePosY = fPosY;
						pRideMeshField = pMeshField;
						break;
					}
				}

				// 位置と向きを反映した頂点座標を入れる
				D3DXVec3TransformCoord(&PosB, &pVtx[nCntVtxWidht + 1].pos, &pMeshField->mtxWorld);

				// 3辺の境界線ベクトルを算出
				vecLineA = PosB - PosA;
				vecLineB = PosC - PosB;
				vecLineC = PosA - PosC;

				// それぞれの始点から位置へのベクトルを算出
				vecToPosA = *pPos - PosA;
				vecToPosB = *pPos - PosB;
				vecToPosC = *pPos - PosC;

				// 正規化
				D3DXVec3Normalize(&vecLineA, &vecLineA);
				D3DXVec3Normalize(&vecLineB, &vecLineB);
				D3DXVec3Normalize(&vecLineC, &vecLineC);

				if ((vecLineA.z * vecToPosA.x) - (vecLineA.x * vecToPosA.z) > 0 &&
					(vecLineB.z * vecToPosB.x) - (vecLineB.x * vecToPosB.z) > 0 &&
					(vecLineC.z * vecToPosC.x) - (vecLineC.x * vecToPosC.z) > 0)
				{// もし全ての境界線ベクトルの内側にいたら

					D3DXVECTOR3 vecNor = { (vecLineB.y * -vecLineA.z) - (vecLineB.z * -vecLineA.y), (vecLineB.z * -vecLineA.x) - (vecLineB.x * -vecLineA.z), (vecLineB.x * -vecLineA.y) - (vecLineB.y * -vecLineA.x) };
					
					D3DXVec3Normalize(&vecNor, &vecNor);

					float fPosY = PosA.y - (((vecNor.x * pPos->x) - (vecNor.x * PosA.x)) + ((vecNor.z * pPos->z) - (vecNor.z * PosA.z)) / vecNor.y);

					if (pPos->y <= fPosY)
					{
						if (pRideMeshField != NULL)
						{
							if (fRidePosY > fPosY)
							{
								break;
							}
						}

						fRidePosY = fPosY;
						pRideMeshField = pMeshField;
						break;
					}
				}

			}

			pVtx += pMeshField->nSplitWidth;		// ポインタをずらす
		}

		// 頂点バッファをアンロックする
		pMeshField->pVtxBuff->Unlock();
	}

	if (pRideMeshField != NULL)
	{
		if (pRideMeshField->type != MESHFIELDTYPE_000)
		{
			pPos->y = fRidePosY;
			pMove->y = 0.0f;
		}

		return pRideMeshField;
	}

	return NULL;
}

//=============================================================================
//	メッシュの設定処理
//=============================================================================
void SetMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nVtx, D3DXVECTOR3* VtxPos, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type, D3DXMATRIX *pmtxParent)
{
	PMESHFIELD pMeshField = &g_ameshfield[0];

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++, pMeshField++)
	{
		if (pMeshField->bUse == true)
		{
			continue;
		}

		// メッシュフィールドの設定
		pMeshField->pos = pos;
		pMeshField->rot = rot;
		pMeshField->type = type;
		pMeshField->nSplitWidth = nSplitWidth;
		pMeshField->nSplitDepth = nSplitDepth;
		pMeshField->mtxParent = pmtxParent;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshField->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < nVtx; nCntVtx++)
		{
			pVtx[nCntVtx].pos = *VtxPos;
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntVtx, 1.0f * nCntVtx);

			VtxPos++;
		}

		// 頂点バッファをアンロックする
		pMeshField->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * (nSplitWidth * 2 * (nSplitDepth - 1) + ((nSplitDepth - 2) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pMeshField->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pMeshField->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntVtxDepth = 0; nCntVtxDepth < nSplitDepth - 1; nCntVtxDepth++)
		{
			for (int nCntVtxWidht = 0; nCntVtxWidht < nSplitWidth; nCntVtxWidht++)
			{
				if (nCntVtxDepth != 0 && nCntVtxWidht == 0)
				{// 奥行きが最初の行以外かつ幅が最初の行なら
					pIdx[0] = (nSplitWidth * (nCntVtxDepth + 1)) + nCntVtxWidht;
					pIdx++;
				}

				pIdx[0] = (nSplitWidth * (nCntVtxDepth + 1)) + nCntVtxWidht;
				pIdx[1] = nSplitWidth * nCntVtxDepth + nCntVtxWidht;

				pIdx += 2;

				if (nCntVtxWidht == nSplitWidth - 1 && nCntVtxDepth != nSplitDepth - 2)
				{// 幅が最後の行だったら
					pIdx[0] = nSplitWidth * nCntVtxDepth + nCntVtxWidht;
					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		pMeshField->pIdxBuff->Unlock();

		pMeshField->bMesh = true;
		pMeshField->bUse = true;
		break;
	}
}

//=============================================================================
//	メッシュの読み込み処理
//=============================================================================
void LoadMesh(const char* pFileName, MeshInfo* MeshInfo)
{
	FILE* pFile = fopen(pFileName, "rb");		// 引数のファイルを開く

	if (pFile == NULL)
	{// 開けなかったら終了
		return;
	}

	// ファイルから取得した情報を格納
	fread(&MeshInfo->nVtx, sizeof(int), 1, pFile);
	fread(&MeshInfo->nSplitWidth, sizeof(int), 1, pFile);
	fread(&MeshInfo->nSplitDepth, sizeof(int), 1, pFile);
	fread(&MeshInfo->VtxPos[0], sizeof(D3DXVECTOR3), MeshInfo->nVtx, pFile);

	fclose(pFile);
}