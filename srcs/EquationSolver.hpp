/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/02 22:07:12 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_EQUATION_SOLVER_HPP
# define COMPUTOR_V1_EQUATION_SOLVER_HPP

# include <cmath>
# include <vector>
# include <string>
# include <exception>
# include <stdexcept>

struct EquationTerm
{
		float	coefficient;
		int		degree;
		int		unknowns_degree;

		EquationTerm()
		: coefficient(0.0f), degree(1), unknowns_degree(0)
		{}
		
		EquationTerm(float p_coefficient, int p_degree, int p_unknowns_degree)
		: coefficient(p_coefficient), degree(p_degree)
		, unknowns_degree(p_unknowns_degree)
		{}
		
		~EquationTerm() {}
		
		EquationTerm(EquationTerm const &obj)
		: coefficient(obj.coefficient), degree(obj.degree)
		, unknowns_degree(obj.unknowns_degree)
		{}
		
		EquationTerm &operator=(EquationTerm const &obj)
		{
			if (this != &obj)
			{
				coefficient = obj.coefficient;
				degree = obj.degree;
				unknowns_degree = obj.unknowns_degree;
			}
			return (*this);
		}
};

std::ostream &operator<<(std::ostream &os, const EquationTerm &obj)
{
	os << obj.coefficient;
	if (obj.degree != 1)
		os << "^" << obj.degree;
	if (obj.unknowns_degree != 0)
	{
		os << "X";
		if (obj.unknowns_degree != 1)
			os << "^" << obj.unknowns_degree;
	}
	os << " ";
	return (os);
}

