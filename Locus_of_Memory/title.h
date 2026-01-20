//========================================================
// 
// タイトルのメイン処理[title.h]
// Author : KAJINO HIYORI
// 
//========================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

// 操作方法の管理
typedef enum
{ 
	OPERATIONTYPE_1P = 0,	// 1PPLAY
	OPERATIONTYPE_2P,		// 2PPLAY
	OPERATIONTYPE_KEYBOARD,	// KEYBOARDPLAY
	OPERATIONTYPE_MAX
}OPERATIONTYPE;

// プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetOperationType(OPERATIONTYPE type);	// 操作方法の決定
OPERATIONTYPE GetOperationType(void);		// 操作方法の取得
#endif
