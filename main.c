#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>
#include <string.h>
#include <iconv.h>

int getElapsedTime(struct timeval start, struct timeval end)
{
	 gettimeofday(&end, NULL);
	 return ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
}

void showElapsedTime(struct timeval start, struct timeval end)
{
	int elapsed = getElapsedTime(start, end);
	printf("[INFO] %d μs\n", elapsed);
}

int isAnagramm(char *word1, char *word2)
{
	int i,j,found,checks = 0;
	int len = strlen(word1);
	int len2 = len;
	char *temp = malloc(len);
	strcpy(temp, word1);

	for (i = len; i != 0; i--)
	{
		found = 0;
		for (j = len2; j != 0 ; j--)
		{
			checks++;
			if (word2[i-1] == temp[j-1])
			{
				memmove(&temp[j-1], &temp[j], len2 - j + 1);
				found = 1;
				len2--;
				break;
			}
		}
		if (found == 0)
		{
			return 0;
		}
	}
	if (checks == len)
	{
		return 0;//same word
	}

	return 1;
}

char *stringAppend(char *firstString, char *secondString)
{
	char *resultString;
	size_t firstStringLength = strlen(firstString);
	size_t secondStringLength = strlen(secondString);

	if((resultString = malloc(firstStringLength + secondStringLength + 1 + 1)) != NULL)
	{
	    resultString[0] = '\0';
	    strcat(resultString, firstString);
		strcat(resultString, ",");
	    strcat(resultString, secondString);
	}
	else
	{
	    fprintf(stderr, "[ERROR] Could not join strings - memory allocation failed!\n");
		exit(EXIT_FAILURE);
	}

	return resultString;
}

/*
 localedef -c -f UTF-8 -i et_EE et_EE.UTF8
 export LC_ALL=et_EE.UTF8
 export LANG=et_EE
 bash
 seq 100 | xargs -Iz ./anagramm lemmad.txt žiirü
 */
//gcc main.c -o anagramm ; ./anagramm lemmad.txt türgi
int main(int argc, char** argv)
{
	struct timeval st, et;
	gettimeofday(&st, NULL);
	iconv_t cd = iconv_open("windows-1257", "UTF-8");
    int elapsed, i;

    setlocale(LC_ALL, "et_EE.UTF-8");

    if (cd == (iconv_t) -1)
    {
    	fprintf(stderr, "[ERROR] Can't use iconv_open!\n");
    	showElapsedTime(st, et);
    	exit(EXIT_FAILURE);
    }

    if (argc < 3)
	{
		fprintf(stderr, "[USAGE] %s path/to/dictionary anagram\n", argv[0]);
		iconv_close(cd);
		showElapsedTime(st, et);
		exit(EXIT_FAILURE);
	}

    char searchInput[sizeof(argv[2])];
    char searchString[sizeof(argv[2])];


    char *searchInputPointer = searchInput;
    char *searchStringPointer = searchString;

    size_t inbytesleft = sizeof(searchInput) - 1;
    size_t outbytesleft = sizeof(searchString) - 1;

    strcpy(searchInput, argv[2]);

    int isIconvNeeded = 0;

    iconv(cd, &searchInputPointer, &inbytesleft, &searchStringPointer, &outbytesleft);
	iconv_close(cd);

	cd = iconv_open("UTF-8", "windows-1257");



	for(i = (sizeof(searchInput) - inbytesleft - 1); i != 0; i--)
	{
		if (searchString[i-1] != searchInput[i-1])
		{
			isIconvNeeded = 1;
			break;
		}
	}

	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;


	stream  = fopen(argv[1], "rb, ccs=windows-1257");

	if (stream == NULL)
	{
		fprintf(stderr, "[ERROR] can't open file %s!\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int results = 0;
	char *result = "";
	while ((nread = getline(&line, &len, stream)) != -1)
	{
		int currentLength = strlen(line);
		line[currentLength - 2] = '\0';
		currentLength = currentLength - 2;

		if (strlen(searchString) != currentLength)
		{
			continue;
		}

		if (isAnagramm(searchString, line) == 0)
		{
			continue;
		}

		if (isIconvNeeded != 0)
		{
			char convertedLine[sizeof(line)];
			char *linePointer = line;
			char *convertedLinePointer = convertedLine;

			size_t inbytesleft2 = sizeof(linePointer) - 1;
			size_t outbytesleft2 = sizeof(convertedLine) - 1;

			iconv(cd, &linePointer, &inbytesleft2, &convertedLinePointer, &outbytesleft2);
			strcpy(line, convertedLine);
		}
		result = stringAppend(result, line);
		results++;
	}

	free(line);
	fclose(stream);

	iconv_close(cd);



    elapsed = getElapsedTime(st, et);
    printf("%d%s\n", elapsed, result);

    return (EXIT_SUCCESS);
}
