/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2024/01/22 15:31:00 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_EQUATION_SOLVER_HPP
# define COMPUTOR_V1_EQUATION_SOLVER_HPP

# include <vector>
# include <string>
# include <exception>
# include "EquationTerm.hpp"
# include "FormulaParser.hpp"
# include "Complex.hpp"
# include "math_functions.hpp"
# include <gmpxx.h>

class EquationSolver
{
public:
	EquationSolver(const std::string& formula, bool p_verbose = false) :
		_verbose(p_verbose),
		_first_expression_terms(), _second_expression_terms(),
		_reduced_expression_terms(), _polynomial_degree(0),
		_a(0.0f), _b(0.0f), _c(0.0f), _discriminant(0.0f),
		_solutions{ Complex(0.0f), Complex(0.0f) }
	{
		FormulaParser(formula).get_expression_terms(_first_expression_terms, _second_expression_terms);
		_simplify_expressions();
		_reduce_expression();
		if (_verbose == true)
			std::cout << "Polynomial degree: " << _polynomial_degree << "\n\n";
		_store_main_coefficients();
		if (_polynomial_degree > 2 && _verbose == true)
			std::cout << "The polynomial degree is stricly greater than 2, I can't solve.\n";
		else if (_polynomial_degree == 2)
			_compute_discriminant();
		if (_polynomial_degree <= 2)
			_compute_solutions();
	}

	~EquationSolver() {}

	mpf_class polynomial_degree() const
	{
		return _polynomial_degree;
	}

	void print_solutions(void) const
	{
		if (_polynomial_degree > 2 && _verbose == true)
			std::cout << " => Cannot find solutions.\n";
		else if (_polynomial_degree == 2)
		{
			if (_reduced_expression_terms.size() == 1
				&& _reduced_expression_terms[0].coefficient == 0.0f)
			{
				if (_verbose == true)
					std::cout << " => All complex numbers are solution.\n";
				else
					std::cout << "C" << std::endl;
			}
			else
			{
				if (_verbose == true)
					std::cout << " => X = ";
				std::cout << _solutions[0] << '\n';
				_print_fractional_solution(_solutions[0]);
				if (_discriminant != 0.0f)
				{
					if (_verbose == true)
						std::cout << " => X = ";
					std::cout << _solutions[1] << "\n";
					_print_fractional_solution(_solutions[1]);
				}
			}
		}
		else if (_polynomial_degree == 1)
		{
			if (_reduced_expression_terms.size() == 1
				&& _reduced_expression_terms[0].coefficient == 0.0f)
			{
				if (_verbose == true)
					std::cout << " => All complex numbers are solution.\n";
				else
				{
					std::cout << "C" << std::endl;
				}
			}
			else
			{
				if (_verbose == true)
					std::cout << " => X = ";
				std::cout << _solutions[0] << "\n";
				_print_fractional_solution(_solutions[0]);
			}
		}
		else if (_polynomial_degree == 0
			&& _reduced_expression_terms.size() == 1
			&& _reduced_expression_terms[0].coefficient == 0.0f)
		{
			if (_verbose == true)
				std::cout << " => All complex numbers are solution.\n";
			else
			{
				std::cout << "C" << std::endl;
			}
		}
		else if (_verbose == true)
			std::cout << " => There is no solution.\n";
	}

private:

	void _print_fractional_solution(const Complex& solution) const
	{
		mpz_class numerator;
		mpz_class denominator;
		char sign;

		if (_verbose == false)
			return;

		std::cout << "      = ";
		sign = (solution.real < 0) ? '-' : '+';
		to_fraction(solution.real, numerator, denominator);
		if (numerator != 0)
		{
			if (solution.real.get_si() != 0)
				std::cout << solution.real.get_si() << ' ' << sign << ' '
				<< numerator << '/' << denominator;
			else
				std::cout << sign << ' ' << numerator << '/' << denominator;
		}
		sign = (solution.imaginary < 0) ? '-' : '+';
		to_fraction(solution.imaginary, numerator, denominator);
		if (solution.imaginary != 0.0f)
		{
			if (solution.real.get_si() != 0)
				std::cout << " " << sign << solution.real.get_si() << "i " << sign << ' '
				<< numerator << '/' << denominator << "i";
			else
				std::cout << " " << sign << ' ' << numerator << '/' << denominator << "i";
		}
		std::cout << "\n\n";
	}

