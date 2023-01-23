/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:46:45 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 18:48:35 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_functions.hpp"

mpf_class square_root(mpf_class number, mpz_class precision)
{
	return nth_root(number, 2, precision);
}

mpf_class nth_root(mpf_class number, mpz_class n, mpz_class precision)
{
    mpz_class start = 0, end = number.get_si();
    mpz_class middle;
 
    mpf_class answer;
 
	if (n == 0)
		return 0;
	if (n == 1)
		return number;
	if (n < 0)
		return 0;

    // for computing integral part
    // of square root of number
    while (start <= end) {
        middle = (start + end) / 2;
        if (power(middle, n) == number) {
            answer = middle;
            break;
        }
 
        // incrementing start if integral
        // part lies on right side of the middle
        if (power(middle, n) < number) {
            start = middle + 1;
            answer = middle;
        }
 
        // decrementing end if integral part
        // lies on the left side of the middle
        else {
            end = middle - 1;
        }
    }
 
    // For computing the fractional part
    // of square root upto given precision
    mpf_class increment = 0.1;
    for (int i = 0; i < precision; ++i) {
        for (int y(0); y < 10 && power(answer, n) <= number; ++y)
            answer += increment;
 
        // loop terminates when power(answer, n) > number
        answer -= increment;
        increment /= 10;
    }
    return answer;
}