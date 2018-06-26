// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef SORTING_TESTS_H
#define SORTING_TESTS_H

#include "../../Error_Checks/ERROR_CHECKS.H"

#include "../SortingShared/SortingLib.h"


// DECLARATIONS

int main();
long TestNegatives();
long TestInts();
long TestAlpas();

template < class T, class U > struct ArraysStruct;
template<class T, class U> long RunTests(ArraysStruct<T,U> arrays[], std::size_t arraysSize);
template < class T, class U > long PrintArray(const T* array, const U arraySize);


// DEFINITIONS

template < class T, class U > struct ArraysStruct
{
     T* array;
     U size;
};


template<class T, class U> long RunTests(ArraysStruct<T, U> arrays[], std::size_t arraysSize)
{
     long nStatus = EXIT_SUCCESS;

     bool bSortCorrect = false;
     std::size_t nNumberOfSorts = 0;
     std::chrono::duration<double> duration;

     for (auto arraysIndex = 0; arraysIndex < arraysSize; arraysIndex++)
     {
          for (ESortingTypes eSortType = ESortingTypes::QuickSort; eSortType <= ESortingTypes::BubbleSort; eSortType++)
          {
               try
               {
                    nNumberOfSorts = 0;

                    //std::shared_ptr<T[]> tempArray(new T[arrays[arraysIndex].size]);
                    T* tempArray = new T[arrays[arraysIndex].size];

                    for (auto nIndex = 0; nIndex < arrays[arraysIndex].size; nIndex++)
                         tempArray[nIndex] = arrays[arraysIndex].array[nIndex];

                    std::wcout << L"SORTING TYPE: " << eSortType;

                    std::wcout << L" -- INPUT ARRAY: ";
                    //CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray.get(), arrays[arraysIndex].size)));
                    CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray, arrays[arraysIndex].size)));
                    

                    //CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::Sort(eSortType, tempArray.get(), arrays[arraysIndex].size, &nNumberOfSorts, &duration)));
                    CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::Sort(eSortType, tempArray, arrays[arraysIndex].size, &nNumberOfSorts, &duration)));

                    std::wcout << L"SORTED ARRAY: ";
                    //CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray.get(), arrays[arraysIndex].size)));
                    CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray, arrays[arraysIndex].size)));

                    auto nNumberOfMircosecondsToSort = std::chrono::duration_cast<std::chrono::microseconds>(duration);

                    std::wcout << L"SORTS: " << nNumberOfSorts << L" - " << L"TIME: " << nNumberOfMircosecondsToSort.count() << L" microseconds: " << std::endl;

                    //CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::VerifySort(tempArray.get(), arrays[arraysIndex].size, bSortCorrect)));
                    CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::VerifySort(tempArray, arrays[arraysIndex].size, bSortCorrect)));
                    std::wcout << L"RESULT: " << (bSortCorrect ? L"SUCCESSFUL" : L"FAILURE") << std::endl << std::endl;

                    CHECK_BOOL_TRUE_LOG_THROW(bSortCorrect);
               }
               catch (long& check_catch_lresult)
               {
                    nStatus += check_catch_lresult;
               }
          }
     }

     return nStatus;
}


template < class T, class U > long PrintArray(const T array[], const U arraySize)
{
     long nStatus = 0;

     if ((0 == array) || (0 == arraySize))
     {
          std::wcout << L"Invalid array" << std::endl;
          return EINVAL;
     }

     for (U iIndex = 0; iIndex < arraySize; iIndex++)
     {
          std::wcout << array[iIndex] << L" ";
     }
     std::wcout << std::endl;

     return nStatus;
}



#endif // SORTING_TESTS_H

