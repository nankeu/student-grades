# Student Grades Program

Systems Programming course assignment (C++).

## Release v0.25

### What's new compared to v0.2
- The same program now works with **three containers**: `std::vector`, `std::list`, `std::deque`
- Read / sort / split / write functions are **templates** in `StudentOps.h`
- `std::list` uses its own `list.sort()` and `splice()` (no copying when splitting)
- Speed comparison of the three containers on the same 5 files

### Project files
| File | Content |
|------|---------|
| `Person.h` / `Person.cpp` | Class `Person` (rule of three + move, `>>` and `<<` operators, final grade by average and median) |
| `StudentIO.h` / `StudentIO.cpp` | Test file generation |
| `StudentOps.h` | Template functions: read, sort, split, write (vector / list / deque) |
| `Timer.h` | Class `Timer` for time measurement (`std::chrono`) |
| `main.cpp` | Menu and speed test for the three containers |

### Speed analysis (seconds)
| Records | Container | Read | Sort | Split | Write | Total |
|---------|-----------|------|------|-------|-------|-------|
| 1 000 | vector | 0.005 | 0.000 | 0.000 | 0.005 | 0.011 |
| 1 000 | list | 0.007 | 0.000 | 0.000 | 0.009 | 0.021 |
| 1 000 | deque | 0.005 | 0.000 | 0.000 | 0.006 | 0.012 |
| 10 000 | vector | 0.054 | 0.002 | 0.000 | 0.030 | 0.087 |
| 10 000 | list | 0.051 | 0.002 | 0.000 | 0.035 | 0.089 |
| 10 000 | deque | 0.056 | 0.005 | 0.001 | 0.029 | 0.091 |
| 100 000 | vector | 0.609 | 0.054 | 0.003 | 0.802 | 1.469 |
| 100 000 | list | 1.012 | 0.041 | 0.008 | 0.339 | 1.400 |
| 100 000 | deque | 0.668 | 0.039 | 0.006 | 0.435 | 1.149 |
| 1 000 000 | vector | 3.724 | 0.695 | 0.027 | 3.361 | 7.808 |
| 1 000 000 | list | 3.680 | 0.700 | 0.089 | 3.376 | 7.846 |
| 1 000 000 | deque | 3.455 | 0.685 | 0.069 | 3.522 | 7.731 |
| 10 000 000 | vector | 34.149 | 8.434 | 0.403 | 30.181 | 73.168 |
| 10 000 000 | list | 33.412 | 10.674 | 1.016 | 28.321 | 73.424 |
| 10 000 000 | deque | 32.402 | 8.160 | 0.724 | 27.368 | 68.655 |

### Conclusion
- **Reading and writing files take about 85 % of the time** for every container,
  so the total time is almost the same for vector, list and deque.
- **Sorting:** `std::list` is the slowest (10.7 s for 10 000 000 records, about 25 % slower
  than vector and deque), because its elements are not stored next to each other in memory.
- **Splitting:** `std::vector` is the fastest (0.40 s), `std::list` the slowest (1.02 s),
  because searching through a linked list is slow.
- **Reading:** `std::deque` is slightly faster than `std::vector`, because it does not need
  to move all elements to a new memory block when it grows.
- **Best overall for 10 000 000 records:** `std::deque` (68.7 s), then vector (73.2 s) and list (73.4 s).
- Differences under ~10 % can also come from the computer (disk cache, other programs),
  because the three tests run one after another on the same files.

### Test computer
- PC: Dell Latitude 5480
- CPU: Intel Core i5-7200U @ 2.50 GHz (2 cores / 4 threads)
- RAM: 8 GB DDR4, 2400 MHz
- Disk: SSD SK hynix SC311 SATA 256 GB
- OS: Windows 10 Pro 64-bit (22H2)
- Compiler: GCC (MinGW), Code::Blocks 25.03, Release mode, `-O2`, `-std=c++17`

### How to run
1. Put the `students*.txt` files in the same folder as the program
2. Run the program and choose:
   - `1` – generate the missing test files (existing files are not overwritten)
   - `2` – run the speed test for vector, list and deque

---

## Release v0.2 (summary)
- Code split into classes and header files, exception handling
- Generation of 1 000 – 10 000 000 student files
- Students split into `passed` / `failed` files
- Speed analysis with `std::vector` (see branch `v0.2`)
