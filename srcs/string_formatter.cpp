/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_formatter.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:17:04 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/10 18:21:46 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

// Format a string after the use of std::to_string(float)
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