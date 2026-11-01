# Student Grades Program

Systems Programming course assignment (C++).

## Release v0.2

### What's new compared to v0.1
- **Refactoring:** code split into classes and several `.h` / `.cpp` files
- **Exception handling:** missing file, empty file, bad lines, wrong grades
- **File generator:** creates random student files (1 000 – 10 000 000 records)
- **Splitting:** students are divided into two files:
  - `failed*.txt` – final grade **< 5.0**
  - `passed*.txt` – final grade **>= 5.0**
- **Speed analysis:** time measured for reading, sorting, splitting and writing

### Project files
| File | Content |
|------|---------|
| `Person.h` / `Person.cpp` | Class `Person` (rule of three + move, `>>` and `<<` operators, final grade by average and median) |
| `StudentIO.h` / `StudentIO.cpp` | File generation, reading, sorting, splitting, writing |
| `Timer.h` | Class `Timer` for time measurement (`std::chrono`) |
| `main.cpp` | Menu and speed test |

### Input file format
Any number of homework grades (ND) is accepted. The last number is always the exam.

### Final grade formula
`Final = 0.4 * homework average + 0.6 * exam`
(a median version is also computed: `0.4 * homework median + 0.6 * exam`)

### Exception handling
| Situation | What happens |
|-----------|-------------|
| File does not exist | Error message, program continues with next file |
| File is empty | Error message |
| Grade not in 1–10 | Line skipped, message shown |
| Letter instead of number | Line skipped, message shown |
| Missing exam grade | Line skipped, message shown |
| Wrong menu choice | Error message, program stops |

### How to run
1. Put the `students*.txt` files in the same folder as the program
2. Run the program
3. Choose:
   - `1` – generate the missing test files (existing files are not overwritten)
   - `2` – run the speed test on all 5 files

### Speed analysis (std::vector)
| Records | Read (s) | Sort (s) | Split (s) | Write (s) | Total (s) |
|---------|----------|----------|-----------|-----------|-----------|
| 1 000 | ? | ? | ? | ? | ? |
| 10 000 | ? | ? | ? | ? | ? |
| 100 000 | ? | ? | ? | ? | ? |
| 1 000 000 | ? | ? | ? | ? | ? |
| 10 000 000 | ? | ? | ? | ? | ? |

### Test computer
- CPU: ?
- RAM: ?
- Disk: ? (SSD / HDD)
- Compiler: GCC (MinGW), Code::Blocks 25.03, Release mode, `-O2`, `-std=c++17`

### Conclusion
Reading the file takes the most time (about 60 % of the total). Sorting is the second
slowest step. Splitting is very fast because the list is already sorted and the
"failed" students are simply moved from the end of the vector.
