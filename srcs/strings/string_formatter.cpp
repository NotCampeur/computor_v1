/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_formatter.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:17:04 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/28 06:22:24 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

void to_string_formatter(std::string & str)
{
	for (size_t i(str.size() - 1); i > 0; --i)
	{
		if (str[i] == '0')
			str.pop_back();
		else if (str[i] == '.')
		{
			str.pop_back();
			break;
		}
		else
			break;
	}
}

std::string get_digits(const std::string & str)
{
	std::string digits;
	bool comma(false);
	
	for (size_t i(0); i < str.size(); ++i)
	{
		if ((str[i] >= '0' && str[i] <= '9')
			|| (str[i] == '-' && i == 0)
			|| (str[i] == '.' && comma == false))
		{
			digits += str[i];
			if (str[i] == '.')
				comma = true;
		}
		else
			break;
	}
	return digits;
}