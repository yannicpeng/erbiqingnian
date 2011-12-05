//----------------------------------------------------------------------------
// FILE: difficulty.h
//
// (C)Copyright 2006, Flagship Studios. All rights reserved.
//----------------------------------------------------------------------------

#ifndef __DIFFICULTY_H_
#define __DIFFICULTY_H_

//----------------------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------------------
	#include "../data/excel/difficulty_hdr.h"		// auto generated by difficulty.xls

//----------------------------------------------------------------------------
// Types
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
struct DIFFICULTY_DATA
{
	char szName[ DEFAULT_INDEX_SIZE ];	// name
	WORD wCode;							// code
	int nUnlockedString;				// string to display when this difficulty is unlocked
};

struct UNIT;
struct STATS_FILE;
typedef unsigned int STATS_VERSION;

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------

const DIFFICULTY_DATA *DifficultyGetData(
	int nDifficulty);

int DifficultyGetCurrent(
	UNIT *pPlayer);

void DifficultySetCurrent(
	UNIT *pPlayer,
	int nDifficulty);
	
int DifficultyGetMax(
	UNIT *pPlayer);

void DifficultySetMax(
	UNIT *pPlayer,
	int nDifficulty);

BOOL DifficultyValidateValue(
	int &nDifficulty,
	UNIT *pPlayer = NULL,
	struct GAMECLIENT *pClient = NULL);

enum STAT_VERSION_RESULT s_VersionStatDifficulty(
	STATS_FILE &tStatsFile,
	STATS_VERSION &nStatsVersion,
	UNIT *pUnit);

#endif