# project1

Implementation of **Locality-Sensitive Hashing (LSH)**, **Hypercube (Random Projection)** nearest-neighbour search, and **k-Means clustering** in C++. The project provides exact and approximate nearest neighbour search algorithms, along with a clustering tool that can use either method for its range-search step.

## Overview

The project builds three executables:

- **`lsh`** – Approximate and exact nearest neighbour / range search using LSH.
- **`cube`** – Approximate and exact nearest neighbour / range search using the Hypercube (Random Projection) method.
- **`cluster`** – k-Means clustering, optionally accelerated with LSH or Hypercube range search, with Silhouette score evaluation.

## Project Structure

```
.
├── Makefile              # Build rules for lsh, cube, and cluster executables
├── cluster.conf           # Sample configuration file for the clustering algorithm
├── header/                # Header files (.h)
│   ├── Dataset.h
│   ├── HashTable.h
│   ├── Hypercube.h
│   ├── InputParser.h
│   ├── Point.h
│   ├── clustering.h
│   ├── neighbour_search.h
│   └── vector_operations.h
├── src/                   # Implementation files (.cpp)
│   ├── Dataset.cpp
│   ├── HashTable.cpp
│   ├── Hypercube.cpp
│   ├── clustering.cpp
│   ├── neighbour_search.cpp
│   └── vector_operations.cpp
├── src_main/              # Entry points for each executable
│   ├── lsh_main.cpp
│   ├── hcube_main.cpp
│   └── cluster_main.cpp
└── output_samples/        # Example output files
```

## Building

The project uses a simple `Makefile` and requires a C++11-compatible compiler (`g++`).

```bash
make            # builds lsh, cube, and cluster executables
make clean      # removes built executables and object files
```

## Usage

### LSH (`lsh`)

Performs exact and approximate nearest neighbour search, as well as range search, using LSH.

```bash
./lsh -i <input_file> -q <query_file> -o <output_file> -k <k> -L <L> -N <N> -R <R>
```

| Flag | Description | Default |
|------|-------------|---------|
| `-i` | Input dataset file | — |
| `-q` | Query dataset file | — |
| `-o` | Output file | — |
| `-k` | Number of hash functions per hash table | 4 |
| `-L` | Number of hash tables | 5 |
| `-N` | Number of nearest neighbours to return | 1 |
| `-R` | Radius for range search | 10000 |

Any file paths not provided on the command line will be requested interactively. After each query, you will be prompted to run another query.

### Hypercube (`cube`)

Performs nearest neighbour and range search using the Hypercube / Random Projection method.

```bash
./cube -i <input_file> -q <query_file> -o <output_file> [options]
```

### Clustering (`cluster`)

Performs k-Means clustering on the input dataset, optionally using LSH or Hypercube for the range search step, and reports the Silhouette score.

```bash
./cluster -i <input_file> -c <conf_file> -o <output_file> -m <method> [-complete]
```

| Flag | Description |
|------|-------------|
| `-i` | Input dataset file |
| `-c` | Configuration file (see `cluster.conf`) |
| `-o` | Output file |
| `-m` | Clustering method: `Classic`, `LSH`, or `Hypercube` |
| `-complete` | Include full list of point IDs per cluster in the output |

#### Configuration file (`cluster.conf`)

```
number_of_clusters: 30
number_of_vector_hash_tables: 20
number_of_vector_hash_functions: 15
max_number_M_hypercube: 1000
number_of_hypercube_dimensions: 20
number_of_probes: 50
```

| Parameter | Description |
|-----------|--------------|
| `number_of_clusters` | Number of clusters (k) for k-Means |
| `number_of_vector_hash_tables` | Number of LSH hash tables (L) |
| `number_of_vector_hash_functions` | Number of LSH hash functions per table (k) |
| `max_number_M_hypercube` | Max candidates to check (M) for Hypercube search |
| `number_of_hypercube_dimensions` | Reduced dimension (d') for Hypercube projection |
| `number_of_probes` | Number of probes for Hypercube search |

#### Output

The clustering output includes, per cluster: size, centroid, and (optionally) member point IDs, followed by total clustering time and the Silhouette score.

## Sample Output

Example outputs produced by the algorithms can be found in the [`output_samples/`](output_samples) directory.

## Requirements

- C++11-compatible compiler (e.g. `g++`)
- `make`

## License

No license specified.
