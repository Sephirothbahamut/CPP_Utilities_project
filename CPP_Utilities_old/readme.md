# What is utils

This repository is a collection of utility headers with most of the code I noticed I ended up rewriting over and over. Being all header files, 
there's no need to include .lib files to the project.

# Usage Notes

 - Most of the headers assume to be compiled with at least C++17.
 - The files inside "include/utils" are mostly ready to be included and used in a C++ project. I only reccommend using the ones mentioned in this document, as some others are still not polished. 
 - The files inside "private" are used internally by other headers, but have no reason to be used alone.
 - The files inside "beta" should not be used without extensive reading and rewriting. They're very old code I haven't yet checked.
 - Some usage examples can be seen in the Tests folder.
 - The angles header in particular contains errors that still need fixing; it's present in include/utils only because it's referenced in some other files.

# Working headers descriptions

### Compilation constants
[_compilation/_](https://github.com/Sephirothbahamut/utils/tree/master/include/utils/compilation)

Will add global inline constexpr variables that mirror common compilation macros, in order to use them in if-constexpr conditions.

```c++
if constexpr(utils::compilation::release) { ... }
```

### Cout extensions
_[console_io.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/console_io.h), [cout_containers.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/cout_containers.h), [cout_utilities.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/cout_utilities.h)_

The files allow for more complex interactions with the console. 
In particular cout_containers allows to output all the standard containers. On Windows platforms the output will be coloured.
Some headers will make use of cout_utilities and colouring if they're included after cout_utilities (for example the data structures). 
Some others will include cout_utilities regardless (the async logger).

```c++
#include <utils/cout_containers>

std::vector<int> v{1, 2, 3};
std::cout << v; // Output: [1, 2, 3]
```

### Logger
_[logger.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/logger.h), [message.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/message.h)_

Delegate output operations to a dedicated thread, in order to save the main thread from output operations overhead.

### Tracking pointer
_[tracking.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/tracking.h)_

A "smart observer pointer" which follows resources in memory as long as they're being moved (not copied). It allows to follow objects stored in contiguous memory. 
The resource must follow strict criteria (either inherit from trackable, or be wrapped in a trackable_wrapper). 

For instance, traditionally one could store a vector of unique pointers to some entities, so that eventual observer pointers to those entities remained valid regardless of how the vector reallocates on growth.

With the tracking pointer, it is possible to observe the entities in a vector that directly owns them, even if vector growth causes reallocation, or if the vector content is sorted.

See the [tests](https://github.com/Sephirothbahamut/utils/blob/master/Tests/test_tracking.cpp) for examples.

### Wrapper
_[wrapper.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/wrapper.h)_

Useless alone. It exists as a pre-made class to inherit from any time a wrapper is needed. 
It exposes std::optional and std::\*\_pointer-like syntax for accessing the wrapped content.

### Algorithms
_algorithm/_

Additional algorithms with an stl-like syntax

### id_pool
_[id_pool.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/id_pool.h)_

Generates a pool of unique identifiers which can be released and recycled for further usage.

See the [tests](https://github.com/Sephirothbahamut/utils/blob/master/Tests/test_id_pool.cpp) for examples.

### Colors
_[graphics/color.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/graphics/color.h)_

Some quick color conversions (rgb, hsv, hsl).

### vec2
_[math/vec.h](https://github.com/Sephirothbahamut/utils/blob/master/include/utils/math/vec2.h)_

A bidimensional vector with most of the common functionalities. Comes with "custom operators" for the bravest to try.

NOTE: This header includes _angle.h_ to keep the interface complete. However, any operation that involves angles is not guaranteed to work properly, as the angles header is not bug-free yet.

### Polymorphic value
_polymorphic_value.h_

THIS CODE IS __NOT__ MINE. It's in the collection only because I use it frequently.

Click [here](https://github.com/jbcoe/polymorphic_value) for the rightful source.
