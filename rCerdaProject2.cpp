//Raul Cerda
//CPSC 474
//Project 2
//Requires input.txt to run
//input.txt has n at the beginning and the n x n matrix under it, each element separated by a space and n>10
//THIS PROGRAM RUNS WITH 5 PARALLEL PROCESSES RANKS 0-4
//Tested and working on TitanV using "mpirun -n 5 ./a.out" and input.txt

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{
	int** matrix;
	int n;
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0)
	{
		//Copy n
		fstream input;
		input.open("input.txt");
		input >> n;
		input.close();
	}
	
	//Set up matrix for each process of size n
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	matrix = new int * [n];
	for(int i = 0; i < n; i++)
		matrix[i] = new int[n]();
	
	if(rank == 0)
	{		
		fstream input;
		input.open("input.txt");
		int temp;
		input >> temp;
		//Copy matrix values from txt
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				input >> temp;
				matrix[i][j] = temp;
			}
		}
		input.close();
	}
	
	//Broadcast and copy values for each process' matrix
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			MPI_Bcast(&matrix[i][j], 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	//END OF SETUP -all processes should have copy of matrix and must do work
	
	if(rank == 0)
	{
		//z and o used simply to make send work (send needs references)
		int z = 0;
		int o = 1;
		int maxA;
		int maxB;
		int maxC;
		int maxD;
		int maxRow, maxCol;
		MPI_Recv(&maxA, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&maxB, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&maxC, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&maxD, 1, MPI_INT, 4, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(maxA >= maxB && maxA >= maxC && maxA >= maxD)
		{
			MPI_Send(&o, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 2, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 3, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 4, 10, MPI_COMM_WORLD);
			MPI_Recv(&maxRow, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&maxCol, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << maxA << endl;
		}
		else if(maxB >= maxA && maxB >= maxC && maxB >= maxD)
		{
			MPI_Send(&z, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
			MPI_Send(&o, 1, MPI_INT, 2, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 3, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 4, 10, MPI_COMM_WORLD);
			MPI_Recv(&maxRow, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&maxCol, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << maxB << endl;
		}
		else if(maxC >= maxB && maxC >= maxA && maxC >= maxD)
		{
			MPI_Send(&z, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 2, 10, MPI_COMM_WORLD);
			MPI_Send(&o, 1, MPI_INT, 3, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 4, 10, MPI_COMM_WORLD);
			MPI_Recv(&maxRow, 1, MPI_INT, 3, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&maxCol, 1, MPI_INT, 3, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << maxC << endl;
		}
		else if(maxD >= maxB && maxD >= maxC && maxD >= maxA)
		{
			MPI_Send(&z, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 2, 10, MPI_COMM_WORLD);
			MPI_Send(&z, 1, MPI_INT, 3, 10, MPI_COMM_WORLD);
			MPI_Send(&o, 1, MPI_INT, 4, 10, MPI_COMM_WORLD);
			MPI_Recv(&maxRow, 1, MPI_INT, 4, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&maxCol, 1, MPI_INT, 4, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << maxD << endl;
		}
		cout << matrix[maxRow][maxCol] << " " << matrix[maxRow][maxCol+1] << endl 
			<< matrix[maxRow+1][maxCol] << " " << matrix[maxRow+1][maxCol+1] << endl
			<< "The row/col of this submatrix is: " << maxRow << "," << maxCol << endl;
	}
	
	if(rank == 1)
	{
		int maxSum = 0;
		int maxRow = 0;
		int maxCol = 0;
		int amIMax = 0;
		for(int row = 0; row <= n/2; row++)
		{
			for(int col = 0; col <= n/2; col++)
			{
				int temp = matrix[row][col] + matrix[row][col+1] + matrix[row+1][col] + matrix[row+1][col+1];
				if(temp > maxSum)
				{
					maxSum = temp;
					maxRow = row;
					maxCol = col;
				}
			}
		}
		MPI_Send(&maxSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(&amIMax, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(amIMax != 0)
		{
			MPI_Send(&maxRow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&maxCol, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	}
	else if(rank == 2)
	{
		int maxSum = 0;
		int maxRow = 0;
		int maxCol = 0;
		int amIMax = 0;
		for(int row = n/2+1; row <= n-2; row++)
		{
			for(int col = 0; col <= n/2; col++)
			{
				int temp = matrix[row][col] + matrix[row][col+1] + matrix[row+1][col] + matrix[row+1][col+1];
				if(temp > maxSum)
				{
					maxSum = temp;
					maxRow = row;
					maxCol = col;
				}
			}
		}
		MPI_Send(&maxSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(&amIMax, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(amIMax != 0)
		{
			MPI_Send(&maxRow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&maxCol, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	}
	else if(rank == 3)
	{
		int maxSum = 0;
		int maxRow = 0;
		int maxCol = 0;
		int amIMax = 0;
		for(int row = 0; row <= n/2; row++)
		{
			for(int col = n/2+1; col <= n-2; col++)
			{
				int temp = matrix[row][col] + matrix[row][col+1] + matrix[row+1][col] + matrix[row+1][col+1];
				if(temp > maxSum)
				{
					maxSum = temp;
					maxRow = row;
					maxCol = col;
				}
			}
		}
		MPI_Send(&maxSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(&amIMax, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(amIMax != 0)
		{
			MPI_Send(&maxRow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&maxCol, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	}
	else if(rank == 4)
	{
		int maxSum = 0;
		int maxRow = 0;
		int maxCol = 0;
		int amIMax = 0;
		int temp = 0;
		for(int row = n/2+1; row <= n-2; row++)
		{
			for(int col = n/2+1; col <= n-2; col++)
			{
				temp = matrix[row][col] + matrix[row][col+1] + matrix[row+1][col] + matrix[row+1][col+1];
				if(temp > maxSum)
				{
					maxSum = temp;
					maxRow = row;
					maxCol = col;
				}
			}
		}
		MPI_Send(&maxSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(&amIMax, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(amIMax != 0)
		{
			MPI_Send(&maxRow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&maxCol, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}