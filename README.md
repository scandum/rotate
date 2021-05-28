The most commonly used rotation algorithms (aka block swaps) were documented around 1981 and haven't notably changed since.

Below I'll describe several variants, notably the conjoined triple reversal, followed by a benchmark graph.

Introduction to rotating
------------------------
A rotation is to swap the left side of an array with the right side.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
```
It's a common operation in a variety of sorting algorithms. After the rotation the data is as following.
```c
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Auxiliary Rotation
------------------
This is an easy and fast way to rotate, but since it requires auxiliary memory it is of little interest to in-place algorithms.

Typically the smaller half is copied to swap memory, the larger half is moved, and the swap memory is copied back to the main array.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
                             ↑  ↑  ↑  ↑  ↑  ↑    ↓  ↓  ↓  ↓  ↓  ↓
┌──────────────────────────┬─────────────────┐ ┌─────────────────┐
│ 1  2  3  4  5  6  7  8  9│                 │ │10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘ └─────────────────┘
  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑
                    ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
┌─────────────────┬──────────────────────────┐ ┌─────────────────┐
│                 │ 1  2  3  4  5  6  7  8  9│ │10 11 12 13 14 15│
└─────────────────┴──────────────────────────┘ └─────────────────┘
  ↓  ↓  ↓  ↓  ↓  ↓                               ↑  ↑  ↑  ↑  ↑  ↑
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Bridge Rotation
---------------
This is a slightly more complex auxiliary rotation that reduces the maximum auxiliary memory requirement from 50% to 33%.

If the overlap between the two halves is smaller than the halves themselves it copies the overlap to swap memory instead.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
                    ↑  ↑  ↑                      ↓  ↓  ↓
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│ 1  2  3  4  5  6│        │10 11 12 13 14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
  ↑  ↑  ↑  ↑  ↑  ↑           ↑  ↑  ↑  ↑  ↑  ↑
  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
┌─────────────────┬──────────────────────────┐ ┌────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6         │ │ 7  8  9│
└─────────────────┴──────────────────────────┘ └────────┘
                                      ↓  ↓  ↓    ↑  ↑  ↑
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Bentley's Juggling Rotation
---------------------------
Also known as the dolphin algorithm. This is a relatively complex and cache inefficient way to rotate in-place, though it does so in the minimal amount of moves. Its first known publication was in 1965.

It computes the greatest common divisor and uses a loop to create a chain of consecutive swaps.

```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
  ↓        ↓        ↓        ↓        ↓
┌──────────────────────────┬─────────────────┐
│10  2  3 13  5  6  1  8  9│ 4 11 12  7 14 15│
└──────────────────────────┴─────────────────┘
     ↓        ↓        ↓        ↓        ↓
┌──────────────────────────┬─────────────────┐
│10 11  3 13 14  6  1  2  9│ 4  5 12  7  8 15│
└──────────────────────────┴─────────────────┘
        ↓        ↓        ↓        ↓        ↓
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Triple Reversal Rotation
------------------------
This is an easy and reliable way to rotate in-place. You reverse the left side, next you reverse the right side, next you reverse the entire array. Upon completion the left and right block will be swapped.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
┌──────────────────────────┬─────────────────┐
│ 9  8  7  6  5  4  3  2  1│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
                             ↓  ↓  ↓  ↓  ↓  ↓
┌──────────────────────────┬─────────────────┐
│ 9  8  7  6  5  4  3  2  1│15 14 13 12 11 10│
└──────────────────────────┴─────────────────┘
  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Gries-Mills Rotation
--------------------
Its first known publication was in 1981. You swap the smallest array to its proper location, since it's in its proper location you can forget about it. The larger array is now divided in two parts, which you swap in a similar manner, until the smallest side shrinks to 0 elements.
```c
┌──────────────────────────┬──────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12  13 14 15│
└──────────────────────────┴──────────────────┘
  ↓  ↓  ↓   ↓  ↓  ↓          ↑  ↑  ↑   ↑  ↑  ↑
┌─────────────────┬────────┬──────────────────┐
│10 11 12 13 14 15│ 7  8  9│ 1  2  3   4  5  6│
└─────────────────┴────────┴──────────────────┘
                    ↑  ↑  ↑            ↓  ↓  ↓