	// Simplify the expressions by removing terms with a coefficient of 0
	// ,by computing the power of constant terms and by grouping alike terms
	void _simplify_expressions()
	{
		for (std::vector<EquationTerm>::iterator it(_first_expression_terms.begin());
			it != _first_expression_terms.end();
			++it)
		{
			if (it->coefficient == 0.0f)
			{
				_first_expression_terms.erase(it);
				--it;
			}
		}
		for (std::vector<EquationTerm>::iterator it(_first_expression_terms.begin());
			it != _first_expression_terms.end();
			++it)
		{
			for (std::vector<EquationTerm>::iterator it2(it + 1);
				it2 != _first_expression_terms.end();
				++it2)
			{
				if (it->unknowns_degree == it2->unknowns_degree)
				{
					it->coefficient += it2->coefficient;
					_first_expression_terms.erase(it2);
					--it;
					break;
				}
			}
		}
		for (std::vector<EquationTerm>::iterator it(_second_expression_terms.begin());
			it != _second_expression_terms.end();
			++it)
		{
			if (it->coefficient == 0.0f)
			{
				_second_expression_terms.erase(it);
				--it;
			}
		}
		if (_verbose == false)
			return;
		std::cout << " | Simplified equation: ";
		_print_terms(_first_expression_terms);
		std::cout << "= ";
		if (_second_expression_terms.size() == 0)
			std::cout << "0\n";
		else
		{
			_print_terms(_second_expression_terms);
			std::cout << '\n';
		}
	}

	// Print the terms of an expression using EquationTerm::operator<<
	void _print_terms(std::vector<EquationTerm> const& terms) const
	{
		for (std::vector<EquationTerm>::const_iterator it(terms.begin());
			it != terms.end();
			++it)
		{
			if (it->coefficient >= 0.0f || it == terms.begin())
				std::cout << *it;
			if (it + 1 != terms.end())
			{
				if ((it + 1)->coefficient >= 0.0f)
					std::cout << "+ ";
				else // Avoid writing - -coefficient
					std::cout << "- " << -(*(it + 1));
			}
		}
	}

	void _reduce_expression()
	{
		_reduced_expression_terms = _first_expression_terms;
		for (std::vector<EquationTerm>::iterator it(_second_expression_terms.begin());
			it != _second_expression_terms.end();
			++it)
		{
			bool found(false);
			for (std::vector<EquationTerm>::iterator it2(_reduced_expression_terms.begin());
				it2 != _reduced_expression_terms.end();
				++it2)
			{
				if (it->unknowns_degree == it2->unknowns_degree)
				{
					it2->coefficient -= it->coefficient;
					if (it2->coefficient >= -0.00001f && it2->coefficient <= 0.00001f)
						_reduced_expression_terms.erase(it2);
					found = true;
					break;
				}
			}
			if (found == false)
				_reduced_expression_terms.push_back(EquationTerm(-it->coefficient, it->unknowns_degree));
		}
		bool bad_unknown_exponent = false;
		for (std::vector<EquationTerm>::iterator it(_reduced_expression_terms.begin());
			it != _reduced_expression_terms.end();
			++it)
		{
			if (it->unknowns_degree < 0)
				bad_unknown_exponent = true;
			if (it->unknowns_degree > _polynomial_degree)
				_polynomial_degree = it->unknowns_degree;
		}
		if (_reduced_expression_terms.size() == 0)
			_reduced_expression_terms.push_back(EquationTerm(0.0f, 0));
		if (_verbose == true)
		{
			std::cout << " | Reduced equation: ";
			_print_terms(_reduced_expression_terms);
			std::cout << "= 0\n\n";
		}
		if (bad_unknown_exponent == true)
			throw std::invalid_argument("Reduced equation is not in form ax^2 + bx + c = 0");
	}

