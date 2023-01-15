/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fraction.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 20:35:08 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/15 04:26:40 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "math_functions.hpp"

int gcd(int a, int b)
{
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

void reduce_fraction(int &numerator, int &denominator)
{
	int gcd_value(gcd(numerator, denominator));
	numerator /= gcd_value;
	denominator /= gcd_value;
}

void to_string_formatter(std::string & str);

void to_fraction(float decimal, int &numerator, int &denominator)
{
	std::string decimal_str(std::to_string(decimal - (int)decimal).substr(2));

	to_string_formatter(decimal_str);
	numerator = std::stoi(decimal_str);
	denominator = power(10, static_cast<int>(decimal_str.size()));
	reduce_fraction(numerator, denominator);
}