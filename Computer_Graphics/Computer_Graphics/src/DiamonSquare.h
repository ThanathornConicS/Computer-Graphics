#pragma once

#include "pch.h"

const int CHUNK_X = 64;
const int CHUNK_Z = 64;

static float FRandom(int range) 
{
	return (rand() % (range * 2)) - range;
}

void SquareStep(int arr[CHUNK_X][CHUNK_Z], int x, int z, int reach);
void DiamondStep(int arr[CHUNK_X][CHUNK_Z], int x, int z, int reach);

void DiamonSquare(int arr[CHUNK_X][CHUNK_Z], int size) 
{
	int half = size / 2;
	if (half < 1)
		return;

	// Square
	for (size_t z = half; z < CHUNK_Z; z += size) 
	{
		for (size_t x = half; x < CHUNK_X; x += size)
		{
			SquareStep(arr, x % CHUNK_X, z % CHUNK_Z, half);
		}
	}

	// Diamond
	int col = 0;
	for (size_t x = 0; x < CHUNK_X; x += half) 
	{
		col++;
		
		// Odd Column
		if (col % 2 != 0) 
		{
			for (size_t z = half; z < CHUNK_Z; z += half) 
			{
				DiamondStep(arr, x % CHUNK_X, z % CHUNK_Z, half);
			}
		}
		else 
		{
			for (size_t z = 0; z < CHUNK_Z; z += half)
			{
				DiamondStep(arr, x % CHUNK_X, z % CHUNK_Z, half);
			}
		}
	}
	DiamonSquare(arr, size / 2);
}

void SquareStep(int arr[CHUNK_X][CHUNK_Z], int x, int z, int reach) 
{
	int count = 0;
	float avg = 0.0f;

	if (x - reach >= 0 && z - reach >= 0) 
	{
		avg += arr[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < CHUNK_Z) 
	{
		avg += arr[x - reach][z + reach];
		count++;
	}
	if (x + reach < CHUNK_X && z - reach >= 0)
	{
		avg += arr[x + reach][z - reach];
		count++;
	}
	if (x + reach < CHUNK_X && z + reach < CHUNK_Z)
	{
		avg += arr[x - reach][z - reach];
		count++;
	}

	avg += FRandom(reach);
	avg /= count;

	arr[x][z] = round(avg);
}
void DiamondStep(int arr[CHUNK_X][CHUNK_Z], int x, int z, int reach) 
{
	int count = 0;
	float avg = 0.0f;

	if (x - reach >= 0)
	{
		avg += arr[x - reach][z];
		count++;
	}
	if (x + reach < CHUNK_X)
	{
		avg += arr[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += arr[x][z - reach];
		count++;
	}
	if (z + reach < CHUNK_Z)
	{
		avg += arr[x][z + reach];
		count++;
	}

	avg += FRandom(reach);
	avg /= count;

	arr[x][z] = static_cast<int>(avg);
}