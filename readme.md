# What is utils

This repository is a collection of utility headers with most of the code I noticed I ended up rewriting over and over. Being all header files, 
there's no need to include .lib files to the project.

# Usage Notes

 - Most of the headers assume to be compiled with at least C++17.
 - The files inside "include/utils" are ready to be included and used in a C++ project. 
 - The files inside "private" are used internally by other headers, but have no reason to be used alone.
 - The files inside "beta" should not be used without extensive reading and rewriting. They're very old code I haven't yet checked.
 - Some usage examples can be seen in the Tests folder.

# Some headers descriptions

### Compilation constants
_compilation/_
Will add global inline constexpr variables that mirror common compilation macros, in order to use them in if-constexpr conditions.

```c++
if constexpr(utils::compilation::release) { ... }
```

### Cout extensions
_console_io.h, cout_containers.h, cout_utilities.h_
The files  allow for more complex interactions with the console. 
In particular cout_containers allows to output all the standard containers. On Windows platforms the output will be coloured.
Some headers will make use of cout_utilities and colouring if they're included after cout_utilities (for example the data structures). 
Some others will include cout_utilities regardless (the async logger).

```c++
#include <utils/cout_containers>

std::vector<int> v{1, 2, 3};
std::cout << v; // Output: [1, 2, 3]
```

### Logger
_logger.h, message.h_
Delegate output operations to a dedicated thread, in order to save the main thread from output operations overhead.

### Tracking pointer
_tracking.h_
A "smart observer pointer" which follows resources in memory when its ownership moves around. 
The resource must follow strict criteria (either inherit from trackable, or be wrapped in a trackable_wrapper). 
See the [tests](https://github.com/Sephirothbahamut/utils/blob/master/Tests/test_tracking.cpp) for examples.

### Wrapper
_wrapper.h_
Useless alone. It exists as a pre-made class to inherit from any time a wrapper is needed. 
It exposes std::optional and std::\*\_pointer-like syntax for accessing the wrapped content.

### Algorithms
_algorithm/_
Additional algorithms with an stl-like syntax

### Containers
_containers/_
stl-like containers. Note: Custom allocatores are not supported (yet).

### Math
_math/_
Math-related utilities. Some are already present in recent C++ standards (lerp, constants...).

### Polymorphic value
_polymorphic_value.h_
THIS CODE IS __NOT__ MINE. It's in the collection only because I use it frequently.
Click [here](https://github.com/jbcoe/polymorphic_value) for the actual source (potentially more updated).
