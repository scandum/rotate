/*
	Copyright (C) 2014-2021 Igor van den Hoven ivdhoven@gmail.com
*/

/*
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	To compile use:
	
	gcc -O3 bench.c
	
	or
	
	g++ -O3 -w -fpermissive bench.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <math.h>

#include "rotate.h"

typedef void SRTFUNC(void *array, size_t left, size_t right);

long long utime()
{
	struct timeval now_time;

	gettimeofday(&now_time, NULL);

	return now_time.tv_sec * 1000000LL + now_time.tv_usec;
}

void test_sort(void *array, void *unsorted, void *valid, int max, int samples, int repetitions, int left, int right, const char *name, char *desc, size_t size)
{
	long long start, end, total, best, average;
	size_t rep, sam;
	int *pta = (int *) array, *ptv = (int *) valid, cnt;
	static int legend;

	if (*name == '*')
	{
		if (legend == 0)
		{
			legend = 1;

			printf("%s\n", "|      Name |    Items | Type |     Best |  Average |     Loops | Samples |     Distribution |");
			printf("%s\n", "| --------- | -------- | ---- | -------- | -------- | --------- | ------- | ---------------- |");
		}
		else
		{
			printf("%s\n", "|           |          |      |          |          |           |         |                  |");
		}
		return;
	}

	best = average = 0;

	for (sam = 0 ; sam < samples ; sam++)
	{
		total = 0;

		start = utime();

		for (rep = 0 ; rep < repetitions ; rep++)
		{
			memcpy(array, unsorted, max * size);

			switch (*name)
			{
				case 'a':
					auxiliary_rotation(array, left, right);
					break;
				case 'b':
					beaker_rotation(array, left, right);
					break;

				case 'c':
					contrev_rotation(array, left, right);
					break;

				case 'g':
					grail_rotation(array, left, right);
					break;

				case 'j':
					juggling_rotation(array, left, right);
					break;

				case 't':
					triprev_rotation(array, left, right);
					break;
			}
		}
		end = utime();

		total = end - start;

		if (!best || total < best)
		{
			best = total;
		}
		average += total;
	}

	average /= samples;

	printf("|%10s | %8d | %4d | %f | %f | %9d | %7d | %16s |\n", name, max, (int) size * 8, best / 1000000.0, average / 1000000.0, repetitions, samples, desc);

	for (cnt = 1 ; cnt < max ; cnt++)
	{
		if (pta[cnt] != ptv[cnt])
		{
			printf("         validate: array[%d] != valid[%d]. (%d vs %d\n", cnt, cnt, pta[cnt], ptv[cnt]);
			break;
		}
	}
}

int main(int argc, char **argv)
{
	int max = 100000;
	int samples = 1000;
	int repetitions = 10;
	int seed = 0;
	int cnt, left, right;
	int *a_array, *r_array, *v_array;
	char dist[40], *sorts[] = { "*", "auxiliary", "beaker", "contrev", "grail", "juggling", "triprev" };

	if (argc >= 1 && argv[1] && *argv[1])
	{
		max = atoi(argv[1]);
	}

	if (argc >= 2 && argv[2] && *argv[2])
	{
		samples = atoi(argv[2]);
	}

	if (argc >= 3 && argv[3] && *argv[3])
	{
		repetitions = atoi(argv[3]);
	}

	if (argc >= 4 && argv[4] && *argv[4])
	{
		seed = atoi(argv[4]);
	}

	printf("Benchmark: array size: %d, samples: %d, repetitions: %d, seed: %d\n\n", max, samples, repetitions, seed);

	// 32 bit

	a_array = (int *) malloc(max * sizeof(int));
	r_array = (int *) malloc(max * sizeof(int));
	v_array = (int *) malloc(max * sizeof(int));

	for (cnt = 0 ; cnt < max ; cnt++)
	{
		r_array[cnt] = cnt;
	}

	for (left = 1 ; left < max / 2 ; left +=  max * 10 / 100 - 1)
	{
		right = max - left;

		memcpy(v_array, r_array, max * sizeof(int));

		auxiliary_rotation(v_array, left, right);

		sprintf(dist, "%d/%d", left, right);

		for (cnt = 0 ; cnt < sizeof(sorts) / sizeof(char *) ; cnt++)
		{
			test_sort(a_array, r_array, v_array, max, samples, repetitions, left, right, sorts[cnt], dist, sizeof(int));
		}
	}

	free(a_array);
	free(r_array);
	free(v_array);

	return 0;
}
