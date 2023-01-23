/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fraction.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 20:35:08 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 18:39:55 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "math_functions.hpp"
#include <sstream>
#include <iostream>

mpz_class gcd(mpz_class a, mpz_class b)
{
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

void reduce_fraction(mpz_class &numerator, mpz_class &denominator)
{
	mpf_class gcd_value(gcd(numerator, denominator));
	numerator /= gcd_value;
	denominator /= gcd_value;
}

void to_string_formatter(std::string & str);

void to_fraction(mpf_class decimal, mpz_class &numerator, mpz_class &denominator)
{
		if (decimal == 0)
		{
			numerator = 0;
			denominator = 1;
			return ;
		}
		if (decimal < 0)
			decimal = -decimal;
		try
		{
			decimal -= decimal.get_si();
			for (int i = 0; i < 6 && decimal - decimal.get_si() != 0; i++)
				decimal *= 10;
			std::stringstream decimal_ss;
			std::string decimal_str;
			decimal_ss << decimal;
			decimal_ss >> decimal_str;
			to_string_formatter(decimal_str);
			numerator = mpz_class(decimal_str);
			denominator = power(10, mpz_class(decimal_str.size()));
			reduce_fraction(numerator, denominator);
		}
		catch(...)
		{
			std::cout << "Unable to compute a satisfying fractional form\n";
		}
}