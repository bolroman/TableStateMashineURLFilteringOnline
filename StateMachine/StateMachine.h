/*
 * StateMachine.h
 *
 *  Created on: Jan 12, 2011
 *      Author: yotamhc
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "../Common/Types.h"
#include "../Common/PatternTable.h"
#include "../Common/HashMap/HashMap.h"
#include "../AhoCorasick/ACTypes.h"

typedef struct {
	int totalFailures;
	int totalGotos;
} MachineStats;


typedef struct {
	StateTable *states;
	PatternTable *patternTable;
	Node *nodeTable[100000];
	STATE_PTR_TYPE firstLevelLookupTable[256];
	HashMap *secondLevelLookupHash;
	int isLoadedFromDump;
	int isSimple;
#ifdef PRINT_COMPRESSED_MACHINE
	int *depthmap;
#endif
} StateMachine;

//int getNextState(StateMachine *machine, State *current, char *str, int length, int *idx, NextStateResult *result);
int match(StateMachine *machine, char *input, int length, int verbose, MachineStats *stats);
void compressStateTable(StateMachine *machine);
int getStateID(State *state);
#ifdef COUNT_CALLS
void printCounters();
#endif
#ifdef X_RATIO
void printXRatio();
#endif

#endif /* STATEMACHINE_H_ */
