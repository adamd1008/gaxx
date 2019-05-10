# gaxx
Simple genetic algorithm framework using C++

## Simple example

I've included an example called `minecraft_seed.cpp` which finds colliding seed strings in Minecraft. In other words, it finds collisions in Java's implementation of `String.hashCode()` which is used by Minecraft to turn a seed string into an integer for generating worlds. It may be more correct to simply call it a "general Java `String.hashCode()` collision finder".

This is possible because Java's implementation of `hashCode()` for strings isn't derived from a cryptographic hash function; it's much simpler than that (see `SeedGenotype::hashCode()` in minecraft_seed.cpp).

### Usage

On Windows, you can create a "project from existing code" to compile the code. Make sure you choose a Visual C++ project with type "console application project".

On Linux, run `make`.

Once built, use a command-line terminal to navigate to the directory with the executable. Run it with no arguments (Linux example):

```
$ ./minecraft_seed 
Usage: ./minecraft_seed <pop#> <breed#> <gen#> <hash>
```

I made a quick tool in Java to print the hash code of a given string. So I know that the hash code for "adamd1008" is -1934068580. But really, you can pick whatever hash code you like. But it must be a valid value for a [signed 32-bit integer](https://en.wikipedia.org/wiki/Integer_(computer_science)#Common_integral_data_types).

Let's give it this hash code and some sensible arguments for population, breeding and generation count to find another string that gives the same hash code:

```
$ ./minecraft_seed 100 30 1000 -1934068580
[G0]	| BEST = 15865636	| "C,JdY#HD:.a1]I4Q=t*$ w~"
[G1]	| BEST = 15865636	| "C,JdY#HD:.a1]I4Q=t*$ w~"
...
[G10]	| BEST = 5244563	| "oaH<+|;K7yGZR_|v.@?d}Jq9Yhz.G1^^`$i"
...
[G100]	| BEST = 210	| "PK(paH"
[G1000]	| BEST = 0	| "PK(phA"
```

I've removed quite a lot of lines of output for brevity.

So the string "PK(phA" gives the same hash code as "adamd1008". We know this because, in this `minecraft_seed.cpp` example, the "fitness" of a string is how close its hash code is to the target hash code. Because we found a string with zero fitness, we have found an exact match. You may need to run the program multiple times, or change the arguments, to find a string with zero fitness.

So, more formally, we are trying to minimise the error of the `hashCode()` function for an input compared to an already known output.
