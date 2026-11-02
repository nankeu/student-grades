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

    Vardas      Pavarde     ND1  ND2 ... NDn  Egz.
    Vardas1     Pavarde1    8    5   ... 3    7

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
| 1 000 | 0.006 | 0.000 | 0.000 | 0.006 | 0.013 |
| 10 000 | 0.072 | 0.003 | 0.000 | 0.037 | 0.113 |
| 100 000 | 0.657 | 0.053 | 0.003 | 0.329 | 1.042 |
| 1 000 000 | 3.575 | 0.701 | 0.031 | 3.225 | 7.534 |
| 10 000 000 | 39.998 | 9.999 | 0.446 | 40.965 | 91.413 |

### Test computer
- PC: Dell Latitude 5480
- CPU: Intel Core i5-7200U @ 2.50 GHz (2 cores / 4 threads)
- RAM: 8 GB DDR4, 2400 MHz
- Disk: SSD SK hynix SC311 SATA 256 GB
- OS: Windows 10 Pro 64-bit (22H2)
- Compiler: GCC (MinGW), Code::Blocks 25.03, Release mode, `-O2`, `-std=c++17`

### Conclusion
- **Reading and writing files take most of the time** (about 90 % of the total).
  For 10 000 000 records: reading 44 %, writing 45 %.
- **Sorting** takes about 10 % of the time.
- **Splitting is almost instant** (less than 1 %), because the list is already sorted:
  the "failed" students are at the end and are simply moved into a second vector.
- From 1 000 000 to 10 000 000 records the time grows about ×12 (not ×10).
  The 10 000 000 test uses about 2 GB of RAM on an 8 GB computer, and the result
  files (about 1 GB) are written to a OneDrive folder, which slows down writing.
