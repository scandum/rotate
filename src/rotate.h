/*
	Copyright (C) 2021 Igor van den Hoven ivdhoven@gmail.coms
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
	rotate 1.2
*/

#ifndef ROTATE_H
#define ROTATE_H

void outsidein_reversal(int *array, size_t block_size)
{
	int *pta, *ptb, swap;

	pta = array;
	ptb = array + block_size;

	block_size /= 2;

	while (block_size--)
	{
		swap = *pta; *pta++ = *--ptb; *ptb = swap;
	}
}

void insideout_reversal(int *array, size_t block_size)
{
	int *pta, *ptb, swap;

	ptb = array + block_size;

	block_size /= 2;

	pta = array + block_size;
	ptb -= block_size;

	while (block_size--)
	{
		swap = *--pta; *pta = *ptb; *ptb++ = swap;
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

void stack_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, swap[8];

	pta = array;
	ptb = array + left;
	ptc = array + right;

	if (left < right)
	{
		memcpy(swap, pta, left * sizeof(int));

		memmove(pta, ptb, right * sizeof(int));

		memcpy(ptc, swap, left * sizeof(int));
	}
	else
	{
		memcpy(swap, ptb, right * sizeof(int));

		memmove(ptc, pta, left * sizeof(int));

		memcpy(pta, swap, right * sizeof(int));
	}
}

// 3 reversal - Origin unknown, but prior to 1981

void reversal_rotation(int *array, size_t left, size_t right)
{
	outsidein_reversal(array, left);
	outsidein_reversal(array + left, right);
	outsidein_reversal(array, left + right);
}

// 2021 - Bridge rotation by Igor van den Hoven

void bridge_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, *ptd, *swap;

	pta = array;
	ptb = pta + left;
	ptc = pta + right;
	ptd = ptc + left;

	if (left < right)
	{
		size_t bridge = right - left;

		if (bridge < left)
		{
			swap = malloc(bridge * sizeof(int));

			memcpy(swap, ptb, bridge * sizeof(int));

			while (left--)
			{
				*--ptc = *--ptd; *ptd = *--ptb;
			}
			memcpy(pta, swap, bridge * sizeof(int));
		}
		else
		{
			swap = malloc(left * sizeof(int));
			memcpy(swap, pta, left * sizeof(int));
			memmove(pta, ptb, right * sizeof(int));
			memcpy(ptc, swap, left * sizeof(int));
		}
	}
	else if (right < left)
	{
		size_t bridge = left - right;

		if (bridge < right)
		{
			swap = malloc(bridge * sizeof(int));
			
			memcpy(swap, ptc, bridge * sizeof(int));
			
			while (right--)
			{
				*ptc++ = *pta; *pta++ = *ptb++;
			}
			memcpy(ptd - bridge, swap, bridge * sizeof(int));
		}
		else
		{
			swap = malloc(right * sizeof(int));
			memcpy(swap, ptb, right * sizeof(int));
			memmove(ptc, pta, left * sizeof(int));
			memcpy(pta, swap, right * sizeof(int));
		}
	}
	else
	{
		swap = malloc(1 * sizeof(int));

		while (left--)
		{
			*swap = *pta; *pta++ = *ptb; *ptb++ = *swap;
		}
	}
	free(swap);
}

// 2021 - Conjoined Triple Reversal rotation by Igor van den Hoven

void contrev_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, *ptd, swap;
	size_t loop;

	pta = array;
	ptb = array + left;
	ptc = array + left;
	ptd = array + left + right;

	if (left > right)
	{
		loop = right / 2;

		while (loop--)
		{
			swap = *--ptb; *ptb = *pta; *pta++ = *ptc; *ptc++ = *--ptd; *ptd = swap;
		}

		loop = (ptb - pta) / 2;

		while (loop--)
		{
			swap = *--ptb; *ptb = *pta; *pta++ = *--ptd; *ptd = swap;
		}
		loop = (ptd - pta) / 2;

		while (loop--)
		{
			swap = *pta; *pta++ = *--ptd; *ptd = swap;
		}
	}
	else if (left < right)
	{
		loop = left / 2;

		while (loop--)
		{
			swap = *--ptb; *ptb = *pta; *pta++ = *ptc; *ptc++ = *--ptd; *ptd = swap;
		}

		loop = (ptd - ptc) / 2;

		while (loop--)
		{
			swap = *ptc; *ptc++ = *--ptd; *ptd = *pta; *pta++ = swap;
		}
		loop = (ptd - pta) / 2;

		while (loop--)
		{
			swap = *pta; *pta++ = *--ptd; *ptd = swap;
		}
	}
	else
	{
		loop = left;

		while (loop--)
		{
			swap = *pta; *pta++ = *ptb; *ptb++ = swap;
		}
	}
}

// 2021 - Trinity rotation by Igor van den Hoven (Conjoined Triple Reversal + Bridge rotation)

#define MAX_AUX 8

