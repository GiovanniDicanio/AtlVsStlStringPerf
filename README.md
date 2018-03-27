# AtlVsStlStringPerf
Compare sorting and concatenation performance of ATL (CStringW) vs. STL (wstring) strings

by Giovanni Dicanio

## Overview

I wrote [some C++ code](../master/AtlVsStlStringPerf/AtlVsStlStringPerf.cpp) to compare the performance of the ATL's `CStringW` class vs. the C++ Standard Library's `std::wstring` class.

There are several aspects that can be considered when measuring string performance. In this case, I tested:

- **sorting** a vector of strings
- **concatenating** strings

I developed and compiled this code with **Visual Studio 2015 with Update 3**.

The code compiles cleanly in both 32-bit and 64-bit builds.

Moreover, defining the `TEST_TINY_STRING_PERFORMANCE` preprocessor macro, you can run the tests with _small_ strings. This is a good option to check the performance of the **SSO** (Small String Optimization) that is implemented in the `std::wstring` class (but _not_ in ATL's `CStringW`).


## Test Results

In my tests on a PC with an Intel Core i7 CPU @3.4GHz, 32GB RAM and Windows 10 64-bit, the **results** are as follow:

1. When SSO is _not enabled_, ATL `CStringW` is _faster_ than `std::wstring` in the **string sorting** test.
2. When SSO is _enabled_, `std::wstring` is _faster_ than `CStringW` in the **string sorting** test.
3. `std::wstring` is _always faster_ than `CStringW` in the **string concatenation** test.


### Sample Output (64-bit build, no SSO)

```
*** String Performance Tests -- by Giovanni Dicanio ***

[64-bit build]

Test string array contains 1600K strings.

=== String Sorting Test ===
ATL1: 2953.99 ms
STL1: 3344.76 ms
ATL2: 2929.95 ms
STL2: 3320.09 ms
ATL3: 2917.2 ms
STL3: 3349.65 ms

=== String Concatenation Test ===
ATL1: 67.862 ms
STL1: 37.6882 ms
ATL2: 59.1186 ms
STL2: 48.7215 ms
ATL3: 61.114 ms
STL3: 35.7283 ms

```

### Sample Output (64-bit build, SSO enabled)

```
*** String Performance Tests -- by Giovanni Dicanio ***

[64-bit build]

Testing in Tiny String Mode.

Test string array contains 1600K strings.

=== String Sorting Test ===
ATL1: 1212.8 ms
STL1: 628.804 ms
ATL2: 1240.4 ms
STL2: 618.088 ms
ATL3: 1235.07 ms
STL3: 624.333 ms

=== String Concatenation Test ===
ATL1: 39.7233 ms
STL1: 11.7282 ms
ATL2: 39.5487 ms
STL2: 12.3145 ms
ATL3: 40.7542 ms
STL3: 12.0191 ms

```
