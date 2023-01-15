/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_functions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 22:49:37 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/15 04:21:53 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_MATH_FUNCTIONS_HPP
# define COMPUTOR_V1_MATH_FUNCTIONS_HPP

float power(float number, float power);
float power(float number, int power);

float square_root(float number, int precision = 5);
float nth_root(float number, int n, int precision = 5);

// Compute the greatest common divisor of a and b.
int gcd(int a, int b);
// Reduce the fraction to its simplest form.
void reduce_fraction(int &numerator, int &denominator);
// Convert a float decimal part only to a fraction.
void to_fraction(float decimal, int &numerator, int &denominator);

#endif