//=============================================================================
//
//	デバッグ表示処理 [debugproc.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(const char* fmt, ...);

#endif _DEBUGPROC_H_
