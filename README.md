# PostScript Interpreter (C++)

## Overview

This project is a simplified PostScript interpreter implemented in C++. It supports:

* Operand stack and dictionary stack
* Built-in operators (arithmetic, stack, string, boolean, control flow)
* User-defined functions (`def`)
* Code blocks (`{ ... }`)
* **Dynamic and Static scoping**

---

## Requirements

* C++17 compatible compiler
* CMake (version 3.10 or higher)

---

## Build Instructions (CMake)

From the root of the project:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

After building, the `build/` directory will contain:

- `ps_interpreter` → interactive REPL
- `runTests` → GoogleTest test executable

---

## Running the Interpreter

From the `build` directory:

```bash
./ps_interpreter
```

You will see the REPL prompt:

```
REPL>
```

---
## Running Tests

This project uses **GoogleTest** for unit testing, integrated with CMake and CTest

<br>

Run the test executable directly (recommended):

```bash id="gtest3"
./runTests
```

or use CTest:

```bash id="gtest2"
ctest
```

---

## Scoping Modes

The interpreter supports:

* **Dynamic Scoping** (PostScript default)
* **Static Scoping**

### How to switch

In `interpreter.cpp`, modify:

```cpp
bool STATIC_SCOPING = true;
```

* `true` → Static scoping
* `false` → Dynamic scoping

Rebuild the project after changing this value.

---

## Scoping Example

```
/x 10 def
/g { x } def

1 dict begin
    /x 20 def
    g
end
```

Expected output:

* Dynamic → `20`
* Static → `10`

---

## Example Usage of operations

### Arithmetic 

```
REPL> 3 4 add  
OP_STACK = [ 7 ]

REPL> 10 2 div  
OP_STACK = [ 5 ]

REPL> 5 3 mul  
OP_STACK = [ 15 ]
```


### Variables

```
REPL> /x 10 def  
REPL> x  
OP_STACK = [ 10 ]

REPL> /x 20 def  
REPL> x  
OP_STACK = [ 20 ]
```

### Functions 

```
REPL> /square { dup mul } def  
REPL> 5 square  
OP_STACK = [ 25 ]
```

### Control Flow

```
REPL> true { 42 print } if  
42

REPL> false { 99 print } if  
(no output)

REPL> true { 1 print } { 2 print } ifelse  
1
```

### Comparisons

```
REPL> 5 5 eq  
OP_STACK = [ true ]

REPL> 3 10 lt  
OP_STACK = [ true ]
```

### Loops

```
REPL> 3 { 100 print } repeat  
100  
100  
100  
```

### Dictionary Stack

```
REPL> 10 dict begin  
REPL> /x 5 def  
REPL> x  
OP_STACK = [ 5 ]  
REPL> end
```

---
