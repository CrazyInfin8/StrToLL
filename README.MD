# Custom `strtoll` implementation

A custom implementation of `strtoll` which functions very similar to the standard C `strtod` function with some minor differences:
1. this function skips over underscores to allow for underscore seperated numbers
2. numbers starting with 0 are not automatically treated as Octals (see details [below](##Options))
3. supports smalltalk style radix literals (see details below)
4. number systems with a radix less than 10 consume as many characters as it can (for example, in the original only `0` would be consumed in the string `0o12345678` with `o12345678` left behind wheras my implementation will consume as many digits it can resulting in `0o12334567` being consumed with only `8` left behind)
5. prefix `0X` is not treated as a hexadecimal literal

*There may be more inconsistancies. These are the minor ones due to design, simplicity, or what I have found through testing...*

## Options
There are some togglable options to change the parsers behavior. they can be enabled with `define`'s. Here are some options and what they do:
| Option | Description | Examples |
--- | --- | ---
|`ALLOW_SMALLTALK_RADIX_LITERALS`|Allows the parser to parse smalltalk style radix literals. radix literals are number literals that start with a base 10 number up to 36, which will be treated as the radix, followed by the character `r`. The rest of the literal is treated as the actual value number using that radix. more info [here](https://en.wikipedia.org/wiki/Smalltalk#Literals) |`16rABCDEF` would be the same as `0xABCDEF`; `2r1010101` would be the same as `0b1010101`; `8r1234567` would be the same as `0o1234567`; `36rA_NUMBER` would have the value of `23209975107`|
|`LEADING_ZERO_AS_OCTAL`|If a `0` character is detected, treat the number as an octal or decimal|`0123` would have the value of `83` instead of `123`|
|`INVALIDATE_SUB_BASE10`|(unimplemented for simplicity) whether to not be greedy consuming digits when there is a prefix and the base of the number is less than 10 | `0o12345678` would consume only `0` with `o12345678` remaining instead of consuming `0o1234567` with `8` remaining