class EquationSolver
{
	public:
		EquationSolver(const std::string & formula)
		: _first_expression_terms(), _second_expression_terms()
		, _reduced_expression_terms(), _polynomial_degree(0)
		, _a(0.0f), _b(0.0f), _c(0.0f), _discriminant(0.0f)
		, _solutions{0.0f, 0.0f}
		{
			std::string trimmed_formula(formula);
			for (size_t i(0); i != trimmed_formula.size(); ++i)
			{
				if (trimmed_formula[i] != ' ')
				{
					trimmed_formula.erase(0, i);
					break;
				}
			}
			_parse_formula(trimmed_formula);
			_simplify_expressions();
			_reduce_expression();
			_simplify_reduced_form();
			_store_main_coefficients();
			print_reduced_expression();
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

		void print_reduced_expression(void) const
		{
			std::cout << "Reduced form: ";
			for (auto it = _reduced_expression_terms.begin(); it != _reduced_expression_terms.end(); ++it)
			{
				std::cout << *it;
				if (it + 1 != _reduced_expression_terms.end())
					std::cout << "+ ";
			}
			std::cout << "= 0\n";
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
					if (_discriminant > 0.0f)
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
		void _parse_formula(const std::string & formula)
		{
			std::vector<EquationTerm>	*current_expression_terms(&_first_expression_terms);
			float current_coefficient(0.0f);
			float temporary_exponant(0.0f);
			int current_constant_degree(1);
			int current_unknown_degree(0);
			bool is_constant(true);
			bool is_negative(false);
			bool is_exponant(false);
			size_t convert_offset(0);

			for (size_t i(0); i != formula.size() + 1; ++i)
			{
				switch (formula[i])
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						if (is_exponant == true)
						{
							temporary_exponant = std::stof(formula.substr(i), &convert_offset);
							if (is_constant == true)
								current_constant_degree = temporary_exponant;
							else
								current_unknown_degree = temporary_exponant;
							if (temporary_exponant != current_constant_degree
								&& temporary_exponant != current_unknown_degree)
								throw std::invalid_argument("Invalid exponant");
						}
						else
						{
							current_coefficient = std::stof(formula.substr(i), &convert_offset);
							current_coefficient = is_negative ? -current_coefficient : current_coefficient;
						}
						i += convert_offset - 1;
						break;
					case ' ':
						break;
					case '\0':
					case '-':
						is_negative = true;
						if (i != 0)
						{
							current_expression_terms->push_back(EquationTerm(current_coefficient
																		, current_constant_degree
																		, current_unknown_degree));
							current_coefficient = 0.0f;
							current_constant_degree = 1;
							current_unknown_degree = 0;
							is_constant = true;
							is_exponant = false;
						}
						break;
					case '+':
						current_expression_terms->push_back(EquationTerm(current_coefficient
																		, current_constant_degree
																		, current_unknown_degree));
						current_coefficient = 0.0f;
						current_constant_degree = 1;
						current_unknown_degree = 0;
						is_negative = false;
						is_constant = true;
						is_exponant = false;
						break;
					case '=':
						current_expression_terms->push_back(EquationTerm(current_coefficient
																		, current_constant_degree
																		, current_unknown_degree));
						current_coefficient = 0.0f;
						current_constant_degree = 1;
						current_unknown_degree = 0;
						is_negative = false;
						is_constant = true;
						is_exponant = false;
						current_expression_terms = &_second_expression_terms;
						break;
					case '*':
						break;
					case 'X':
						is_constant = false;
						current_unknown_degree = 1;
						break;
					case '^':
						is_exponant = true;
						break;
					default:
						throw std::invalid_argument(std::string("Unexpected character found in the formula: ") + formula[i]);
				}
			}
			if (isdigit(formula.back()) == false && formula.back() != 'X')
				throw std::invalid_argument("Formula looks incomplete");
			if (current_expression_terms == &_first_expression_terms)
				_second_expression_terms.push_back(EquationTerm(0.0f, 1, 0));
		}

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
					continue;
				}
				else if (it->degree == 0)
					it->coefficient = 1.0f;
				else if (it->degree > 1)
					it->coefficient = std::pow(it->coefficient, it->degree);
				it->degree = 1;
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
					continue;
				}
				else if (it->degree == 0)
					it->coefficient = 1.0f;
				else if (it->degree > 1)
					it->coefficient = std::pow(it->coefficient, it->degree);
				it->degree = 1;
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
						found = true;
						break;
					}
				}
				if (found == false)
					_reduced_expression_terms.push_back(EquationTerm(-it->coefficient, 1, it->unknowns_degree));
			}
		}

		void _simplify_reduced_form(void)
		{
			for (std::vector<EquationTerm>::iterator it(_reduced_expression_terms.begin());
				it != _reduced_expression_terms.end();
				++it)
			{
				if (it->coefficient == 0.0f)
				{
					_reduced_expression_terms.erase(it);
					--it;
				}
				else if (it->unknowns_degree > _polynomial_degree)
					_polynomial_degree = it->unknowns_degree;
			}
			if (_reduced_expression_terms.size() == 0)
				_reduced_expression_terms.push_back(EquationTerm(0.0f, 1, 0));
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
		}

		void _compute_discriminant(void)
		{
			if (_reduced_expression_terms.size() == 1
				&& _reduced_expression_terms[0].coefficient == 0)
				return;
			_discriminant = _b * _b - 4 * _a * _c;
			if (_discriminant < 0.0f)
				std::cout << "Discriminant is strictly negative, the two solutions are complex" << std::endl;
			else if (_discriminant == 0.0f)
				std::cout << "Discriminant is equal to zero, one solution exist" << std::endl;
			else
				std::cout << "Discriminant is strictly positive, two solutions exist" << std::endl;
		}

		void _compute_solutions(void)
		{
			if (_polynomial_degree == 1)
				_solutions[0] = -_c / _b;
			else if (_polynomial_degree == 2 && _discriminant >= 0.0f)
			{
				_solutions[0] = (-_b + std::sqrt(_discriminant)) / (2 * _a);
				if (_discriminant > 0.0f)
					_solutions[1] = (-_b - std::sqrt(_discriminant)) / (2 * _a);
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
		float						_solutions[2];
};

#endif