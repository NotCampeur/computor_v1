/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_functions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 22:49:37 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 13:09:16 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_MATH_FUNCTIONS_HPP
# define COMPUTOR_V1_MATH_FUNCTIONS_HPP

# include <gmpxx.h>

mpf_class power(mpf_class number, mpf_class power);
mpf_class power(mpf_class number, mpz_class power);

mpf_class square_root(mpf_class number, mpz_class precision = 5);
mpf_class nth_root(mpf_class number, mpz_class n, mpz_class precision = 5);

// Compute the greatest common divisor of a and b.
mpz_class gcd(mpz_class a, mpz_class b);
// Reduce the fraction to its simplest form.
void reduce_fraction(mpz_class &numerator, mpz_class &denominator);
// Convert a float decimal part only to a fraction.
void to_fraction(mpf_class decimal, mpz_class &numerator, mpz_class &denominator);

#endif