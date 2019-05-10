# gaxx

Simple genetic algorithm framework using C++

## Simple example: Minecraft world seeds

### Background

Minecraft can generate worlds based on a specified seed value. This is a signed 64-bit integer value used to ["seed" a pseudorandom number generator](https://en.wikipedia.org/wiki/Random_seed), which churns out values to create the world and its features.

The game provides a text box for you to type the seed when creating a new world, under the "More World Options..." menu.

If the specified seed value can be parsed as a signed 64-bit integer then it will use this exact number as the seed; if not, it will take the given string and calculate a signed 32-bit integer seed using the `String.hashCode()` function.

Trivia: Minecraft will silently ignore the number zero given as a seed, and will randomly create a seed instead without telling you. However, it will not reject a string which produces a hash code of zero (which might not actually be possible with the set of printable characters; I'm not sure).

### Using GA to find colliding strings

I've included an example called `minecraft_seed.cpp` which finds [colliding seed strings](https://en.wikipedia.org/wiki/Collision_(computer_science)) for generating Minecraft worlds. In other words, it finds collisions in Java's implementation of `String.hashCode()` which is used by Minecraft to turn a seed string into an integer for creating worlds. It may be more correct to simply call it a "general Java `String.hashCode()` collision finder".

This is possible because Java's implementation of `hashCode()` for strings isn't derived from a [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function), which must have strong [collision resistance](https://en.wikipedia.org/wiki/Collision_resistance). It is a much simpler hashing algorithm (see the `hashCode()` function in minecraft_seed.cpp).

### Compilation

On Windows, you can create a "project from existing code" to compile the code. Make sure you choose a Visual C++ project with type "console application project".

On Linux, run `make`.

Alternatively, use the supplied 64-bit Windows binary.

### Usage

Use a command-line terminal to navigate to the directory with the executable. Run it with no arguments:

```
D:\Work\gaxx\src\x64\Release>gaxx
Usage: gaxx <pop#> <breed#> <gen#> <hash>
```

I made a quick tool in Java to print the hash code of a given string. There are a couple of online tools to do this also, for example [here](http://jsfiddle.net/Ciul/w42en/).

So I know that the hash code for "adamd1008" is -1934068580. But really, you can pick whatever hash code you like, provided it is a valid value for a [signed 32-bit integer](https://en.wikipedia.org/wiki/Integer_(computer_science)#Common_integral_data_types).

Let's give it this hash code and some sensible arguments for population, breeding and generation count to find another string that gives the same hash code:

```
D:\Work\gaxx\src\x64\Release>gaxx 100 30 1000 -1934068580
[G0]    | BEST = 1839232        | "WSWZRdM"
[G1]    | BEST = 1839232        | "WSWZRdM"
[G2]    | BEST = 1839232        | "WSWZRdM"
[G3]    | BEST = 1839232        | "WSWZRdM"
[G4]    | BEST = 974272 | "WSV[r$O"
[G5]    | BEST = 974272 | "WSV[r$O"
[G6]    | BEST = 974272 | "WSV[r$O"
[G7]    | BEST = 923274 | "WSVZZ`L"
[G8]    | BEST = 923274 | "WSVZZ`L"
[G9]    | BEST = 686031 | "WSVR[dL"
[G10]   | BEST = 484470 | "WSUJRdI"
...
[G100]  | BEST = 120    | "WSUZZdO"
...
[G200]  | BEST = 9      | "WSUZZga"
...
[G300]  | BEST = 1      | "WSUZZgi"
...
[G1000] | BEST = 0      | "WSUZZgj"
```

I've removed quite a lot of lines of output for brevity.

So the string "WSUZZgj" gives the same hash code as "adamd1008". We know this because, in this `minecraft_seed.cpp` example, the "fitness" of a string is how close its hash code is to the target hash code. Because we found a string with zero fitness, we have found an exact match. You may need to run the program multiple times, or change the arguments, to find a string with zero fitness.

So, more formally, we are trying to minimise the error of the `hashCode()` function for an input compared to an already known output.

Running it again will produce a different output:

```
D:\Work\gaxx\src\x64\Release>gaxx 100 30 1000 -1934068580
[G0]    | BEST = 31796764       | "H}+<,xBfXNoaO8]2Z/2_|W.;hoJ#{5"
[G1]    | BEST = 28301456       | "|dxs(rycYD1y}IlosiH3n@t:9@k`jkD+sxHO>]-"
[G2]    | BEST = 10597171       | "UA|hpz(xPno}8;*"
[G3]    | BEST = 10597171       | "UA|hpz(xPno}8;*"
[G4]    | BEST = 10597171       | "UA|hpz(xPno}8;*"
[G5]    | BEST = 8624437        | "P@\b:k"
[G6]    | BEST = 282957 | "PH|b:k"
[G7]    | BEST = 282957 | "PH|b:k"
[G8]    | BEST = 282957 | "PH|b:k"
[G9]    | BEST = 223407 | "PH~b:K"
[G10]   | BEST = 223407 | "PH~b:K"
...
[G100]  | BEST = 210	| "PK(paH"
...
[G200]  | BEST = 3      | "PJHS*>"
...
[G300]  | BEST = 2      | "PJHS*?"
...
[G1000] | BEST = 2      | "PJHS*?"
```

This time we didn't find a collision. We only found a string that has a hash code with a difference of 2 from the target hash.
