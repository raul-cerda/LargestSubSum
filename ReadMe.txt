Raul Cerda
Project 2
CPSC 474

This project reads a n x n matrix, where n > 10, and finds the 2x2 submatrix with the greatest
sum in the larger matrix. The matrix is searched in parallel by 4 processes that search one fourth
of the large matrix, then report their respective maximums to the root process, which decides
the overall maximum.
Compiled on TitanV with mpic++
Tested on TitanV with mpirun -n 5 ./a.out

Input for program requires the first number to be n with
the n x n input matrix under it
"input.txt" file must be in same directory as a.out to run

Will print the maximum sum, followed by the 2x2 submatrix, and then the relative location
of the submatrix within the matrix