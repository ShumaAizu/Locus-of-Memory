//==========================================
// 
// 3Dポリゴンの描画[shadow.h]
// 
//==========================================
#ifndef _SHADOW_H_
#define	_SHADOW_H_

// プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

#endif