	void _store_main_coefficients(void)
	{
		for (std::vector<EquationTerm>::iterator it = _reduced_expression_terms.begin();
			it != _reduced_expression_terms.end();
			++it)
		{
			if (it->unknowns_degree == 2)
				_a = it->coefficient;
			else if (it->unknowns_degree == 1)
				_b = it->coefficient;
			else if (it->unknowns_degree == 0)
				_c = it->coefficient;
		}
		if (_verbose == true)
			std::cout << "Isolating coefficients: a = " << _a << ", b = " << _b << ", c = " << _c << "\n\n";
	}

	void _compute_discriminant(void)
	{
		if (_reduced_expression_terms.size() == 1
			&& _reduced_expression_terms[0].coefficient == 0)
			return;
		_discriminant = _b * _b - 4 * _a * _c;
		if (_verbose == false)
			return;
		std::cout << "Calcul of discriminant for second degree equation: [b^2 - 4ac] = " << _discriminant << '\n';
		if (_discriminant < 0.0f)
			std::cout << " | Discriminant is strictly negative, the two solutions are complex\n\n";
		else if (_discriminant == 0.0f)
			std::cout << " | Discriminant is equal to zero, one solution exist\n\n";
		else
			std::cout << " | Discriminant is strictly positive, two solutions exist\n\n";
	}

	void _compute_solutions(void)
	{
		mpf_class discriminant_root;
		mpf_class denominator = 2 * _a;

		if (_polynomial_degree == 1)
		{
			if (_verbose == true)
				std::cout << "First degree equation solution is:\n\n"
				<< "   -c\n"
				<< " = ___\n"
				<< "    b\n\n"
				<< " = " << -_c << " / " << _b << "\n\n";
			_solutions[0] = Complex(-_c / _b);
		}
		else if (_polynomial_degree == 2 && _discriminant >= 0.0f)
		{
			discriminant_root = square_root(_discriminant);
			if (_verbose == true)
				std::cout << "Applying the quadratic solution:\n\n"
				<< "   (-b +- sqrt(b^2 - 4ac))\n"
				<< " = _______________________\n"
				<< "             2a\n\n"
				<< " = (" << -_b << " +- " << discriminant_root << ") / " << denominator << "\n\n";
			_solutions[0] = Complex((-_b + discriminant_root) / denominator);
			if (_discriminant > 0.0f)
				_solutions[1] = Complex((-_b - discriminant_root) / denominator);
		}
		else if (_polynomial_degree == 2 && _discriminant < 0.0f)
		{
			discriminant_root = square_root(-_discriminant);
			if (_verbose == true)
				std::cout << "Applying the quadratic solution for complex:\n\n"
				<< "   (-b +- sqrt(b^2 - 4ac))   -b    sqrt(-1) * sqrt(-(b^2 - 4ac))\n"
				<< " = _______________________ = __ +- _____________________________\n"
				<< "             2a              2a                 2a\n\n"
				<< " = " << -_b << " / " << denominator << " +- " << discriminant_root << "i / " << denominator << "\n\n";
			_solutions[0] = Complex(-_b / denominator, discriminant_root / denominator);
			_solutions[1] = Complex(-_b / denominator, -(discriminant_root / denominator));
		}
	}

	EquationSolver(EquationSolver const& obj);
	EquationSolver& operator=(EquationSolver const& obj);

	bool _verbose;

	std::vector<EquationTerm>	_first_expression_terms;
	std::vector<EquationTerm>	_second_expression_terms;
	std::vector<EquationTerm>	_reduced_expression_terms;
	mpz_class					_polynomial_degree;
	mpf_class					_a;
	mpf_class					_b;
	mpf_class					_c;
	mpf_class					_discriminant;
	Complex						_solutions[2];
};

#endif