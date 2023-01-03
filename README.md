The most commonly used rotation algorithms (aka block swaps) were documented around 1981 and haven't notably changed since.

Below I'll describe the known variants as well as three novel rotation algorithms introduced in 2021, notably the [trinity rotation](https://github.com/scandum/rotate#Trinity-Rotation), followed by some [benchmarks](https://github.com/scandum/rotate#Benchmarks).

Introduction to rotating
------------------------
A rotation is to swap the left side of an array with the right side.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
```
After the rotation the data is as following.
```c
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

Utility of rotating
-------------------
According to Sean Parent rotations are a very common operation: https://www.youtube.com/watch?v=UZmeDQL4LaE

Auxiliary Rotation
------------------
This is an easy and fast way to rotate, but since it requires auxiliary memory it is of little interest to in-place algorithms.

Typically the smaller half is copied to swap memory, the larger half is moved, and the swap memory is copied back to the main array.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
                             └──┴──┴──┴──┴──┴────┬──┬──┬──┬──┬──┐
┌──────────────────────────┬─────────────────┐ ┌─────────────────┐
│ 1  2  3  4  5  6  7  8  9│                 │ │10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘ └─────────────────┘
  └──┴──┴──┴──┴──┴──┼──┼──┼──┬──┬──┬──┬──┬──┐
┌─────────────────┬──────────────────────────┐ ┌─────────────────┐
│                 │ 1  2  3  4  5  6  7  8  9│ │10 11 12 13 14 15│
└─────────────────┴──────────────────────────┘ └─────────────────┘
  ┌──┬──┬──┬──┬──┬───────────────────────────────┴──┴──┴──┴──┴──┘
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

