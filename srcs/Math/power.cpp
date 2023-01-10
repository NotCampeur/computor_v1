/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:48:12 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/10 01:55:14 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float power(float number, float power)
{
	float result = 1;

	if (power == 0)
		return 1;
	if (power < 0)
	{
		power = -power;
		number = 1 / number;
	}
	while (power > 1.0f)
	{
		--power;
		number *= number;
	}
	result = number;
	return result;
}