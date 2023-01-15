/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:48:12 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/15 04:23:48 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_functions.hpp"

float power(float number, float n_power)
{
	float result(0);
	int power_int(n_power);
	float power_decimal(n_power - power_int);
	int numerator(0);
	int denominator(0);

	if (n_power == 0)
		return 1;
	to_fraction(power_decimal, numerator, denominator);
	// Hard cap to not overflow int size.
	while (numerator > 31)
	{
		--numerator;
		reduce_fraction(numerator, denominator);
	}
	result = power(number, power_int) * nth_root(power(number, numerator), denominator);
	return result;
}

float power(float number, int power)
{
	float result(0);

	if (power == 0)
		return 1;
	if (power < 0)
	{
		power = -power;
		number = 1 / number;
	}
	result = number;
	while (power > 1)
	{
		--power;
		result *= number;
	}
	return result;
}