/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:48:12 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 18:34:19 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_functions.hpp"

mpf_class power(mpf_class number, mpf_class n_power)
{
	mpf_class result(0);
	mpz_class power_int(n_power);
	mpf_class power_decimal(n_power - power_int);
	mpz_class numerator(0);
	mpz_class denominator(0);

	if (n_power == 0)
		return 1;
	to_fraction(power_decimal, numerator, denominator);
	result = power(number, power_int) * nth_root(power(number, numerator), denominator);
	return result;
}

mpf_class power(mpf_class number, mpz_class power)
{
	mpf_class result(0);

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