┌─────────────────┬────────┬────────┬─────────┐
│10 11 12 13 14 15│ 4  5  6│ 1  2  3│  7  8  9│
└─────────────────┴────────┴────────┴─────────┘
                    ↓  ↓  ↓  ↑  ↑  ↑
┌─────────────────┬───────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6   7  8  9│
└─────────────────┴───────────────────────────┘
```

Grail Rotation
--------------
The grail rotation from the Holy Grail Sort Project is Gries-Mills derived and tries to improve locality by shifting memory either left or right depending on which side it's swapped from. In addition it performs an auxiliary rotation on stack memory when the smallest side reaches a size of 1 element.

Beaker Rotation
---------------
The beaker rotation is grail derived and uses a modulo computation to minimize the number of loops. This improves performance when the relative size difference between the two halves is large.

Conjoined Triple Reversal Rotation
----------------------------------
The conjoined triple reversal rotation (aka trinity rotation) is derived from the triple reversal rotation. Rather than three seperate reversals it conjoins the three reversals, improving locality and reducing the number of moves. Optionally, if the smallest side is smaller than 8 elements it skips the trinity rotation and performs an auxiliary rotation on stack memory.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
  ↓                       ↓  ↓              ↓
┌──────────────────────────┬─────────────────┐
│10  2  3  4  5  6  7  8  1│15 11 12 13 14  9│
└──────────────────────────┴─────────────────┘
     ↓                 ↓        ↓        ↓
┌──────────────────────────┬─────────────────┐
│10 11  3  4  5  6  7  2  1│15 14 12 13  8  9│
└──────────────────────────┴─────────────────┘
        ↓           ↓              ↓  ↓
┌──────────────────────────┬─────────────────┐
│10 11 12  4  5  6  3  2  1│15 14 13  7  8  9│
└──────────────────────────┴─────────────────┘
           ↓     ↓                 ↓
┌──────────────────────────┬─────────────────┐
│10 11 12 13  5  4  3  2  1│15 14  6  7  8  9│
└──────────────────────────┴─────────────────┘
              ↓                 ↓
┌──────────────────────────┬─────────────────┐
│10 11 12 13 14  4  3  2  1│15  5  6  7  8  9│
└──────────────────────────┴─────────────────┘
                 ↓           ↓
┌──────────────────────────┬─────────────────┐
│10 11 12 13 14 15  3  2  1│ 4  5  6  7  8  9│
└──────────────────────────┴─────────────────┘
                    ↓     ↓
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```
Benchmarks
----------
Since the juggling rotation is rather slow and the auxiliary/bridge and grail/beaker rotations are fairly similar I've omitted the auxiliary, juggling and grail rotations from the benchmark graph.

While performance may vary depending on the specific implemention, from worst to best the order is:

* Bentley's Juggling Rotation
* Triple Reversal Rotation (reversal)
* Grail Rotation
* Beaker Rotation (beaker)
* Auxiliary Rotation
* Bridge Rotation (bridge)
* Conjoined Triple Reversal Rotation (trinity)

It should be noted that the auxiliary Rotation performs better for smaller arrays and when the relative size difference between the two halves is large.

