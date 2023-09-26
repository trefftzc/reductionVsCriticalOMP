//
// Comparing reduce vs. critical section in OpenMP
// Create a vector of integers with 1,000,000 entries
// Fill with random integer positive values
// Find the largest element and its position in the vector
// Use two different approaches:
// 1. Perform a reduction and then look for the entry
// 2. Use a critical section to update the max value and its entry
// Time the different parts of the code
//
#include <omp.h>
#include <cstdlib>
#include <float.h>
#include <algorithm>
#include <iostream>
#include <vector>

const int SIZE = 1000000;
int main(int argc,char *argv[])
{
  std::vector < int > array(SIZE);

  double startGenerating,endGenerating;
  startGenerating = omp_get_wtime();
#pragma omp parallel for
  for(int i = 0;i < SIZE;i++) {
    array[i] = rand();
  }
  endGenerating = omp_get_wtime();

  // Now compare two approaches to finding
  // the largest element in the array
  // 1. Perform a reduction and then find the corresponding element
  // 2. Use a critical section
  double startReduction,endReduction;
  startReduction = omp_get_wtime();
  int maxValue = 0;
#pragma omp parallel for reduction(max:maxValue)
  for(int i = 0;i < SIZE;i++) {
    maxValue = std::max(maxValue,array[i]);
  }
  endReduction = omp_get_wtime();
  double startFindMaxEntry,endFindMaxEntry;
  startFindMaxEntry = omp_get_wtime();
  int maxIndex = -1;
  for(int i = 0;i < SIZE;i++) {
    if (array[i] == maxValue) {
      maxIndex = i;
      break;
    }
  }
  endFindMaxEntry = omp_get_wtime();
  std::cout << "Max value: " << maxValue
       << " at position: " << maxIndex << std::endl;
  double startCriticalSection,endCriticalSection;
  startCriticalSection = omp_get_wtime();
  maxIndex = -1;
  maxValue = 0;
#pragma omp parallel for shared(maxValue,maxIndex)
  for(int i = 0;i < SIZE;i++) {
    #pragma omp critical
    if (array[i] > maxValue)
      {
      maxValue = array[i];
      maxIndex = i;
      }
  }
  endCriticalSection = omp_get_wtime();
  std::cout << "Max value: " << maxValue
	    << " at position: " << maxIndex << std::endl;
  std::cout << "Times: " << std::endl;
  std::cout << "Generation: " << (endGenerating - startGenerating)
	    << std::endl;
  std::cout << "Reduction: " << (endReduction - startReduction)
	    << std::endl;
  std::cout << "FindMaxEntry: " << (endFindMaxEntry - startFindMaxEntry)
	    << std::endl;
  std::cout << "Reduction + FindMaxEntry: " <<
    (endReduction - startReduction)+(endFindMaxEntry - startFindMaxEntry)
	    <<std::endl;
  std::cout << "Critical Section: " << (endCriticalSection - startCriticalSection)
	    << std::endl;
}
  
