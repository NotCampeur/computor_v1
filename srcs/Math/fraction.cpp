/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fraction.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 20:35:08 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/28 06:07:07 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "math_functions.hpp"
#include "strings_functions.hpp"
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

void to_fraction(mpf_class decimal, mpz_class &numerator, mpz_class &denominator)
{
	decimal -= decimal.get_si();
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
		mp_exp_t exp(0);
		std::string decimal_str = decimal.get_str(exp);
		exp = (exp < 0) ? -exp : 0;
		to_string_formatter(decimal_str);
		numerator = mpz_class(decimal_str);
		denominator = power(10, mpz_class(decimal_str.size() + mpz_class(exp)));
		reduce_fraction(numerator, denominator);
	}
	catch(...)
	{
		std::cout << "Unable to compute a satisfying fractional form\n";
	}
}