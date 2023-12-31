# How to run tests

Running the standard test is easy:
```bash
# assuming you're at the root
cd tests/ && make test
```
This will perform a test at all optimisation levels and give a summary of the files tested in the file *test_results.log*.
Use 
```bash
make test_clean
```
to remove all the files generated during testing

## Generating your own tests

All test files should be formatted as thus:

## File layout

The actual file layout is a CSV table. The first line should give the array to be tested. The second should be a newline with no carriage return. The third should then list the expected first row of the 2D array (which should be the same as the line 1 since the perms are in lexicographic ordering). The subsequent lines should fill out the rest of the array and the file should end with a newline.

See the files in [tests/DATA](https://github.com/BloatedMonke/CPerm/tree/main/tests/DATA) for examples.

### Primitive types

{func}{n}{k}{type}.[dat]

where:
- **func**: Function that we're testing. This should be a single letter. See the [function code table](https://github.com/BloatedMonke/CPerm/blob/main/tests/README.md#function-codes)
- **n**: The length of the array
- **k**: Partial permutation order. If the function does not have one, i.e cartesian\_product, then this should be the length of the generated rearrangement arrays. In the case of cartesian\_product this would be 2.
- **type**: The primitive type (single token). The point of this is to test different sizes,  the actual type doesn't matter too much. Hence any variant of a primitive type which is the same size need not be tested. See the [available types table](https://github.com/BloatedMonke/CPerm/blob/main/tests/README.md#available-primitive-types)
- .***dat***: The file extension for the comparison files. *dat* is an optional name but **the period must be there**. Get as creative as you please.

### Struct dumps

Use the create-dump script. See
```bash
tests/create-dump.sh --help
```
for more info.

### Function Codes

|     Function      | OPcode | Implemented |
|-------------------|--------|-------------|
|    Permutations   |   P    |      ✔️      |
|    Combinations   |   C    |      ✔️      |
| Cartesian-product |   c    |             |
|      Product      |   p    |             |

### Available Primitive Types

| Available Types |  Alias | Implemented |
|-----------------|--------|-------------|
|       char      |   char |       ✔️     |
|       int       |   int  |       ✔️     |
|      short      |  short |             |
|  long long int  |   lld  |             |
|      float      |  float |             |
|      double     | double |             |

