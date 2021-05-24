The most commonly used rotation algorithms (aka block swaps) were documented around 1981 and haven't notably changed since.

Below I'll describe several variants, notably the conjoined triple reversal, followed by a benchmark graph.

Introduction to rotating
------------------------
A rotation is to swap the left side of an array with the right side.
```
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
```
It's a common operation in a variety of sorting algorithms. After the rotation the data is as following.
```
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Auxiliary Rotation
------------------
This is an easy and fast way to rotate, but since it requires auxiliary memory it is of little interest to in-place algorithms.

Typically the smaller half is copied to swap memory, the larger half is moved, and the swap memory is copied back to the main array.

Bentley's Juggling Rotation
---------------------------
Also known as the dolphin algorithm. This is a relatively complex and slow way to rotate in-place. Its first known publication was in 1965.

It finds the greatest common divor and uses a loop to create a chain of consecutive swaps.

Triple Reversal Rotation
------------------------
This is an easy and reliably way to rotate in-place. You reverse the left side, next you reverse the right side, next you reverse the entire array. Upon completion the left and right block will be swapped.

Gries-Mills Rotation
--------------------
Its first known publication was in 1981. You swap the smallest array to its proper location, since it's in its proper location you can forget about it. The larger array is now divided in two parts, which you swap in a similar manner, until the smallest side shrinks to 0 elements.
```
┌───────────────────────────┬──────────────────┐
│ 1  2  3  4  5  6   7  8  9│10 11 12  13 14 15│
└───────────────────────────┴──────────────────┘
                              ↓  ↓  ↓   ↓  ↓  ↓
┌─────────────────┬────────┬───────────────────┐
│10 11 12 13 14 15│ 7  8  9│  1  2  3   4  5  6│
└─────────────────┴────────┴───────────────────┘
                    ↓  ↓  ↓
┌─────────────────┬────────┬─────────┬─────────┐
│10 11 12 13 14 15│ 4  5  6│  1  2  3│  7  8  9│
└─────────────────┴────────┴─────────┴─────────┘
                              ↓  ↓  ↓
┌─────────────────┬────────────────────────────┐
│10 11 12 13 14 15│ 1  2  3   4  5  6   7  8  9│
└─────────────────┴────────────────────────────┘
```

Grail Rotation
--------------
The grail rotation is Gries-Mills derived and tries to improve locality by shifting memory either left or right depending on which side it's swapped from. In addition it performs an auxiliary rotation when the smallest side reaches a size of 1 element.

Beaker Rotation
---------------
The beaker rotation is grail derived and uses a modulo computation to minimize the number of loops. This improves performance when the relative size difference between the two halves is large.

Conjoined Triple Reversal Rotation
----------------------------------
The conjoined triple reversal rotation (aka contrev rotation) is derived from the triple reversal rotation. Rather than three seperate reversals it conjoins the three reversals, improving locality. 

Benchmarks
----------
Since the juggling rotation is rather slow and the grail/beaker rotations are fairly similar I've omitted the juggling and grail rotation from the benchmark graph.

While performance may vary depending on the specific implemention, from worst to best the order is:

* Bentley's Juggling Rotation
* Triple Reversal Rotation (triprev)
* Grail Rotation
* Beaker Rotation (beaker)
* Auxiliary Rotation (auxiliary)
* Conjoined Triple Reversal Rotation (contrev)

It should be noted that the auxiliary Rotation performs better for smaller arrays and when the relative size difference between the two halves is large.

The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 bench.c`. Each test was ran 1,000 times with the time (in seconds) reported of the best and average run.

<details><summary><b>data table</b></summary>

