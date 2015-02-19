#include "TableStateMachineTest.h"

typedef struct{
	char* pattern;
	int patternLen;
	unsigned char* hoffmanCode;
	int hoffmanCodeLen;
}Anchors;

typedef struct{
	Anchors anchors[100];
	int sizeOfList;
}AnchorsList;



void GetLongestAnchor(TableStateMachine *machine, char* input, AnchorsList* curNode)
{
	STATE_PTR_TYPE_WIDE current, next;
	STATE_PTR_TYPE_WIDE *table;
	unsigned char *matches;
	char **patterns;
	int idx;
	int matched = 0;

	table = machine->table;
	matches = machine->matches;
	patterns = machine->patterns;
	idx = 0;
	current = 0;

	while (idx < strlen(input))
	{
		next = GET_NEXT_STATE(table, current, input[idx]);

		if (GET_1BIT_ELEMENT(matches, next))
		{
			if (patterns[next] != 0)
			{
    			Node* node = machine->nodeTable[next];
    			curNode->anchors[curNode->sizeOfList].pattern = node->patterns[0];
    			curNode->anchors[curNode->sizeOfList].hoffmanCode = node->huffman[0];
    			curNode->anchors[curNode->sizeOfList].hoffmanCodeLen = node->huffman_len[0];
    			curNode->anchors[curNode->sizeOfList].patternLen = node->patternsLen[0];

    			matched = 1;
			}

		}
		current = next;
		idx++;
	}
	if (matched ==1)
	{
		curNode->sizeOfList++;
	}
}

int CompressURLsMachine (char *path,TableStateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][50])
{
	/* load URLs to Memory */
	int number_of_urls = *num_of_urls;
	char *urls[10000];
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

	int x;

	//FILE *file = fopen("Tableoutput.txt", "w");

	for(x=0; x<number_of_urls; x++)
	{
		int length = strlen(urls[x]);
		int idx = 0;
		int current = 0;
		AnchorsList curNode;
        int P[100];
        AnchorsList N[100];
        int ymins[100];
    	curNode.sizeOfList = 0;

    	while (idx < length)
    	{
    		P[idx+1]=P[idx]+ symbolArr[urls[x][idx]].size;
    		ymins[idx+1]=-1*urls[x][idx];
    		next = GET_NEXT_STATE(machine->table, current, urls[x][idx]);
    		if (GET_1BIT_ELEMENT(machine->matches, next))
    		{
    			Node* node = machine->nodeTable[next];
    			curNode.sizeOfList = 0;
    			curNode.anchors[curNode.sizeOfList].pattern = node->patterns[0];
    			curNode.anchors[curNode.sizeOfList].hoffmanCode = node->huffman[0];
    			curNode.anchors[curNode.sizeOfList].hoffmanCodeLen = node->huffman_len[0];
    			curNode.anchors[curNode.sizeOfList].patternLen = node->patternsLen[0];
    			curNode.sizeOfList++;

    			if (machine->patterns[next] != 0)
    			{
    				char* longestPattern = machine->patterns[next];
    				int longestPatternLen = strlen(longestPattern);

    				if(longestPatternLen > 0)
    				{
						int i;
						char* longestPatternPtr = longestPattern;

						for(i=1;i < longestPatternLen-1; i++)
						{
							longestPatternPtr++;
							GetLongestAnchor(machine,(longestPatternPtr),&curNode);
						}
    				}
    			}

				N[idx+1]=curNode;

				int y;
				for (y=0; y<curNode.sizeOfList; y++)
				{
					if(P[idx-(curNode.anchors[y].patternLen)+1]+curNode.anchors[y].hoffmanCodeLen<P[idx+1])
					{
						ymins[idx+1]=y;
					}
				}
    		}

			if(ymins[idx+1]>=0)
			{
				P[idx+1]=P[idx-(curNode.anchors[ymins[idx+1]].patternLen)+1]+(curNode.anchors[ymins[idx+1]].hoffmanCodeLen);
			}

    		idx++;
    		current = next;
    	}


		int z=length;

		int resultCodeLen=0;
		int resultIndex=0;
		memset(results[x],0,50);

		while(z>0)
		{
			if(ymins[z]>=0)
			{
				ConcutByteArrayToByteArray(results[x], &resultIndex,
						&resultCodeLen, N[z].anchors[ymins[z]].hoffmanCode, N[z].anchors[ymins[z]].hoffmanCodeLen);

				z-=strlen(N[z].anchors[ymins[z]].pattern);
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

		//RomanOutput
//		fprintf(file, "\nURL - %s\n", urls[x]);
//		int index;
//		for(index=0;index<resultIndex;index++)
//		{
//			fprintf(file, "%02X", results[x][index]);
//		}
		//****************

	}

	//fclose(file);


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