[![auxiliary rotation](/images/auxiliary.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=0s)

Bridge Rotation
---------------
This is a slightly more complex auxiliary rotation that reduces the maximum auxiliary memory requirement from 50% to 33%. If the overlap between the two halves is smaller than the halves themselves it copies the overlap to swap memory instead. Its first known publication was in 2021 by Igor van den Hoven.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
                    └──┴──┴──────────────────────┬──┬──┐
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│ 1  2  3  4  5  6│        │10 11 12 13 14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
  ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10  2  3  4  5  6│ 1      │   11 12 13 14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
     ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10 11  3  4  5  6│ 1  2   │      12 13 14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
        ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10 11 12  4  5  6│ 1  2  3│         13 14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
           ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10 11 12 13  5  6│ 1  2  3│ 4          14 15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
              ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10 11 12 13 14  6│ 1  2  3│ 4  5          15│ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
                 ├─────────────────┬────────┘
┌─────────────────┬────────┬─────────────────┐ ┌────────┐
│10 11 12 13 14 15│ 1  2  3│ 4  5  6         │ │ 7  8  9│
└─────────────────┴────────┴─────────────────┘ └────────┘
                                      ┌──┬──┬────┴──┴──┘
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

[![bridge rotation](/images/bridge.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=15s)

Juggling Rotation
-----------------
Also known as the dolphin algorithm. This is a relatively complex and inefficient way to rotate in-place, though it does so in the minimal number of moves. Its first known publication was in 1965.

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
[![juggling rotation](/images/juggling.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=119s)

Triple Reversal Rotation
------------------------
This is an easy and reliable way to rotate in-place. You reverse the left side, next you reverse the right side, next you reverse the entire array. Upon completion the left and right block will be swapped. There's no known first publication, but it was prior to 1981.
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

[![reversal rotation](/images/reversal.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=23s)

Gries-Mills Rotation
--------------------
In some cases this rotation outperforms the classic triple reversal rotation while making fewer moves. You swap the smallest array linearly towards its proper location, since the blocks behind it are in the proper location you can forget about them. What remains of the larger array is now divided in two parts, which you swap in a similar manner, until the smallest side shrinks to 0 elements. Its first known publication was in 1981 by David Gries and Harlan Mills. 
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
           ┌──┬──┬──┬──┬──┬──┴──┴──┴──┴──┴──┘
┌────────┬─────────────────┬─────────────────┐
│ 1  2  3│10 11 12 13 14 15│ 4  5  6  7  8  9│
└────────┴─────────────────┴─────────────────┘
  └──┴──┴──┬──┬──┐
┌────────┬────────┬──────────────────────────┐
│10 11 12│ 1  2  3│13 14 15  4  5  6  7  8  9│
└────────┴────────┴──────────────────────────┘
           └──┴──┴──┬──┬──┐
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

[![griesmills rotation](/images/griesmills.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=47s)

Successive Rotation
-------------------
First described by Gries and Mills in 1981, this rotation is very similar to the Gries-Mills rotation but performs non-linear swaps. It is implemented as the Piston Rotation in the benchmark, named after a loop optimization that removes up to `log n` branch mispredictions by block swapping in pairs of two.

```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
  ┌──┬──┬──┬──┬──┬───────────┴──┴──┴──┴──┴──┘
┌─────────────────┬────────┬─────────────────┐
│10 11 12 13 14 15│ 7  8  9│ 1  2  3  4  5  6│
└─────────────────┴────────┴─────────────────┘
                    └──┴──┴───────────┬──┬──┐
┌─────────────────┬────────┬────────┬────────┐
│10 11 12 13 14 15│ 4  5  6│ 1  2  3│ 7  8  9│
└─────────────────┴────────┴────────┴────────┘
                    ┌──┬──┬──┴──┴──┘
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

[![piston rotation](/images/piston.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=87s)

Grail Rotation
--------------
The grail rotation from the Holy Grail Sort Project is Gries-Mills derived and tries to improve locality by shifting memory either left or right depending on which side it's swapped from. In addition it performs an auxiliary rotation on stack memory when the smallest side reaches a size of 1 element, which is the worst case for the Gries-Mills rotation. The flow diagram is identical to that of Gries-Mills, but due to memory being shifted from the right the visualization differs.

[![grail rotation](/images/grail.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=67s)

Helix Rotation
---------------
The helix rotation has similarities with the Gries-Mills rotation but has a distinct sequential movement pattern. It is an improvement upon the Grail rotation by merging the two inner loops into a single loop, significantly improving performance when the relative size difference between the two halves is large. In addition it doesn't stop when the smallest block no longer fits, but continues and recalculates the left or right side. The utilization of the merged loops is counter-intuitive and is likely novel. Its first known publication was in 2021 by Control from the Holy Grail Sort Project.
```c
┌──────────────────────────┬─────────────────┐
│ 1  2  3  4  5  6  7  8  9│10 11 12 13 14 15│
└──────────────────────────┴─────────────────┘
           ┌──┬──┬──┬──┬──┬──┴──┴──┴──┴──┴──┘
┌────────┬─────────────────┬─────────────────┐
│ 1  2  3│10 11 12 13 14 15│ 4  5  6  7  8  9│
└────────┴─────────────────┴─────────────────┘
  ┌──┬──┬───────────┴──┴──┘
┌─────────────────┬──────────────────────────┐
│13 14 15 10 11 12│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
  └──┴──┴──┬──┬──┐
┌─────────────────┬──────────────────────────┐
│10 11 12 13 14 15│ 1  2  3  4  5  6  7  8  9│
└─────────────────┴──────────────────────────┘
```

[![helix rotation](/images/helix.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=108s)

Trinity Rotation
----------------
The trinity rotation (aka conjoined triple reversal) is derived from the triple reversal rotation. Rather than three separate reversals it conjoins the three reversals, improving locality and reducing the number of moves. Optionally, if the overlap is smaller than 8 elements, it skips the trinity rotation and performs an auxiliary or bridge rotation on stack memory. Its first known publication was in 2021 by Igor van den Hoven.
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

[![trinity rotation](/images/trinity.gif)](https://www.youtube.com/watch?v=rHubUT40FDc&t=35s)

Benchmarks
----------
Since the auxiliary/bridge rotations are fairly similar I've omitted the auxiliary rotation from the benchmark graph. Similarly the grail rotation has been omitted since it's fundamentally slower than the helix rotation. The contrev rotation is the trinity rotation without auxiliary memory.

While performance may vary depending on the specific implemention and array size, from worst to best the order is:

* Juggling Rotation (juggling)
* Auxiliary Rotation
* Gries-Mills Rotation (griesmills)
* Piston Rotation (piston)
* Grail Rotation
* Bridge Rotation (bridge)
* Triple Reversal Rotation (reversal)
* Helix Rotation (helix)
* Trinity Rotation (trinity)

It should be noted that the auxiliary Rotation performs better for smaller arrays and when the relative size difference between the two halves is large.

The following benchmark was on WSL 2 gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04). The source code was compiled using `gcc -O3 bench.c`. Each test was ran 1,000 times with the time (in seconds) reported of the best and average run.

![rotation graph](/images/bridge_helix_trinity_reversal.png)

<details><summary><b>data table</b></summary>

|      Name |    Items | Type |     Best |  Average |     Loops | Samples |     Distribution |
| --------- | -------- | ---- | -------- | -------- | --------- | ------- | ---------------- |
|    bridge |  1000000 |   32 | 0.000389 | 0.000510 |         1 |     200 |         1/999999 |
|     helix |  1000000 |   32 | 0.000383 | 0.000457 |         1 |     200 |         1/999999 |
|   trinity |  1000000 |   32 | 0.000395 | 0.000482 |         1 |     200 |         1/999999 |
|  reversal |  1000000 |   32 | 0.000517 | 0.000617 |         1 |     200 |         1/999999 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000385 | 0.000445 |         1 |     200 |      1000/999000 |
|     helix |  1000000 |   32 | 0.000487 | 0.000559 |         1 |     200 |      1000/999000 |
|   trinity |  1000000 |   32 | 0.000412 | 0.000469 |         1 |     200 |      1000/999000 |
|  reversal |  1000000 |   32 | 0.000507 | 0.000581 |         1 |     200 |      1000/999000 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000456 | 0.000616 |         1 |     200 |     99999/900001 |
|     helix |  1000000 |   32 | 0.000543 | 0.000619 |         1 |     200 |     99999/900001 |
|   trinity |  1000000 |   32 | 0.000446 | 0.000551 |         1 |     200 |     99999/900001 |
|  reversal |  1000000 |   32 | 0.000519 | 0.000598 |         1 |     200 |     99999/900001 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000495 | 0.000595 |         1 |     200 |    199998/800002 |
|     helix |  1000000 |   32 | 0.000572 | 0.000656 |         1 |     200 |    199998/800002 |
|   trinity |  1000000 |   32 | 0.000447 | 0.000513 |         1 |     200 |    199998/800002 |
|  reversal |  1000000 |   32 | 0.000519 | 0.000636 |         1 |     200 |    199998/800002 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000557 | 0.000659 |         1 |     200 |    299997/700003 |
|     helix |  1000000 |   32 | 0.000519 | 0.000694 |         1 |     200 |    299997/700003 |
|   trinity |  1000000 |   32 | 0.000441 | 0.000544 |         1 |     200 |    299997/700003 |
|  reversal |  1000000 |   32 | 0.000515 | 0.000701 |         1 |     200 |    299997/700003 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000527 | 0.000612 |         1 |     200 |    399996/600004 |
|     helix |  1000000 |   32 | 0.000481 | 0.000553 |         1 |     200 |    399996/600004 |
|   trinity |  1000000 |   32 | 0.000437 | 0.000506 |         1 |     200 |    399996/600004 |
|  reversal |  1000000 |   32 | 0.000506 | 0.000602 |         1 |     200 |    399996/600004 |
|           |          |      |          |          |           |         |                  |
|    bridge |  1000000 |   32 | 0.000394 | 0.000488 |         1 |     200 |    499995/500005 |
|     helix |  1000000 |   32 | 0.000669 | 0.000746 |         1 |     200 |    499995/500005 |
|   trinity |  1000000 |   32 | 0.000398 | 0.000498 |         1 |     200 |    499995/500005 |
|  reversal |  1000000 |   32 | 0.000511 | 0.000590 |         1 |     200 |    499995/500005 |

</details>

![rotation graph](/images/contrev_piston_griesmills_juggler.png)

<details><summary><b>data table</b></summary>

|      Name |    Items | Type |     Best |  Average |     Loops | Samples |     Distribution |
| --------- | -------- | ---- | -------- | -------- | --------- | ------- | ---------------- |
|   contrev |  1000000 |   32 | 0.000717 | 0.000840 |         1 |     200 |         1/999999 |
|    piston |  1000000 |   32 | 0.002091 | 0.002398 |         1 |     200 |         1/999999 |
|griesmills |  1000000 |   32 | 0.002436 | 0.002565 |         1 |     200 |         1/999999 |
|   juggler |  1000000 |   32 | 0.000612 | 0.000701 |         1 |     200 |         1/999999 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000416 | 0.000487 |         1 |     200 |      1000/999000 |
|    piston |  1000000 |   32 | 0.000474 | 0.000549 |         1 |     200 |      1000/999000 |
|griesmills |  1000000 |   32 | 0.000490 | 0.000581 |         1 |     200 |      1000/999000 |
|   juggler |  1000000 |   32 | 0.001234 | 0.001339 |         1 |     200 |      1000/999000 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000448 | 0.000512 |         1 |     200 |     99999/900001 |
|    piston |  1000000 |   32 | 0.000534 | 0.000682 |         1 |     200 |     99999/900001 |
|griesmills |  1000000 |   32 | 0.000550 | 0.000619 |         1 |     200 |     99999/900001 |
|   juggler |  1000000 |   32 | 0.001279 | 0.001570 |         1 |     200 |     99999/900001 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000446 | 0.000528 |         1 |     200 |    199998/800002 |
|    piston |  1000000 |   32 | 0.000564 | 0.000630 |         1 |     200 |    199998/800002 |
|griesmills |  1000000 |   32 | 0.000578 | 0.000687 |         1 |     200 |    199998/800002 |
|   juggler |  1000000 |   32 | 0.001282 | 0.001783 |         1 |     200 |    199998/800002 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000444 | 0.000517 |         1 |     200 |    299997/700003 |
|    piston |  1000000 |   32 | 0.000517 | 0.000626 |         1 |     200 |    299997/700003 |
|griesmills |  1000000 |   32 | 0.000527 | 0.000612 |         1 |     200 |    299997/700003 |
|   juggler |  1000000 |   32 | 0.002259 | 0.002476 |         1 |     200 |    299997/700003 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000431 | 0.000502 |         1 |     200 |    399996/600004 |
|    piston |  1000000 |   32 | 0.000505 | 0.000610 |         1 |     200 |    399996/600004 |
|griesmills |  1000000 |   32 | 0.000509 | 0.000588 |         1 |     200 |    399996/600004 |
|   juggler |  1000000 |   32 | 0.001983 | 0.002230 |         1 |     200 |    399996/600004 |
|           |          |      |          |          |           |         |                  |
|   contrev |  1000000 |   32 | 0.000443 | 0.000543 |         1 |     200 |    499995/500005 |
|    piston |  1000000 |   32 | 0.000667 | 0.000755 |         1 |     200 |    499995/500005 |
|griesmills |  1000000 |   32 | 0.000681 | 0.000776 |         1 |     200 |    499995/500005 |
|   juggler |  1000000 |   32 | 0.001304 | 0.001475 |         1 |     200 |    499995/500005 |

</details>
