/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 10:56:28 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/01 00:22:48 by ldutriez         ###   ########.fr       */
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
		int polynomial_degree = solver.polynomial_degree();
		std::cout << "Polynomial degree: " << polynomial_degree << std::endl;
		if (polynomial_degree > 2)
			throw std::invalid_argument("The polynomial degree is stricly greater than 2, I can't solve.");
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}