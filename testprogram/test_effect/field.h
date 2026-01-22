//==========================================
// 
// 3Dポリゴンの描画[field.h]
// 
//==========================================
#ifndef _FIELD_H_
#define	_FIELD_H_

#include "main.h"

// プロトタイプ宣言
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
D3DXVECTOR3 GetFieldPos(void);

#endif
