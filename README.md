# C Utilities
A bunch of little utility headers you can add anywhere.
`ds.h` is the original header I made and has the content of all the other headers combined.
## Lists
A simple hidden-metadata dynamic vector with simple usage:
```c
  int *numbers = list_init(int);

    list_push(numbers, 3);
    list_push(numbers, 1);
    list_push(numbers, 2);
    list_push(numbers, 5);
    list_push(numbers, 7);

    printf("last: %i\n", list_last(numbers));
    list_pop(numbers);
    
    numbers[2] = 8;
    
    if(list_full(numbers)){
        list_resize(numbers, 32);
    }

    printf("size: %lu capacity: %lu\n", list_size(numbers), list_capacity(numbers));

    list_swap_delete(numbers, 2);

    printf("last number: %i\n", list_last(numbers));

    int *numbers2 = list_alloc(int, 4);

    list_push(numbers2, 9);
    list_push(numbers2, 8);
    list_push(numbers2, 7);
    list_push(numbers2, 6);

    list_join(numbers, numbers2);

    list_free(numbers2);

    printf("numbers: ");
    for(size_t i = 0; i < list_size(numbers); i++){
        printf("%i ", numbers[i]);
    }
    printf("\n");

    list_free(numbers);
```

## Hashtables
Comes in 3 varieties: `hash32_t` which uses 32 bit keys, `hash64_t` which uses 64 bit keys and `string_hash_t` which uses string keys
Demonstration TBD

## Strings
A C-compatible null-terminated string system with length, removes usual headache with strings in C.
Demonstration TBD
