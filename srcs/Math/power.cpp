/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:48:12 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/13 00:16:40 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_functions.hpp"
#include <iostream>

//! Still need to fix it, not working with decimal power.
//! Need to use nth_root.
float power(float number, float n_power)
{
	float result(0);
	int power_int(n_power);
	float power_decimal(n_power - power_int);
	int n_root(0);

	if (n_power == 0)
		return 1;
	if (power_int == 0)
		power_int = 1;
	if (power_decimal == 0.0f)
		power_decimal = 1.0f;
	n_root = power_int * power_decimal;
	std::cout << "power of " << number << " to " << n_power << " = " << "(" << 10 << "th_root(" << number << "))^" << n_root << '\n';
	result = power(nth_root(number, 10), n_root);
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