|      Name |    Items | Type |     Best |  Average |     Loops | Samples |     Distribution |
| --------- | -------- | ---- | -------- | -------- | --------- | ------- | ---------------- |
| auxiliary |  1000000 |   32 | 0.000386 | 0.000429 |         1 |    1000 |         1/999999 |
|    beaker |  1000000 |   32 | 0.000381 | 0.000418 |         1 |    1000 |         1/999999 |
|   contrev |  1000000 |   32 | 0.000380 | 0.000408 |         1 |    1000 |         1/999999 |
|     grail |  1000000 |   32 | 0.000376 | 0.000410 |         1 |    1000 |         1/999999 |
|  juggling |  1000000 |   32 | 0.000613 | 0.000642 |         1 |    1000 |         1/999999 |
|   triprev |  1000000 |   32 | 0.000532 | 0.000576 |         1 |    1000 |         1/999999 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000451 | 0.000480 |         1 |    1000 |    100000/900000 |
|    beaker |  1000000 |   32 | 0.000468 | 0.000500 |         1 |    1000 |    100000/900000 |
|   contrev |  1000000 |   32 | 0.000429 | 0.000461 |         1 |    1000 |    100000/900000 |
|     grail |  1000000 |   32 | 0.000468 | 0.000502 |         1 |    1000 |    100000/900000 |
|  juggling |  1000000 |   32 | 0.000648 | 0.000674 |         1 |    1000 |    100000/900000 |
|   triprev |  1000000 |   32 | 0.000526 | 0.000560 |         1 |    1000 |    100000/900000 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000488 | 0.000528 |         1 |    1000 |    199999/800001 |
|    beaker |  1000000 |   32 | 0.000687 | 0.000718 |         1 |    1000 |    199999/800001 |
|   contrev |  1000000 |   32 | 0.000444 | 0.000481 |         1 |    1000 |    199999/800001 |
|     grail |  1000000 |   32 | 0.000646 | 0.000677 |         1 |    1000 |    199999/800001 |
|  juggling |  1000000 |   32 | 0.000806 | 0.000869 |         1 |    1000 |    199999/800001 |
|   triprev |  1000000 |   32 | 0.000532 | 0.000576 |         1 |    1000 |    199999/800001 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000544 | 0.000588 |         1 |    1000 |    299998/700002 |
|    beaker |  1000000 |   32 | 0.000511 | 0.000546 |         1 |    1000 |    299998/700002 |
|   contrev |  1000000 |   32 | 0.000445 | 0.000482 |         1 |    1000 |    299998/700002 |
|     grail |  1000000 |   32 | 0.000533 | 0.000569 |         1 |    1000 |    299998/700002 |
|  juggling |  1000000 |   32 | 0.001966 | 0.002025 |         1 |    1000 |    299998/700002 |
|   triprev |  1000000 |   32 | 0.000533 | 0.000575 |         1 |    1000 |    299998/700002 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000594 | 0.000646 |         1 |    1000 |    399997/600003 |
|    beaker |  1000000 |   32 | 0.000541 | 0.000576 |         1 |    1000 |    399997/600003 |
|   contrev |  1000000 |   32 | 0.000439 | 0.000478 |         1 |    1000 |    399997/600003 |
|     grail |  1000000 |   32 | 0.000563 | 0.000600 |         1 |    1000 |    399997/600003 |
|  juggling |  1000000 |   32 | 0.001757 | 0.001821 |         1 |    1000 |    399997/600003 |
|   triprev |  1000000 |   32 | 0.000526 | 0.000588 |         1 |    1000 |    399997/600003 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000630 | 0.000696 |         1 |    1000 |    499996/500004 |
|    beaker |  1000000 |   32 | 0.000480 | 0.000512 |         1 |    1000 |    499996/500004 |
|   contrev |  1000000 |   32 | 0.000432 | 0.000471 |         1 |    1000 |    499996/500004 |
|     grail |  1000000 |   32 | 0.000806 | 0.000846 |         1 |    1000 |    499996/500004 |
|  juggling |  1000000 |   32 | 0.001112 | 0.001155 |         1 |    1000 |    499996/500004 |
|   triprev |  1000000 |   32 | 0.000523 | 0.000561 |         1 |    1000 |    499996/500004 |

</details>
