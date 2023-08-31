#include "main.h"

/**
 * flip_bits - counts the number of bits needed to flip from n to m
 * @n: the first num
 * @m: the second num
 *
 * Return: the number of bits needed to flip
 */
unsigned int flip_bits(unsigned long int n, unsigned long int m)
{
	unsigned int count = 0;
	unsigned long int diff = n ^ m;

	while (diff)
	{
		count += diff & 1;
		diff >>= 1;
	}

	return (count);
}