void trinity_rotation(int *array, size_t left, size_t right)
{
	int *pta, *ptb, *ptc, *ptd, swap[MAX_AUX];
	size_t loop;

	if (left < right)
	{
		if (left <= MAX_AUX)
		{
			memcpy(swap, array, left * sizeof(int));
			memmove(array, array + left, right * sizeof(int));
			memcpy(array + right, swap, left * sizeof(int));
		}
		else
		{
			pta = array;
			ptb = pta + left;

			loop = right - left;

			if (loop <= MAX_AUX && loop > 3)
			{
				ptc = pta + right;
				ptd = ptc + left;

				memcpy(swap, ptb, loop * sizeof(int));

				while (left--)
				{
					*--ptc = *--ptd; *ptd = *--ptb;
				}
				memcpy(pta, swap, loop * sizeof(int));
			}
			else
			{
				ptc = ptb;
				ptd = ptc + right;

				loop = left / 2;

				while (loop--)
				{
					*swap = *--ptb; *ptb = *pta; *pta++ = *ptc; *ptc++ = *--ptd; *ptd = *swap;
				}

				loop = (ptd - ptc) / 2;

				while (loop--)
				{
					*swap = *ptc; *ptc++ = *--ptd; *ptd = *pta; *pta++ = *swap;
				}

				loop = (ptd - pta) / 2;

				while (loop--)
				{
					*swap = *pta; *pta++ = *--ptd; *ptd = *swap;
				}
			}
		}
	}
	else if (right < left)
	{
		if (right <= MAX_AUX)
		{
			memcpy(swap, array + left, right * sizeof(int));
			memmove(array + right, array, left * sizeof(int));
			memcpy(array, swap, right * sizeof(int));
		}
		else
		{
			pta = array;
			ptb = pta + left;

			loop = left - right;

			if (loop <= MAX_AUX && loop > 3)
			{
				ptc = pta + right;
				ptd = ptc + left;

				memcpy(swap, ptc, loop * sizeof(int));
			
				while (right--)
				{
					*ptc++ = *pta; *pta++ = *ptb++;
				}
				memcpy(ptd - loop, swap, loop * sizeof(int));
			}
			else
			{
				ptc = ptb;
				ptd = ptc + right;

				loop = right / 2;

				while (loop--)
				{
					*swap = *--ptb; *ptb = *pta; *pta++ = *ptc; *ptc++ = *--ptd; *ptd = *swap;
				}

				loop = (ptb - pta) / 2;

				while (loop--)
				{
					*swap = *--ptb; *ptb = *pta; *pta++ = *--ptd; *ptd = *swap;
				}

				loop = (ptd - pta) / 2;

				while (loop--)
				{
					*swap = *pta; *pta++ = *--ptd; *ptd = *swap;
				}
			}
		}
	}
	else
	{
		pta = array;
		ptb = pta + left;

		while (left--)
		{
			*swap = *pta; *pta++ = *ptb; *ptb++ = *swap;
		}
	}
}

#undef MAX_AUX

// 1981 - Gries-Mills rotation by David Gries and Harlan Mills

void griesmills_rotation(int *array, size_t left, size_t right)
{
	size_t start = 0;

	while (left && right)
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
		}
		else
		{
			do
			{
				forward_block_swap(array, start + left - right, start + left, right);

				left -= right;
			}
			while (right <= left);
		}
	}
}

// 2020 - Grail rotation by the Holy Grail Sort project (Gries-Mills derived)

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
		stack_rotation(array + start, left, right);
	}
}

// 2021 - Piston rotation by Igor van den Hoven. Based on the successive swap described by Gries and Mills in 1981.

void piston_rotation(int *array, size_t left, size_t right)
{
	size_t start = 0;

	while (left > 0)
	{
		while (left <= right)
		{
			forward_block_swap(array, start, start + right, left);
			right -= left;
		}
		if (right <= 0)
		{
			break;
		}
		do
		{
			forward_block_swap(array, start, start + left, right);
			left -= right;
			start += right;
		}
		while (right <= left);
	}

/*	if (left && right)
	{
		stack_rotation(array + start, left, right);
	}*/
}

// 2021 - Helix rotation by Control (grail derived)

void helix_rotation(int *array, size_t left, size_t right)
{
	int swap;
	size_t start = 0;
	size_t end = left + right;
	size_t mid = left;

	while (1)
	{
		if (left > right)
		{
			if (right <= 1)
			{
				break;
			}

			while (mid > start)
			{
				swap = array[--mid]; array[mid] = array[--end]; array[end] = swap;
			}
			mid += (left %= right);
			right = end - mid;
		}
		else
		{
			if (left <= 1)
			{	
				break;
			}

			while (mid < end)
			{
				swap = array[mid]; array[mid++] = array[start]; array[start++] = swap;
			}
			mid -= (right %= left);
			left = mid - start;
		}
	}

	if (left && right)
	{
		stack_rotation(array + start, left, right);
	}
}

// 2021 - Drill rotation by Igor van den Hoven (grail derived with piston and helix loops)

void drill_rotation(int *array, size_t left, size_t right)
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
			loop = end - mid - (right %= left);

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

		loop = mid - start - (left %= right);

		do
		{
			swap = array[--mid]; array[mid] = array[--end]; array[end] = swap;
		}
		while (--loop);
	}

	if (left && right)
	{
		stack_rotation(array + start, left, right);
	}
}

// 1965 - Juggling aka Dolphin rotation

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

	if (left == 0)
	{
		return;
	}

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

#endif
