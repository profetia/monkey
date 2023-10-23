# Monkey interpreter

Monkey programming language interpreter designed in [_Writing An Interpreter In Go_](https://interpreterbook.com/), implemented in modern C++. While the original Monkey interpreter is written in Go.


## Build

The implementation of the interpreter is done in C++ and the build chain uses
CMake. See [CMake](https://cmake.org/) for more information.

Use the steps below to build and run the interpreter. The procedure need a
Unix-like build environment. On Windows it is possible to use
[Cygwin](https://www.cygwin.com/). Make sure to include a compiler for C++ and
CMake.

    mkdir build
    cd build
    cmake ..
    make all -j

This will build an executable in the folder build/bin/Debug.

## Getting started with Monkey

### Variable bindings and number types

You can define variables using `let` keyword. Supported number types are integers and floating-point numbers.

```sh
>> let a = 1;
>> a
1
>> let b = 0.5;
>> b
0.5
```

### Arithmetic expressions

You can do usual arithmetic operations against numbers, such as `+`, `-`, `*` and `/`. 

```sh
>> let a = 10;
>> let b = a * 2;
>> (a + b) / 2 - 3;
12
>> let c = 2.5;
>> b + c
22.5
```

### If expressions

You can use `if` and `else` keywords for conditional expressions. The last value in an executed block are returned from the expression.

```sh
>> let a = 10;
>> let b = a * 2;
>> let c = if (b > a) { 99 } else { 100 };
>> c
99
```

### Functions and closures

You can define functions using `fn` keyword. All functions are closures in Monkey and you must use `let` along with `fn` to bind a closure to a variable. Closures enclose an environment where they are defined, and are evaluated in *the* environment when called. The last value in an executed function body are returned as a return value.

```sh
>> let multiply = fn(x, y) { x * y };
>> multiply(50 / 2, 1 * 2)
50
>> fn(x) { x + 10 }(10)
20
>> let newAdder = fn(x) { fn(y) { x + y }; };
>> let addTwo = newAdder(2);
>> addTwo(3);
5
>> let sub = fn(a, b) { a - b };
>> let applyFunc = fn(a, b, func) { func(a, b) };
>> applyFunc(10, 2, sub);
8
```

### Strings

You can build strings using a pair of double quotes `""`. Strings are immutable values just like numbers. You can concatenate strings with `+` operator.

```sh
>> let makeGreeter = fn(greeting) { fn(name) { greeting + " " + name + "!" } };
>> let hello = makeGreeter("Hello");
>> hello("John");
Hello John!
```

### Arrays

You can build arrays using square brackets `[]`. Arrays can contain any type of values, such as integers, strings, even arrays and functions (closures). To get an element at an index from an array, use `array[index]` syntax.

```sh
>> let myArray = ["Thorsten", "Ball", 28, fn(x) { x * x }];
>> myArray[0]
Thorsten
>> myArray[4 - 2]
28
>> myArray[3](2);
4
```

### Hash tables

You can build hash tables using curly brackets `{}`. Hash literals are `{key1: value1, key2: value2, ...}`. You can use numbers, strings and booleans as keys, and any type of objects as values. To get a value of a key from a hash table, use `hash[key]` syntax.

```sh
>> let myHash = {"name": "Jimmy", "age": 72, true: "yes, a boolean", 99: "correct, an integer"};
>> myHash["name"]
Jimmy
>> myHash["age"]
72
>> myHash[true]
yes, a boolean
>> myHash[99]
correct, an integer
```

### Built-in functions

There are many built-in functions in Monkey, for example `len()`, `first()` and `last()`.

```sh
>> len("hello");
5
>> let myArray = ["one", "two", "three"];
>> len(myArray)
3
>> first(myArray)
one
>> rest(myArray)
[two, three]
>> last(myArray)
three
>> push(myArray, "four")
[one, two, three, four]
>> puts("Hello World")
Hello World
null
```
