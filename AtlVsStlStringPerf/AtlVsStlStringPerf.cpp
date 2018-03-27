///////////////////////////////////////////////////////////////////////////////
//
// *** ATL vs. STL String Performance Tests -- by Giovanni Dicanio ***
// 
// This code tests both sorting string arrays and string concatenation.
// 
// The ATL CStringW class is compared against the std::wstring class.
// 
///////////////////////////////////////////////////////////////////////////////


//=============================================================================
//                                  Includes
//=============================================================================

#include <algorithm>    // std::shuffle, std::sort
#include <exception>    // std::exception
#include <iostream>     // std::cout
#include <random>       // std::mt19937
#include <string>       // std::wstring, std::string
#include <vector>       // std::vector

#include <Windows.h>    // Windows Platform SDK
#include <atlexcept.h>  // CAtlException
#include <atlstr.h>     // ATL CStringW

using namespace std;
using namespace ATL;

// Define for tiny strings (STL strings enable SSO)
//#define TEST_TINY_STRING_PERFORMANCE


//=============================================================================
//                        Performance Counter Helpers
//=============================================================================

long long Counter() 
{
    LARGE_INTEGER li;
    ::QueryPerformanceCounter(&li);
    return li.QuadPart;
}

long long Frequency() 
{
    LARGE_INTEGER li;
    ::QueryPerformanceFrequency(&li);
    return li.QuadPart;
}

void PrintTime(const long long start, const long long finish, const char * const s) 
{
    cout << s << ": " << (finish - start) * 1000.0 / Frequency() << " ms \n";
}


//=============================================================================
//                           Performance Tests
//=============================================================================

// Concatenate groups of strings contained in the input string vector
// (used in the concatenation performance test)
template <typename StringType>
void StringConcat(const vector<StringType>& v)
{
    // Concatenate strings in groups of 'kConcatenationCount'
    constexpr size_t kConcatenationCount = 6;

    const size_t maxIter = v.size() / kConcatenationCount;

    for (size_t i = 0; i < maxIter; i += kConcatenationCount)
    {
        // Concatenate indexes from 'i' to 'i+kConcatenationCount-1':
        // make sure they are in valid range
        ATLASSERT((i >= 0) && ((i + kConcatenationCount - 1) < v.size()));

        wstring result = v[i] + v[i + 1] + v[i + 2] + v[i + 3];
        result += v[i + 4];
        result += v[i + 5];
    }
}


void RunTests()
{
    cout << "\n*** String Performance Tests -- by Giovanni Dicanio *** \n\n";

#ifdef _WIN64
    cout << "[64-bit build] \n\n";
#else
    cout << "[32-bit build] \n\n";
#endif

#ifdef TEST_TINY_STRING_PERFORMANCE
    cout << "Testing in Tiny String Mode. \n\n";
#endif // TEST_TINY_STRING_PERFORMANCE

    // Build a vector of strings generated starting from "Lorem Ipsum"
    const auto shuffled = []() -> vector<wstring> 
    {
        const wstring lorem[] = 
        {
            L"Lorem ipsum dolor sit amet, consectetuer adipiscing elit.",
            L"Maecenas porttitor congue massa. Fusce posuere, magna sed",
            L"pulvinar ultricies, purus lectus malesuada libero,",
            L"sit amet commodo magna eros quis urna.",
            L"Nunc viverra imperdiet enim. Fusce est. Vivamus a tellus.",
            L"Pellentesque habitant morbi tristique senectus et netus et",
            L"malesuada fames ac turpis egestas. Proin pharetra nonummy pede.",
            L"Mauris et orci. [*** add more chars to prevent SSO ***]"
        };

        vector<wstring> v;
#ifdef _DEBUG
        constexpr int kTestIterationCount = 1000;
#else
        constexpr int kTestIterationCount = 200'000;
#endif
        for (int i = 0; i < kTestIterationCount; ++i) 
        {
            for (const auto & s : lorem) 
            {
#ifdef TEST_TINY_STRING_PERFORMANCE
                UNREFERENCED_PARAMETER(s);
                v.push_back(L"#" + to_wstring(i));
#else
                v.push_back(s + L" (#" + to_wstring(i) + L")");
#endif
            }

        }

        mt19937 prng(1980);
        shuffle(v.begin(), v.end(), prng);
        return v;
    }();

    const auto shuffledPtrs = [&]() -> vector<const wchar_t *> 
    {
        vector<const wchar_t *> v;
        for (const auto& s : shuffled) 
        {
            v.push_back(s.c_str());
        }
        return v;
    }();

    cout << "Test string array contains " << (shuffled.size() / 1000) << "K strings. \n\n";
    
    long long start = 0;
    long long finish = 0;

    vector<CStringW> atl1(shuffledPtrs.begin(), shuffledPtrs.end());
    vector<wstring>  stl1 = shuffled;
    vector<CStringW> atl2(shuffledPtrs.begin(), shuffledPtrs.end());
    vector<wstring>  stl2 = shuffled;
    vector<CStringW> atl3(shuffledPtrs.begin(), shuffledPtrs.end());
    vector<wstring>  stl3 = shuffled;

    //-------------------------------------------------------------------------

    cout << "=== String Sorting Test === \n";

    start = Counter();
    sort(atl1.begin(), atl1.end());
    finish = Counter();
    PrintTime(start, finish, "ATL1");

    start = Counter();
    sort(stl1.begin(), stl1.end());
    finish = Counter();
    PrintTime(start, finish, "STL1");

    start = Counter();
    sort(atl2.begin(), atl2.end());
    finish = Counter();
    PrintTime(start, finish, "ATL2");

    start = Counter();
    sort(stl2.begin(), stl2.end());
    finish = Counter();
    PrintTime(start, finish, "STL2");

    start = Counter();
    sort(atl3.begin(), atl3.end());
    finish = Counter();
    PrintTime(start, finish, "ATL3");

    start = Counter();
    sort(stl3.begin(), stl3.end());
    finish = Counter();
    PrintTime(start, finish, "STL3");

    cout << '\n';

    //-------------------------------------------------------------------------
   
    cout << "=== String Concatenation Test === \n";   

    start = Counter();
    StringConcat(atl1);
    finish = Counter();
    PrintTime(start, finish, "ATL1");

    start = Counter();
    StringConcat(stl1);
    finish = Counter();
    PrintTime(start, finish, "STL1");

    start = Counter();
    StringConcat(atl2);
    finish = Counter();
    PrintTime(start, finish, "ATL2");

    start = Counter();
    StringConcat(stl2);
    finish = Counter();
    PrintTime(start, finish, "STL2");

    start = Counter();
    StringConcat(atl3);
    finish = Counter();
    PrintTime(start, finish, "ATL3");

    start = Counter();
    StringConcat(stl3);
    finish = Counter();
    PrintTime(start, finish, "STL3");
}


int main()
{
    constexpr int kExitOk = 0;
    constexpr int kExitError = 1;

    try
    {
        RunTests();
    }
    catch (const CAtlException& e)
    {
        cerr << "\n*** ERROR: CAtlException thrown; HR=0x" << hex << e.m_hr << '\n';
        return kExitError;
    }
    catch (const exception& e)
    {
        cerr << "\n*** ERROR: " << e.what() << '\n';
        return kExitError;
    }

    return kExitOk;
}

///////////////////////////////////////////////////////////////////////////////
