# Student Grades Program

Systems Programming course assignment (C++).
The program reads students and their grades from a file, calculates the final grade,
sorts the students and splits them into two groups: **passed** (final >= 5.0) and **failed** (final < 5.0).
It also measures how fast each step is with different containers and strategies.

## Repository structure

    include/          header files
        Person.h        class Person (rule of three + move, >> and << operators)
        StudentIO.h     test file generation
        StudentOps.h    read / sort / split (4 strategies) / write - templates
        Timer.h         class Timer (std::chrono)
    src/              source files
        main.cpp        menu and speed tests
        Person.cpp
        StudentIO.cpp
    CMakeLists.txt    build with CMake (any OS)
    Makefile          build with make (Unix)
    run.bat           build and run on Windows (CMake + MinGW)

## How to build and run

### Linux / macOS (make)
1. Open a terminal in the project folder
2. Build: `make`
3. Run: `./students` (or `make run`)
4. Delete compiled files: `make clean`

### Any OS (CMake)
1. Build:

        cmake -S . -B build
        cmake --build build

2. Run from the project folder: `./build/students` (Windows: `build\students.exe`)

### Windows (double click)
1. Install CMake and Code::Blocks (with MinGW)
2. Double-click `run.bat` - it builds the program and starts it

The `students*.txt` data files must be in the folder where the program is started.

## How to use the program

    === Student Grades v1.0 ===
    1 - Generate missing test files
    2 - Speed test
    0 - Exit

1. Choose `1` to create the test files `students1000.txt` ... `students10000000.txt`
   (files that already exist are not overwritten)
2. Choose `2` for a speed test, then choose:
   - the container: vector, list, deque or all three
   - the strategy: 1, 2, 3, 4 or all
   - the largest file to test (1 000 ... 10 000 000)
3. Results are written to `passed<N>.txt` and `failed<N>.txt`

### Input file format

    Vardas      Pavarde     ND1  ND2 ... NDn  Egz.
    Vardas1     Pavarde1    8    5   ... 3    7

Any number of homework grades (ND); the last number is the exam.
`Final = 0.4 * homework average + 0.6 * exam` (a median version is also calculated).

---

# Releases

## v0.1
- Class `Person` with the rule of three and overloaded `>>` / `<<` operators
- Final grade by average or median, random grade generation
- Reading students from a file, sorted and formatted output

## v0.2
- Code split into classes and header files
- Exception handling (missing file, empty file, wrong grades, bad lines)
- Generation of 1 000 – 10 000 000 student files
- Students split into `passed` / `failed` files
- Speed analysis with `std::vector`:

| Records | Read (s) | Sort (s) | Split (s) | Write (s) | Total (s) |
|---------|----------|----------|-----------|-----------|-----------|
| 1 000 | 0.006 | 0.000 | 0.000 | 0.006 | 0.013 |
| 10 000 | 0.072 | 0.003 | 0.000 | 0.037 | 0.113 |
| 100 000 | 0.657 | 0.053 | 0.003 | 0.329 | 1.042 |
| 1 000 000 | 3.575 | 0.701 | 0.031 | 3.225 | 7.534 |
| 10 000 000 | 39.998 | 9.999 | 0.446 | 40.965 | 91.413 |

Reading and writing the files take about 90 % of the time.

## v0.25
- Same program with `std::vector`, `std::list` and `std::deque` (template functions)
- Results for 10 000 000 records:

| Container | Read (s) | Sort (s) | Split (s) | Write (s) | Total (s) |
|-----------|----------|----------|-----------|-----------|-----------|
| vector | 34.149 | 8.434 | 0.403 | 30.181 | 73.168 |
| list | 33.412 | 10.674 | 1.016 | 28.321 | 73.424 |
| deque | 32.402 | 8.160 | 0.724 | 27.368 | 68.655 |

`std::list` sorts the slowest; the total time is almost the same because reading/writing dominate.

## v1.0
- Menu to choose the container, the split strategy and the file size
- Four split strategies:

| Strategy | How it works | Memory |
|----------|--------------|--------|
| 1 | Copies every student to **two new** containers (`failed`, `passed`). `students` keeps everything | Most |
| 2 | Moves failed students to **one new** container and erases them one by one from `students` (loop + `erase`). Then `students` is shrunk to its new size | Less |
| 3 | Strategy 2 with `<algorithm>`: `std::find_if` finds the first failed student (the list is sorted), `std::move` moves the whole block, one `erase` removes it. For `std::list`: `splice` (no copy) | Less |
| 4 | Strategy 2 with `<algorithm>`: `std::stable_partition` puts passed first and failed last (works even if the data is not sorted), then `std::move` + one `erase` | Less |

- Makefile and CMakeLists.txt, clean repository (only source files)

### Results (seconds)

Strategy 2 erases students one by one. For `vector` and `deque` every `erase` shifts all following
elements, so the time grows as n² - files above 100 000 records are skipped (it would take hours).

**100 000 records**

| Container | Strategy | Read | Sort | Split | Write | Total |
|-----------|----------|------|------|-------|-------|-------|
| vector | 1 | ? | ? | ? | ? | ? |
| vector | 2 | ? | ? | ? | ? | ? |
| vector | 3 | ? | ? | ? | ? | ? |
| vector | 4 | ? | ? | ? | ? | ? |
| list | 1 | ? | ? | ? | ? | ? |
| list | 2 | ? | ? | ? | ? | ? |
| list | 3 | ? | ? | ? | ? | ? |
| list | 4 | ? | ? | ? | ? | ? |
| deque | 1 | ? | ? | ? | ? | ? |
| deque | 2 | ? | ? | ? | ? | ? |
| deque | 3 | ? | ? | ? | ? | ? |
| deque | 4 | ? | ? | ? | ? | ? |

**10 000 000 records**

| Container | Strategy | Read | Sort | Split | Write | Total |
|-----------|----------|------|------|-------|-------|-------|
| vector | 1 | ? | ? | ? | ? | ? |
| vector | 3 | ? | ? | ? | ? | ? |
| vector | 4 | ? | ? | ? | ? | ? |
| list | 1 | ? | ? | ? | ? | ? |
| list | 2 | ? | ? | ? | ? | ? |
| list | 3 | ? | ? | ? | ? | ? |
| list | 4 | ? | ? | ? | ? | ? |
| deque | 1 | ? | ? | ? | ? | ? |
| deque | 3 | ? | ? | ? | ? | ? |
| deque | 4 | ? | ? | ? | ? | ? |

### Memory (10 000 000 records, peak, measured with GCC on Linux)

| Container | Strategy 1 | Strategy 3 |
|-----------|-----------|-----------|
| vector | 3.1 GB | 2.7 GB |
| list | 3.7 GB | 1.8 GB |
| deque | 3.2 GB | 2.1 GB |

Strategy 1 really uses more memory, but for `vector` the difference is small: the biggest memory
use happens while reading, when the vector grows and copies itself to a bigger memory block.

### Conclusion
?

### Test computer
- PC: Dell Latitude 5480
- CPU: Intel Core i5-7200U @ 2.50 GHz (2 cores / 4 threads)
- RAM: 8 GB DDR4, 2400 MHz
- Disk: SSD SK hynix SC311 SATA 256 GB
- OS: Windows 10 Pro 64-bit (22H2)
- Compiler: GCC (MinGW), Release mode, C++17
