# Red-Black Tree & Hash Table — C Implementation
The implementations were done during subject DSA_B at FIIT STU.
C implementations of two fundamental data structures — a **Red-Black Tree (RBT)** and a **Hash Table (HT)** — along with a benchmarking harness that measures insert, search, and delete performance for both.

## Data Structures

### Hash Table
Open-addressing hash table using **double hashing** for collision resolution. The primary hash (`hash_dtk`) reinterprets the `double` value as a 64-bit integer via a union. The secondary hash (`hash_dp`) uses a polynomial function with randomly initialized coefficients. The table resizes automatically (4× the current size, rounded up to the next prime) when the load factor exceeds 0.8, and rehashes all existing entries into the new table. Deleted slots are marked with a `DELETED` tombstone to preserve probe chains.

### Red-Black Tree
Self-balancing BST storing `double` values. Maintains the standard RB properties through left/right rotations and recoloring after insertions and deletions. Uses a global sentinel `NIL` node in place of `NULL` to simplify boundary conditions. Supports export to `.dot` format for visualization with Graphviz.

## Project Structure

```
Red_Black_Tree.c / .h    # Red-Black Tree implementation
Hash_Table.c / .h        # Hash Table implementation
Tester.c                 # Benchmarking harness for both structures
```

## Benchmarking

`Tester.c` generates 1,000,000 random `double` values and runs configurable rounds of insert, search, and delete tests at increasing scales (1,000 → 10,000 → 100,000 operations). Results are written to text files:

```
RBT_INSERT_DATA.txt / RBT_SEARCH_DATA.txt / RBT_DELETE_DATA.txt
HT_INSERT_DATA.txt  / HT_SEARCH_DATA.txt  / HT_DELETE_DATA.txt
```

## Building & Running

```bash
gcc -O2 -o tester Tester.c Red_Black_Tree.c Hash_Table.c -lm
./tester
```

The program will prompt for the number of test attempts and the number of scaling thresholds per attempt.
