/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:48:12 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/12 22:52:58 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! Still need to fix it, not working with decimal power.
//! Need to use nth_root.
float power(float number, float power)
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
	while (power > 1.0f)
	{
		--power;
		result *= number;
	}
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