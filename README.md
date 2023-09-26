# reductionVsCriticalOMP

 Comparing reduce vs. critical section in OpenMP
 Create a vector of integers with 1,000,000 entries
 Fill with random integer positive values
 Find the largest element and its position in the vector
 Use two different approaches:
 1. Perform a reduction and then look for the entry
 2. Use a critical section to update the max value and its entry
 Time the different parts of the code
