//----------------------------------------------------------------------------
// FILE: quest_talk.h
//
// This is a template quest. The player first talks to Cast Giver (in quest.xls),
// and then must speak with Cast Rewarder. Optionally, the quest giver gives 
// items to the player to take to the rewarder.
// 
// author: Chris March, 3-6-2007
//
// (C)Copyright 2007, Flagship Studios. All rights reserved.
//----------------------------------------------------------------------------

#ifndef __QUEST_DONATION_H
#define __QUEST_DONATION_H_

//----------------------------------------------------------------------------
// FORWARD REFERENCES	
//----------------------------------------------------------------------------
struct QUEST_FUNCTION_PARAM;

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void QuestDonateInit(
				   const QUEST_FUNCTION_PARAM &tParam);

void QuestDonateFree(
				   const QUEST_FUNCTION_PARAM &tParam);

#endif