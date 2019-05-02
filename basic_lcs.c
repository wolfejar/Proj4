#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#define WIKI_ARRAY_SIZE 10
#define WIKI_LINE_SIZE 2001

static int **_matrix;
static int _matrix_row_size = 0;
static int _matrix_collumn_size = 0;

char* resultingSubstrings [WIKI_ARRAY_SIZE];
char* LCS (char * s1, char * s2, char ** longest_common_substring);

//load the lines into an array
char  **wiki_array;
char **longestSub;

void readToMemory();
void printResults();
void printToFile();

int main()
{
	    struct timeval time1;
    	struct timeval time2;
    	struct timeval time3;
    	struct timeval time4;
    	double e1, e2, e3;
    	int numSlots, Version = 1; //base = 1, pthread = 2, openmp = 3, mpi = 4

    	gettimeofday(&time1, NULL);
    	readToMemory();
    	gettimeofday(&time2, NULL);

    	//time to read to memory
    	e1 = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
    	e1 += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
    	printf("Time to read full file to Memory: %f\n", e1);

    	gettimeofday(&time3, NULL);

    	int i;
    	for(i = 0; i < WIKI_ARRAY_SIZE /*- 1*/ ; i++)
    	{
		      printf("%d-%d: %s", i ,i + 1 ,"lines submitted to LCS");
          printf("\n");
       		resultingSubstrings[i] = LCS((void*)wiki_array[i], (void*)wiki_array[i+1], longestSub);
       		//longestSub++;
    	}
	  printToFile();

   	gettimeofday(&time4, NULL);

   	//time to find all longest substrings
   	e2 = (time4.tv_sec - time3.tv_sec) * 1000.0; //sec to ms
   	e2 += (time4.tv_usec - time3.tv_usec) / 1000.0; // us to ms
   	printf("Time find all Substrings: %f\n", e2);

   	//total elapsed time between reading and finding all longest substrings
   	e3 = (time4.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
   	e3 += (time4.tv_usec - time1.tv_usec) / 1000.0; // us to ms
   	printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), e3);
}

void readToMemory()
{
	int nlines, maxlines = 10;
	int k, n, err, *count, nthreads = 24;
	int i;
	double nchars = 0;
	FILE *fd;

	 //Adding malloc for space
	wiki_array = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i = 0; i < WIKI_ARRAY_SIZE; i++)
	{
	  	wiki_array[i] = malloc(2001);
	}
	//saved results
	longestSub = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i = 0; i < WIKI_ARRAY_SIZE -1; i++)
	{
	  	longestSub[i] = malloc(2001);
	}

	fd = fopen("testData10.txt", "r");
	nlines = -1;
	do
	{
		err = fscanf(fd, "%[^\n]\n", wiki_array[++nlines]);
		if(wiki_array[nlines] != NULL)
			nchars += (double) strlen(wiki_array[nlines]);
	}
	while (err != EOF && nlines < WIKI_ARRAY_SIZE);

	fclose(fd);
	printf("Read in %d lines averaging %.01f chars/line\n", nlines, nchars / nlines);
}

void printToFile()
{
	FILE *f = fopen("LargestCommonSubstrings.txt", "w");
	if (f == NULL)
	{
    		printf("Error opening LargestCommonSubstrings.txt!\n");
    		exit(1);
	}

	//longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
	int i;
	for(i = 0; i < WIKI_ARRAY_SIZE /*- 2*/; i++)
	{
		fprintf(f, "%d-%d: %s", i, i + 1, resultingSubstrings[i]);
		fprintf(f, "\n");
	}

	fclose(f);
}

void printResults()
{
  int i;
	longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
  	for(i = 0; i <= WIKI_ARRAY_SIZE - 2; i++)
  	{
      		printf("%d-%d: %s", i , i + 1 ,longestSub[i]);
      		printf("\n");
  	}
}

 static void init(int s1_length, int s2_length)
 {
    if (s1_length+1 > _matrix_row_size || s2_length+1 > _matrix_collumn_size)
    {
		    /* free matrix */
		  int i;
		  for (i = 0; i < _matrix_row_size; i++)
	    		 free(_matrix[i]);
		        free(_matrix);

		  /* malloc matrix */
		  _matrix = (int **)malloc((s1_length+1) * sizeof(int*));
		  for (i = 0; i < s1_length+1; i++)
	    		 _matrix[i] = (int *)malloc((s2_length+1) * sizeof(int));

		        _matrix_row_size = s1_length+1;
		  _matrix_collumn_size = s2_length+1;
    }
    	int i;
    	for (i = 0; i <= s1_length; i++)
		_matrix[i][s2_length] = 0;

    	int j;
    	for (j = 0; j <= s2_length; j++)
		_matrix[s1_length][j] = 0;
}

char* LCS(char *s1, char *s2, char **longest_common_substring)
{
    	int s1_length = strlen(s1);
    	int s2_length = strlen(s2);
    	init(s1_length, s2_length);

    	int max_len = 0, max_index_i = -1;
    	int i,j;
    	for (i = s1_length-1; i >= 0; i--)
    	{
    		for (j = s2_length-1; j >= 0; j--)
		{
    	    if (s1[i] != s2[j])
	    		{
    				_matrix[i][j] = 0;
    				continue;
    	    }

    	    _matrix[i][j] = _matrix[i+1][j+1] + 1;

    	    if (_matrix[i][j] > max_len)
	    		{
    				max_len = _matrix[i][j];
    				max_index_i = i;
    	    }
    		}
    	}
    	if (longest_common_substring != NULL)
    	{
		      *longest_common_substring = malloc(sizeof(char) * (max_len+1));
		      strncpy(*longest_common_substring, s1+max_index_i, max_len);
		      (*longest_common_substring)[max_len] = '\0';
		      printf("%s\n", *longest_common_substring);
          printf("%d\n", max_len);
    	}
    	return *longest_common_substring;
}
