/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 10:56:28 by ldutriez          #+#    #+#             */
/*   Updated: 2022/12/26 08:57:54 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor_v1.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage: ./computor_v1 \"equation\"" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "===============================\nEquation: " << av[1] << std::endl;
	try
	{
		EquationSolver solver(av[1]);
		std::cout << "Reduced form: \n";
		std::cout << "Polynomial degree: " << solver.polynomial_degree() << std::endl;
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}