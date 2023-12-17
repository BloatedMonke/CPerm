# CPerm

A collection of combinatoric generators and iterators to be used with an array of arbitrary type.

## Usage

All iterators return a ```struct perm ```. This struct contains the generated 2D array, its height, width and the size of the object being permuted.

```c
#include <stdio.h>
#include "cperm.h"

struct Foo {
    int bar;
    char baz;
};

void foo_pretty_print(void *x)
{ 
    int y = ((struct Foo *)x)->bar;
    char c = ((struct Foo *)x)->baz;
    printf("%d%c", y, c);
}

int main(void)
{
    struct Foo foo[4];
    int n = sizeof(foo) / sizeof(*foo);
    int k = 2;
    
    for (int i = 0, c = 'a'; i < n; ++i, ++c) {
        foo[i].bar = i+1;
        foo[i].baz = c;
    }
    
    struct perm A = permutations(foo, n, k, sizeof(*foo));
    print_perm(&A, foo_pretty_print);

    /** Output
    [
    [1a, 2b],
    [1a, 3c],
    [1a, 4d],
    [4d, 1a],
    [4d, 2b],
    [4d, 3c],
    [1a, 3c],
    [1a, 4d],
    [1a, 2b],
    [4d, 2b],
    [4d, 3c],
    [4d, 1a]
    ]
    */
    /* Or iterate through it */
    for (int i = 0; i < perm_height(A); ++i) {
        for (int j = 0; j < perm_width(A); ++j) {
            /* Code */
        }
    }
}
```

## Building

A simple build (from the root):

```
gcc -Iinclude/ -o cperm.o -c src/cperm.c
```

Known to compile with gcc & clang.

## Functions

As of right now only permutations and combinations are available. The plan is to add a lot more behaviour and functions. Contributions are welcome (see [CONTRIBUTING.md](https://github.com/BloatedMonke/CPerm/blob/main/CONTRIBUTING.md)).

## Planned Features

Some examples of planned features::

### Functions

- cartesian product
- affine permutations

### Behaviour

- generate perms on the fly so that not as much RAM is not used up

## License

CPerm is licensed under the MIT license. Refer to the [LICENSE](https://github.com/BloatedMonke/CPerm/blob/main/LICENSE) file
