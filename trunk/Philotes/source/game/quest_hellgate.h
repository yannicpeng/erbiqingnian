//----------------------------------------------------------------------------
// FILE: quest_hellgate.h
//
// (C)Copyright 2005, Flagship Studios. All rights reserved.
//----------------------------------------------------------------------------

#ifndef __QUEST_HELLGATE_H_
#define __QUEST_HELLGATE_H_

//----------------------------------------------------------------------------
// FORWARD REFERENCES	
//----------------------------------------------------------------------------
struct GAME;
struct QUEST;
struct QUEST_FUNCTION_PARAM;

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void QuestInitHellgate(
	const QUEST_FUNCTION_PARAM &tParam);

void QuestFreeHellgate(
	const QUEST_FUNCTION_PARAM &tParam);
	
#endif