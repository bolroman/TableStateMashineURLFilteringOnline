/*
 * PatternTable.c
 *
 *  Created on: Jan 14, 2011
 *      Author: yotamhc
 */

#include <stdlib.h>
#include <stdio.h>
#include "PatternTable.h"

#define MAX_PATTERN_LENGTH 1024

PatternTable *patterntable_create(int size) {
	PatternTable *table = (PatternTable*)malloc(sizeof(PatternTable));
	Pattern** patterns = (Pattern**)malloc(sizeof(Pattern**) * size);
	//char ***pats = (char***)malloc(sizeof(char**) * size);

	table->patterns = patterns;
	table->size = size;
	return table;
}

void patterntable_destroy(PatternTable *table) {
	//char ***patterns;
	Pattern ** patterns;
	int i, j;

	patterns = table->patterns;

	for (i = 0; i < table->size; i++) {
		if (patterns[i]->pattern == NULL)
			continue;
		j = 0;
		while (patterns[i][j].pattern != NULL) {
			free(patterns[i][j].pattern);
			j++;
		}
	}
	free(patterns);
	free(table);
}

void patterntable_add_state(PatternTable *table, STATE_PTR_TYPE_WIDE state, int numPatterns) {
	Pattern *statePatterns;
	int i;

	if (numPatterns == 0)
		return;

	statePatterns = (Pattern *)malloc(sizeof(Pattern) * (numPatterns + 1));
	for (i = 0; i < numPatterns + 1; i++) {
		statePatterns[i].pattern = NULL;
	}
	table->patterns[state] = statePatterns;
}

#define TO_HEX(val) \
	(((val) >= 10) ? ('A' + ((val) - 10)) : (char)('0' + (val)))

char *createPattern(char *pattern, int len) {
	char buff[MAX_PATTERN_LENGTH];
	char *res;
	int i, j;


	for (i = 0, j = 0; i < len; i++)
	{
		if (pattern[i] >= 32 && pattern[i] < 127) {
			buff[j++] = pattern[i];
		} else {
			buff[j++] = '|';
			buff[j++] = TO_HEX((pattern[i] & 0x0F0) >> 4);
			buff[j++] = TO_HEX(pattern[i] & 0x00F);
			buff[j++] = '|';
		}
	}
	buff[j++] = '\0';
	res = (char*)malloc(sizeof(char) * j);
	strcpy(res, buff);
	return res;
}

void patterntable_add_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *pattern, int len) {
	char *cpy;

	if (table->patterns[sourceState][idx].pattern != NULL) {
		patterntable_update_pattern(table, sourceState, idx, pattern, len);
		return;
	}

	cpy = createPattern(pattern, len);

	//RB here
	table->patterns[sourceState][idx].pattern = cpy;
}

void patterntable_update_pattern(PatternTable *table, STATE_PTR_TYPE_WIDE sourceState, int idx, char *more, int len) {
	int oldlen, newlen, freeOld;
	char *old, *next, *new;

	new = createPattern(more, len);
	len = strlen(new);

	freeOld = 1;

	//RB
	old = table->patterns[sourceState][idx].pattern;
	if (old == NULL) {
		old = "\0";
		freeOld = 0;
	}
	oldlen = strlen(old);

	newlen = oldlen + len + 1;

	next = (char*)malloc(sizeof(char) * (newlen + 1));

	strcpy(next, old);
	next[oldlen] = ';';
	strcpy(&(next[oldlen + 1]), new);

	//RB
	table->patterns[sourceState][idx].pattern = next;

	if (freeOld) {
		free(old);
	}
}
