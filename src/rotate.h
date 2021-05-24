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
	rotate 1.0
*/

#ifndef ROTATE_H
#define ROTATE_H

void block_reversal(int *array, size_t block_size)
{
	int *pta, *ptb, swap;

	pta = array;
	ptb = array + block_size - 1;

	block_size /= 2;

	while (block_size--)
	{
		swap = *pta; *pta++ = *ptb; *ptb-- = swap;
	}
}

void forward_block_swap(int *array, const size_t start1, const size_t start2, size_t block_size)
{
	int *pta, *ptb, swap;

	pta = array + start1;
	ptb = array + start2;

	while (block_size--)
	{
		swap = *pta; *pta++ = *ptb; *ptb++ = swap;
	}
}

void backward_block_swap(int *array, const size_t start1, const size_t start2, size_t block_size)
{
	int *pta, *ptb, swap;

	pta = array + start1 + block_size;
	ptb = array + start2 + block_size;

	while (block_size--)
	{
		swap = *--pta; *pta = *--ptb; *ptb = swap;
	}
}

void auxiliary_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, *swap;

	pta = array;
	ptb = array + left;
	ptc = array + right;

	if (left < right)
	{
		swap = malloc(left * sizeof(int));

		memcpy(swap, pta, left * sizeof(int));

		memmove(pta, ptb, right * sizeof(int));

		memcpy(ptc, swap, left * sizeof(int));
	}
	else
	{
		swap = malloc(right * sizeof(int));

		memcpy(swap, ptb, right * sizeof(int));

		memmove(ptc, pta, left * sizeof(int));

		memcpy(pta, swap, right * sizeof(int));
	}
	free(swap);
}

// Gries-Mills / Holy Grail derived, based on the java implementation by Control

void beaker_rotation(int *array, size_t left, size_t right)
{
	int swap;
	size_t start = 0;
	size_t end = left + right;
	size_t mid = left;
	size_t loop;

	while (left > 1)
	{
		if (left <= right)
		{
			right %= left;
			loop = end - mid - right;

			do
			{
				swap = array[mid]; array[mid++] = array[start]; array[start++] = swap;
			}
			while (--loop);
		}

		if (right <= 1)
		{
			break;
		}

		left %= right;
		loop = mid - start - left;

		do
		{
			swap = array[--mid]; array[mid] = array[--end]; array[end] = swap;
		}
		while (--loop);
	}

	if (left && right)
	{
		if (left <= right)
		{
			int swap = array[start];
			memmove(&array[start], &array[start + 1], (right) * sizeof(int));
			array[start + right] = swap;
		}
		else
		{
			int swap = array[start + left];
			memmove(&array[start + 1], &array[start], (left) * sizeof(int));
			array[start] = swap;
		}
	}
}

// conjoined triple reversal rotation

void contrev_rotation(int *array, size_t left, size_t right)
{
	if (left < 8 || right < 8)
	{
		int swap[7];

		if (left < 8)
		{
			memcpy(swap, array, left * sizeof(int));
			memmove(array, array + left, right * sizeof(int));
			memcpy(array + right, swap, left * sizeof(int));
		}
		else
		{
			memcpy(swap, array + left, right * sizeof(int));
			memmove(array + right, array, left * sizeof(int));
			memcpy(array, swap, right * sizeof(int));
		}
	}
	else
	{
		int *pta, *ptb, *ptc, *ptd, swap;
		size_t loop;

		pta = array;
		ptb = pta + left;

		ptc = ptb;
		ptd = ptc + right;

		loop = left < right ? left / 2 : right / 2;

		while (loop--)
		{
			swap = *--ptb; *ptb = *pta; *pta++ = *ptc; *ptc++ = *--ptd; *ptd = swap;
		}

		if (left >= right)
		{
			loop = (ptb - pta) / 2;

			while (loop--)
			{
				swap = *--ptb; *ptb = *pta; *pta++ = *--ptd; *ptd = swap;
			}
		}
		else
		{
			loop = (ptd - ptc) / 2;

			while (loop--)
			{
				swap = *ptc; *ptc++ = *--ptd; *ptd = *pta; *pta++ = swap;
			}
		}

		loop = (ptd - pta) / 2;

		while (loop--)
		{
			swap = *pta; *pta++ = *--ptd; *ptd = swap;
		}
	}
}

// Gries-Mills derived, based on the java implementation from the Holy Grail Sort project

void grail_rotation(int *array, size_t left, size_t right)
{
	size_t min   = left <= right ? left : right;
	size_t start = 0;

	while (min > 1)
	{
		if (left <= right)
		{
			do
			{
				forward_block_swap(array, start, start + left, left);

				start += left;
				right -= left;
			}
			while (left <= right);

			min = right;
		}
		else
		{
			do
			{
				backward_block_swap(array, start + left - right, start + left, right);

				left -= right;
			}
			while (right <= left);

			min = left;
		}
	}

	if (min)
	{
		if (left <= right)
		{
			int swap = array[start];
			memmove(&array[start], &array[start + 1], (right) * sizeof(int));
			array[start + right] = swap;
		}
		else
		{
			int swap = array[start + left];
			memmove(&array[start + 1], &array[start], (left) * sizeof(int));
			array[start] = swap;
		}
	}
}

// Juggling / Dolphin rotation

int gcd(int a, int b)
{
	int r;

	while (b)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

void juggling_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, *ptd, swap;
	const size_t nmemb = left + right;

	ptd = array + gcd(left, nmemb);

	for (ptc = array ; ptc < ptd ; ptc++)
	{
		swap = *ptc;
		pta = ptc;

		while (1)
		{
			ptb = pta + left;

			if (ptb >= array + nmemb)
			{
				ptb -= nmemb;

				if (ptb == ptc)
				{
					break;
				}
			}
			*pta = *ptb;
			pta = ptb;
		}
		*pta = swap;
	}
}

// Classic 3 reversal

void triprev_rotation(int *array, size_t left, size_t right)
{
	block_reversal(array, left);
	block_reversal(array + left, right);
	block_reversal(array, left + right);
}

#endif
