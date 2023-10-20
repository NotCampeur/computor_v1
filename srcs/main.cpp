/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 10:56:28 by ldutriez          #+#    #+#             */
/*   Updated: 2023/10/20 16:01:34 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor_v1.hpp"
#include <cstring>

int main(int ac, char **av)
{
	if (ac == 1)
	{
		std::string equation;
		std::cout << "Running in interactive mode, type \"exit\" to quit.\n";
		while (std::cin.fail() == false)
		{
			std::getline(std::cin, equation);
			if (equation == "exit")
				return EXIT_SUCCESS;
			std::cout << "===============================\nEquation: " << equation << '\n';
			try
			{
				EquationSolver solver(equation);
				solver.print_solutions();
			}
			catch (std::exception &e) {
				std::cout << "Error: " << e.what() << std::endl;
			}
		}
	}
	else if (ac == 2 && strcmp(av[1], "--help") == 0)
	{
		std::cout << "Usage: " << av[0] << " [[--calculator] equation]\n\n";
		std::cout << "\tEquation must be in the form: aX^2 + bX + c = 0\n";
		std::cout << "\t\tExample: " << av[0] << " \"2X^2 + 3X + 4 = 0\"\n";
		std::cout << "\n\tIf no equation is given, the program will read from stdin.\n\n";
		std::cout << "\tIf the argument \"--calculator\" is given:\n" <<
					"\t\tthe program will just compute the input without unknowns." << std::endl;
		return EXIT_SUCCESS;
	}
	else if (ac == 3 && strcmp(av[1], "--calculator") == 0)
	{
		EquationSolver solver(av[2], true);
		return EXIT_SUCCESS;
	}
	else if (ac == 2)
	{
		std::cout << "===============================\nEquation: " << av[1] << '\n';
		try
		{
			EquationSolver solver(av[1]);
			solver.print_solutions();
		}
		catch (std::exception &e) {
			std::cout << "Error: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	std::cout << "Wrong use, you can type \"" << av[0] << " --help\"" << '\n';
	return EXIT_FAILURE;
}