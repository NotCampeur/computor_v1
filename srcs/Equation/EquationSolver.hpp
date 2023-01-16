/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/16 16:09:44 by ldutriez         ###   ########.fr       */
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

class EquationSolver
{
	public:
		EquationSolver(const std::string & formula)
		: _first_expression_terms(), _second_expression_terms()
		, _reduced_expression_terms(), _polynomial_degree(0)
		, _a(0.0f), _b(0.0f), _c(0.0f), _discriminant(0.0f)
		, _solutions{0.0f, 0.0f}
		{
			FormulaParser(formula).get_expression_terms(_first_expression_terms, _second_expression_terms);
			_simplify_expressions();
			_reduce_expression();
			_store_main_coefficients();
			std::cout << "Polynomial degree: " << _polynomial_degree << '\n';
			if (_polynomial_degree > 2)
				std::cout << "The polynomial degree is stricly greater than 2, I can't solve.\n";
			else if (_polynomial_degree == 2)
				_compute_discriminant();
			if (_polynomial_degree <= 2)
				_compute_solutions();
		}
		
		~EquationSolver() {}
	
		int polynomial_degree() const
		{
			return _polynomial_degree;
		}

		void print_solutions(void) const
		{
			if (_polynomial_degree > 2)
				std::cout << "Cannot find solutions.\n";
			else if (_polynomial_degree == 2)
			{
				if (_reduced_expression_terms.size() == 1
					&& _reduced_expression_terms[0].coefficient == 0.0f)
					std::cout << "All real numbers are solution.\n";
				else
				{
					std::cout << _solutions[0] << '\n';
					if (_discriminant != 0.0f)
						std::cout << _solutions[1] << '\n';
				}
			}
			else if (_polynomial_degree == 1)
			{
				if (_reduced_expression_terms.size() == 1
					&& _reduced_expression_terms[0].coefficient == 0.0f)
					std::cout << "All real numbers are solution.\n";
				else
					std::cout << _solutions[0] << '\n';
			}
			else if (_polynomial_degree == 0
				&& _reduced_expression_terms.size() == 1
				&& _reduced_expression_terms[0].coefficient == 0.0f)
				std::cout << "All real numbers are solution.\n";
			else
				std::cout << "There is no solution.\n";
		}

	private:

		// Simplify the expressions by removing terms with a coefficient of 0
		// ,by computing the power of constant terms and by grouping alike terms
		void _simplify_expressions(void)
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
			std::cout << "Simplified equation: ";
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
		void _print_terms(std::vector<EquationTerm> const &terms) const
		{
			for (std::vector<EquationTerm>::const_iterator it(terms.begin());
				it != terms.end();
				++it)
			{
				std::cout << *it;
				if (it + 1 != terms.end())
				{
					if ((it + 1)->coefficient >= 0.0f)
						std::cout << "+ ";
					else
						std::cout << "- ";
				}
			}
		}

		void _reduce_expression(void)
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
			std::cout << "Reduced equation: ";
			_print_terms(_reduced_expression_terms);
			std::cout << "= 0\n";
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
			std::cout << "Isolating coefficients: a = " << _a << ", b = " << _b << ", c = " << _c << '\n';
		}

		void _compute_discriminant(void)
		{
			if (_reduced_expression_terms.size() == 1
				&& _reduced_expression_terms[0].coefficient == 0)
				return;
			_discriminant = _b * _b - 4 * _a * _c;
			std::cout << "Calcul of discriminant for second degree equation: " << _discriminant << '\n';
			if (_discriminant < 0.0f)
				std::cout << "Discriminant is strictly negative, the two solutions are complex\n";
			else if (_discriminant == 0.0f)
				std::cout << "Discriminant is equal to zero, one solution exist\n";
			else
				std::cout << "Discriminant is strictly positive, two solutions exist\n";
		}

		void _compute_solutions(void)
		{
			if (_polynomial_degree == 1)
			{
				std::cout << "First degree equation solution is: " << -_c << '/' << _b << '\n';
				_solutions[0] = -_c / _b;
			}
			else if (_polynomial_degree == 2 && _discriminant >= 0.0f)
			{
				std::cout << "Applying the quadratic solution: (" << -_b << " +- sqrt(" << _discriminant << ")) / 2 * " << _a << '\n';
				_solutions[0] = (-_b + square_root(_discriminant)) / (2 * _a);
				if (_discriminant > 0.0f)
					_solutions[1] = (-_b - square_root(_discriminant)) / (2 * _a);
			}
			else if (_polynomial_degree == 2 && _discriminant < 0.0f)
			{
				std::cout << "Applying the quadratic solution: (" << -_b << " +- sqrt(" << -_discriminant << ")i) / 2 * " << _a << '\n';
				_solutions[0] = Complex(-_b / (2 * _a), square_root(-_discriminant) / (2 * _a));
				_solutions[1] = Complex(-_b / (2 * _a), -(square_root(-_discriminant) / (2 * _a)));
			}
		}

		EquationSolver(EquationSolver const &obj);
		EquationSolver &operator=(EquationSolver const &obj);

		std::vector<EquationTerm>	_first_expression_terms;
		std::vector<EquationTerm>	_second_expression_terms;
		std::vector<EquationTerm>	_reduced_expression_terms;
		int							_polynomial_degree;
		float						_a;
		float						_b;
		float						_c;
		float						_discriminant;
		Complex						_solutions[2];
};

#endif