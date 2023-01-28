/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 06:04:49 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/28 06:16:07 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_STRINGS_FUNCTIONS_HPP
# define COMPUTOR_V1_STRINGS_FUNCTIONS_HPP

// Format a string after the use of std::to_string(float)
void to_string_formatter(std::string & str);

// Return a substring containing only digits
// If the string is not a number, return an empty string
// Stop at the first non-digit character
// '-' and '.' are considered digits
std::string get_digits(const std::string & str);

#endif