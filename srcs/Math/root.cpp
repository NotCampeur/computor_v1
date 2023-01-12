/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:46:45 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/12 23:11:07 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_functions.hpp"

float square_root(float number, int precision)
{
    int start = 0, end = number;
    int middle;
 
    float answer;
 
    // for computing integral part
    // of square root of number
    while (start <= end) {
        middle = (start + end) / 2;
        if (middle * middle == number) {
            answer = middle;
            break;
        }
 
        // incrementing start if integral
        // part lies on right side of the middle
        if (middle * middle < number) {
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
    float increment = 0.1;
    for (int i = 0; i < precision; ++i) {
        while (answer * answer <= number) {
            answer += increment;
        }
 
        // loop terminates when answer * answer > number
        answer -= increment;
        increment /= 10;
    }
    return answer;
}

float nth_root(float number, int n, int precision)
{
    int start = 0, end = number;
    int middle;
 
    float answer;
 
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
    float increment = 0.1;
    for (int i = 0; i < precision; ++i) {
        while (power(answer, n) <= number) {
            answer += increment;
        }
 
        // loop terminates when power(answer, n) > number
        answer -= increment;
        increment /= 10;
    }
    return answer;
}