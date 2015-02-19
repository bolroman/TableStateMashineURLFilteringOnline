#include "StateMachineTest.h"


int CompressURLsStateMachine (char *path,StateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][100])
{
	/* load URLs to Memory */
	int number_of_urls = *num_of_urls;
	char *urls[30000];
	FILE *f;
	f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Cannot read file %s\n", path);
		exit(1);
	}
	size_t read;
	char * line = NULL;
	size_t len = 0;
    int next = 0;
	while(!feof(f))
	{
	  read = getline(&line, &len, f);
	  if (read!=-1)
	  {
		urls[next]= (char*)malloc(len*sizeof(char)+1);
		strncpy(urls[next], line, len);
		urls[next][len] = '\0';
		next++;
		if(next==number_of_urls) {
			break;
		}
	  }
	}

	fclose(f);

	int z;
	for(z=0;z<number_of_urls;z++)
	{

		int length = strlen(urls[z]);
		urls[z][length-1]=0;
	}

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	//FILE *file = fopen("StateOutput.txt", "w");

	int x;
	for(x=0;x < 1/*number_of_urls*/;x++)
	{
		int length = strlen(urls[x]);
		int idx = 0;
		Node *curNode;
        int P[100];
        Node *N[10000];
        int ymins[10000];
        int realIdx = -1;

    	State *nextState;
    	NextStateResult next;
    	State **stateTable = machine->states->table;
    	STATE_PTR_TYPE nxtid;
    	State* s = machine->states->table[0];

    	int prevIdx = idx-1;

    	//init members
    	int ind;
    	for(ind=0;ind<length;ind++)
    	{
        	//length of next minimal encoded string = length of current minimum encoded string + sizeof next symbol
			P[ind+1]=P[ind]+ symbolArr[urls[x][ind]].size;
			//reset anchor index for future use
			ymins[ind+1]=-1*urls[x][ind];
    	}

        while (idx < length)
		{
        	if (prevIdx<idx)
        	{
        		realIdx++;
        		prevIdx = idx;
        	}

			next.fail = FALSE;
			next.match = FALSE;

			if (s == stateTable[0]) {
				nxtid = GET_FIRST_LEVEL_STATE(machine, urls[x][idx]);
				if (nxtid == LOOKUP_VALUE_NOT_FOUND) {
					next.fail = TRUE;
					nextState = s;
				} else {
					nextState = GET_STATE_POINTER_FROM_ID(machine, nxtid);
					if (nextState == s)
						next.fail = TRUE;
					else
						(idx)++;
				}
				// THIS PREVENTS ONE CHAR MATCHES!
			} else {

				switch (GET_STATE_TYPE(s))
				{
				case STATE_TYPE_LOOKUP_TABLE:
				{
					uchar *table = &((s)[5 + 2 + 32]);
					uchar c = (uchar)urls[x][idx];
					int s_idx = c * 2;

					if (table[s_idx] != (uchar)(-1) || table[s_idx + 1] != (uchar)(-1)) { /* There is a goto */
						int i, count, ind;
						(&next)->next = ((machine)->states->table[(int)((table[(int)s_idx + 1] << 8) | (table[(int)s_idx]))]);
						(&next)->match = (((&((s)[5 + 2]))[(c) / 8] >> ((c) % 8)) & 0x01);
						(&next)->stateID = (int)((table[(int)s_idx + 1] << 8) | (table[(int)s_idx]));


						if ((&next)->match) {
							count = 0;
							ind = 5 + 2;
							for (i = 0; i < 32; i++) {
								if (((s)[ind + i] & 1) != 0 && i * 8 < (int)c)
									count++;
								if (((s)[ind + i] & 2) != 0 && i * 8 + 1 < (int)c)
									count++;
								if (((s)[ind + i] & 4) != 0 && i * 8 + 2 < (int)c)
									count++;
								if (((s)[ind + i] & 8) != 0 && i * 8 + 3 < (int)c)
									count++;
								if (((s)[ind + i] & 16) != 0 && i * 8 + 4 < (int)c)
									count++;
								if (((s)[ind + i] & 32) != 0 && i * 8 + 5 < (int)c)
									count++;
								if (((s)[ind + i] & 64) != 0 && i * 8 + 6 < (int)c)
									count++;
								if (((s)[ind + i] & 128) != 0 && i * 8 + 7 < (int)c)
									count++;
								if (i * 8 + 8 >= (int)c)
									break;
							}
							(&next)->pattern = (machine->patternTable)->patterns[((s)[2] << 8) | (s)[3]][count];
						}
						(&next)->fail = 0;
						(idx)++;
					} else { /* Failure */
						(&next)->next = ((machine)->states->table[(int)(((s)[5 + 1] << 8) | ((s)[5]))]);
						(&next)->match = 0;
						(&next)->fail = 1;
					}
				}
					break;
				case STATE_TYPE_PATH_COMPRESSED:
					do {
						StateHeader *header;
						int i, j, count, plength, failed, matched , id;
						uchar *chars;
						char *pattern;
						uchar *matches, *flags;

						pattern = NULL;
						header = (StateHeader*)(s);
						plength = (header->size << 8) | ((s)[1] & 0x0FF);
						chars = (uchar*)(&((s)[GOTO_IDX + 2]));
						matches = &((s)[GOTO_IDX + 2 + plength]);
						flags = &((s)[GOTO_IDX + 2 + plength + (int)ceil(plength / 8.0)]);

						failed = FALSE;
						matched = FALSE;
						for (i = 0; i < plength; i++) {
							if (idx >= (length)) {
								break;
							}
							if ((uchar)(urls[x])[idx] != chars[i]) {
								failed = TRUE;
								break;
							} else {
								if (GET_1BIT_ELEMENT(matches, i)) {
									/* It is a match */
									matched = TRUE;

									if (0) {
										/* Count pattern index */
										count = 0;
										for (j = 0; j < i; j++) {
											if (GET_1BIT_ELEMENT(matches, j))
												count++;
										}
										id = ((s)[2] << 8) | (s)[3];

										//RB
										pattern = concat_strings_nofree(pattern, (machine->patternTable)->patterns[id][count].pattern);
									}
								}
								(idx) = idx + 1;
							}
						}

						if (failed) {
							switch (GET_2BITS_ELEMENT(flags, i)) {
							case PTR_TYPE_REGULAR:
								/* Search ptr is ptrs array */
								count = 0;
								for (j = 0; j < i; j++) {
									if (GET_2BITS_ELEMENT(flags, j) == PTR_TYPE_REGULAR)
										count++;
								}
								/* The pointer is in: */
								j = GOTO_IDX + 2 + plength + (int)ceil(plength / 8.0) + (int)ceil(plength / 4.0) + count * 2;
								(&next)->next = GET_STATE_POINTER_FROM_ID(machine, ((s)[j + 1] << 8) | ((s)[j]));
								(&next)->stateID = ((s)[j + 1] << 8) | ((s)[j]);
								break;
							case PTR_TYPE_LEVEL0:
								(&next)->next = (machine)->states->table[0];
								(&next)->stateID = 0;
								break;
							case PTR_TYPE_LEVEL1:
								(&next)->next = GET_STATE_POINTER_FROM_ID(machine, GET_FIRST_LEVEL_STATE(machine, (urls[x])[idx - 1]));
								(&next)->stateID = GET_FIRST_LEVEL_STATE(machine, (urls[x])[idx - 1]);
								break;
							case PTR_TYPE_LEVEL2:
								(&next)->next = GET_STATE_POINTER_FROM_ID(machine, GET_SECOND_LEVEL_STATE(machine, (urls[x])[idx - 2], (urls[x])[idx - 1]));
								(&next)->stateID = GET_SECOND_LEVEL_STATE(machine, (urls[x])[idx - 2], (urls[x])[idx - 1]);
								break;
							default:
								fprintf(stderr, "Unknown pointer type in a path compressed state: %d\n", GET_2BITS_ELEMENT(flags, i));
								exit(1);
							}
							(&next)->fail = TRUE;
							(&next)->match = matched;
							//RB
							(&next)->pattern.pattern = pattern;
						} else {
							(&next)->next = GET_STATE_POINTER_FROM_ID(machine, ((s)[GOTO_IDX + 1] << 8) | ((s)[GOTO_IDX]));\
							(&next)->match = matched;
							(&next)->fail = FALSE;

							//RB
							(&next)->pattern.pattern = pattern;
						}
					} while (FALSE);

					break;
				case STATE_TYPE_LINEAR_ENCODED:
					getNextState_LE(s, urls[x], length, &idx, &next, machine, machine->patternTable, 0);
					break;
				case STATE_TYPE_BITMAP_ENCODED:
					getNextState_BM(s, urls[x], length, &idx, &next, machine, machine->patternTable, 0);
					break;
				}

				nextState = next.next;
			}

			if ((s == nextState) && (s == (stateTable[0])) && (next.fail)) {
				(idx)++;
			}
			s = nextState;

			curNode = machine->nodeTable[next.stateID];
			if (next.match)
			{
				printf("%s\n",next.pattern.pattern);

				N[realIdx+1]=curNode;

				//foreach anchor in this prefix do:
				int y;
				for (y=0; y<curNode->numMatches; y++)
				{
					//if replacement of pattern[y] by codeHoffman[y] is less than size of endoded pattern if i leave next symbol.
					if(P[realIdx-strlen(curNode->patterns[y])+1]+curNode->huffman_len[y]<P[realIdx+1])
					{
						//remember anchor's index
						ymins[realIdx+1]=y;
					}
				}
			}
			//if exist anchor which hoffmanCode minimizes total lenght of encoded string than.
			if(ymins[realIdx+1]>=0)
			{
				//update size of total encoded string
				P[realIdx+1]=P[realIdx-strlen(curNode->patterns[ymins[realIdx+1]])+1]+curNode->huffman_len[ymins[realIdx+1]];
			}

		}

		int z=length;

		int resultCodeLen=0;
		int resultIndex=0;

		while(z>0)
		{
			if(ymins[z]>=0)
			{
				ConcutByteArrayToByteArray(results[x], &resultIndex,
						&resultCodeLen, N[z]->huffman[ymins[z]], N[z]->huffman_len[ymins[z]]);

				z-=strlen(N[z]->patterns[ymins[z]]);
			}
			else
			{
				if (symbolArr[ymins[z]*-1].size != 0)
				{
					ConcutByteArrayToByteArray(results[x], &resultIndex,
							&resultCodeLen, symbolArr[ymins[z]*-1].symbolCode, symbolArr[ymins[z]*-1].size);
				}
				z--;
			}
		}

//		//RomanOutput
//		fprintf(file, "\nURL - %s\n", urls[x]);
//		int index;
//		for(index=0;index<resultIndex;index++)
//		{
//			fprintf(file, "%02X", results[x][index]);
//		}
//		//****************

	}

//	fclose(file);


	gettimeofday(&stop, NULL);
    int count = 0;

    int c;
    for(c=0;c<number_of_urls;c++)
    {
    	count+=strlen(urls[c]);
    }
	*num_of_urls=count;

	return 1000000*(stop.tv_sec-start.tv_sec)+ stop.tv_usec - start.tv_usec;
}
