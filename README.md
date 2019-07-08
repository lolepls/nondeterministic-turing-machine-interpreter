# nondeterministic-turing-machine-interpreter
Nondeterministic Turing machine interpreter and simulator, written in C.

# Overview

This is the implementation of a single tape non-deterministic turing machine interpreter. 
It's written in standard C language, using only standard libraries, and nothing more.

The turing machine must be given to the algorithm via standard input, by specifying its structure and the input strings that shall be evaluated. For each input string, the algorithm will print on standard output the following values:

```
1 (if the string is accepted by the turing machine;)
0 (if the string is not accepted by the turing machine;)
U (if the turing machine hasn't completed the computation on a user-defined number of steps.)
```

You can write your own turing machine on a simple .txt file, following the instruction given on the section "Input file structure", or you can just try the interpreter with some of the turing machines provided by me. You can find them in the "mt" folder.

# Conventions 

The tape of the machine is bi-infinite, and its symbols are characters. The states of the turing machine are described with integer numbers. The blank symbol is represented by using underscore character "_".

The machine always starts its computation from the state 0 and from the first character loaded in the input tape.
Its input tape is bi-infinite and it contains the blank character on every non initialized cell.
The head can move to the left, to the right or stay on the same position. Those movements are represented by using R, L and S characters in the input file.


# Input file structure

The input file must describe the structure of a turing machine, and it must also contain the strings the machine has to load into the input tape for the evaluation. I suggest you to open one of the given .txt files on the folder mt/inputs and to look at it while reading the following informations.


## tr

The first section starts with "tr". This section contains the table of instructions of the machine, one for each line.
Every character here must be separated from the others with a space. Let me use an example to explain the instructions:


```
tr
0 a c R 1
```

This instruction means that if the machine is on the state 0 and the head reads the character "a" on the tape, it writes the character "c" on the tape, moves the head to the right and goes to the state 1.
So, the convention used in order to represernt the instructions is:


```
[current_state] [read_symbol] [written_symbol] [head_movement] [next_state]

```
If the machine can not perform any move, it can not proceed on the computation. So it prints "0" on standard output, meaning that the input string is not accepted by that turing machine.

## acc

This section contains the accepting states, one for each line. For example:


```
acc
3
4
8

```

means that 3, 4 and 8 are the accepting states. If the turing machine reaches one of them while computing a string, it means that the string is accepted. The turing machine ends its computation and prints "1" on standard output. There must be no transitions starting from an accepting state.

## max

In order to avoid the problem of infinite computations (i.e. the turing machine can not finish the computation, and goes on forever) you have to specify the max number of steps after which the turing machine marks the current input string as U (unknown).
Everytime the turing machine performs a transition takes a step. After max steps, the computations ends.


```
max
50000

```

This means that after 50000 steps the turing machine stops and prints "U" on standard output.

## run

On the run section, you have to specify the input strings that the machine has to evaluate. Every string must end with newline character \n.


```
run
aaa
aaabaa
aaccdddaaacc

```

This means that the turing machine will compute sequentially the three strings, printing 0, 1 or U for each one of them.

## Example of a turing machine structure file

So, an extremely simple turing machine file looks like this:


```
tr
0 a a R 0
0 a a R 1
1 _ a S 2
acc
2
max
8
run
aaaaa
aaaaaaaaa
aaaba
a
```
This is a turing machine with 3 states (0, 1 and 2) that accepts every string of seven or less "a".
The output of the algorithm with this machine given as input will be the following:

```
1
U
0
1
```
# Test cases

You can test the code with the inputs/outputs provided in the "tests" directory. 
These tests have a good level of complexity and they stress the time/space used by the code.