The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 bench.c`. Each test was ran 1,000 times with the time (in seconds) reported of the best and average run.

![rotation graph](/graph1.png)

<details><summary><b>data table</b></summary>

|      Name |    Items | Type |     Best |  Average |     Loops | Samples |     Distribution |
| --------- | -------- | ---- | -------- | -------- | --------- | ------- | ---------------- |
| auxiliary |  1000000 |   32 | 0.000364 | 0.000394 |         1 |    1000 |         1/999999 |
|    beaker |  1000000 |   32 | 0.000364 | 0.000392 |         1 |    1000 |         1/999999 |
|    bridge |  1000000 |   32 | 0.000365 | 0.000386 |         1 |    1000 |         1/999999 |
|     grail |  1000000 |   32 | 0.000363 | 0.000386 |         1 |    1000 |         1/999999 |
|  juggling |  1000000 |   32 | 0.000603 | 0.000630 |         1 |    1000 |         1/999999 |
|   trinity |  1000000 |   32 | 0.000362 | 0.000384 |         1 |    1000 |         1/999999 |
|  reversal |  1000000 |   32 | 0.000520 | 0.000553 |         1 |    1000 |         1/999999 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000434 | 0.000460 |         1 |    1000 |    100000/900000 |
|    beaker |  1000000 |   32 | 0.000455 | 0.000479 |         1 |    1000 |    100000/900000 |
|    bridge |  1000000 |   32 | 0.000436 | 0.000460 |         1 |    1000 |    100000/900000 |
|     grail |  1000000 |   32 | 0.000457 | 0.000481 |         1 |    1000 |    100000/900000 |
|  juggling |  1000000 |   32 | 0.000632 | 0.000657 |         1 |    1000 |    100000/900000 |
|   trinity |  1000000 |   32 | 0.000420 | 0.000445 |         1 |    1000 |    100000/900000 |
|  reversal |  1000000 |   32 | 0.000513 | 0.000543 |         1 |    1000 |    100000/900000 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000471 | 0.000501 |         1 |    1000 |    199999/800001 |
|    beaker |  1000000 |   32 | 0.000676 | 0.000702 |         1 |    1000 |    199999/800001 |
|    bridge |  1000000 |   32 | 0.000471 | 0.000492 |         1 |    1000 |    199999/800001 |
|     grail |  1000000 |   32 | 0.000635 | 0.000668 |         1 |    1000 |    199999/800001 |
|  juggling |  1000000 |   32 | 0.000798 | 0.000831 |         1 |    1000 |    199999/800001 |
|   trinity |  1000000 |   32 | 0.000435 | 0.000460 |         1 |    1000 |    199999/800001 |
|  reversal |  1000000 |   32 | 0.000522 | 0.000557 |         1 |    1000 |    199999/800001 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000525 | 0.000552 |         1 |    1000 |    299998/700002 |
|    beaker |  1000000 |   32 | 0.000498 | 0.000524 |         1 |    1000 |    299998/700002 |
|    bridge |  1000000 |   32 | 0.000521 | 0.000545 |         1 |    1000 |    299998/700002 |
|     grail |  1000000 |   32 | 0.000521 | 0.000548 |         1 |    1000 |    299998/700002 |
|  juggling |  1000000 |   32 | 0.001943 | 0.001991 |         1 |    1000 |    299998/700002 |
|   trinity |  1000000 |   32 | 0.000437 | 0.000474 |         1 |    1000 |    299998/700002 |
|  reversal |  1000000 |   32 | 0.000520 | 0.000575 |         1 |    1000 |    299998/700002 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000567 | 0.000606 |         1 |    1000 |    399997/600003 |
|    beaker |  1000000 |   32 | 0.000522 | 0.000549 |         1 |    1000 |    399997/600003 |
|    bridge |  1000000 |   32 | 0.000512 | 0.000532 |         1 |    1000 |    399997/600003 |
|     grail |  1000000 |   32 | 0.000550 | 0.000578 |         1 |    1000 |    399997/600003 |
|  juggling |  1000000 |   32 | 0.001744 | 0.001788 |         1 |    1000 |    399997/600003 |
|   trinity |  1000000 |   32 | 0.000435 | 0.000465 |         1 |    1000 |    399997/600003 |
|  reversal |  1000000 |   32 | 0.000518 | 0.000554 |         1 |    1000 |    399997/600003 |
|           |          |      |          |          |           |         |                  |
| auxiliary |  1000000 |   32 | 0.000609 | 0.000651 |         1 |    1000 |    499996/500004 |
|    beaker |  1000000 |   32 | 0.000468 | 0.000498 |         1 |    1000 |    499996/500004 |
|    bridge |  1000000 |   32 | 0.000377 | 0.000397 |         1 |    1000 |    499996/500004 |
|     grail |  1000000 |   32 | 0.000796 | 0.000828 |         1 |    1000 |    499996/500004 |
|  juggling |  1000000 |   32 | 0.001084 | 0.001124 |         1 |    1000 |    499996/500004 |
|   trinity |  1000000 |   32 | 0.000376 | 0.000401 |         1 |    1000 |    499996/500004 |
|  reversal |  1000000 |   32 | 0.000513 | 0.000545 |         1 |    1000 |    499996/500004 |

</details>
