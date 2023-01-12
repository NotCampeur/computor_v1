/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_functions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 22:49:37 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/12 22:58:29 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_MATH_FUNCTIONS_HPP
# define COMPUTOR_V1_MATH_FUNCTIONS_HPP

float power(float number, float power);
float power(float number, int power);

float square_root(float number, int precision = 5);
float nth_root(float number, int n, int precision = 5);

#endif