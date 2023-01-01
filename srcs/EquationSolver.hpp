/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/01 16:42:35 by ldutriez         ###   ########.fr       */
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
		, _reduced_expression_terms()
		, _polynomial_degree(0), _discriminant(0.0f)
		, _solutions{0.0f, 0.0f}
		{
			_parse_formula(formula);
			_simplify_expressions();
			_reduce_expression();
			print_reduced_expression();
			std::cout << "Polynomial degree: " << _polynomial_degree << '\n';
			if (_polynomial_degree > 2)
				std::cout << "The polynomial degree is stricly greater than 2, I can't solve.\n";
			else
			{
				_compute_discriminant();
			}
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

	private:
		void	_parse_formula(const std::string & formula)
		{
			bool	is_first_expression(true);
			std::vector<EquationTerm>	*current_expression_terms(&_first_expression_terms);
			float temporary_coefficient(0.0f);
			int temporary_degree(1);
			bool is_constant(false);
			bool is_negative(false);
			size_t convert_index(0);

			for (std::string::const_iterator it = formula.begin();
				it != formula.end();
				++it)
			{
				switch (*it)
				{
				case ' ':
					if (is_constant == true)
					{
						temporary_coefficient = (is_negative == true) ? -temporary_coefficient : temporary_coefficient;
						current_expression_terms->push_back(EquationTerm(temporary_coefficient, temporary_degree, 0));
					}
					else
						current_expression_terms->back().unknowns_degree = temporary_degree;
					temporary_coefficient = 0.0f;
					temporary_degree = 1;
					is_constant = false;
					is_negative = false;
					break;
				case '=':
					if (is_first_expression == false)
						throw std::invalid_argument("Too many expressions in the formula, only two are allowed");
					is_first_expression = false;
					current_expression_terms = &_second_expression_terms;
					if (*(it + 1) == ' ')
						++it;
					break;
				case '-':
					is_negative = true;
					if (*(it + 1) == ' ')
						++it;
					break;
				case '+':
				case '*':
					if (*(it + 1) == ' ')
						++it;
					break;
				case '/':
					throw std::invalid_argument("Division is not supported");
				case 'X':
					is_constant = false;
					break;
				case '^':
					++it;
					if (it == formula.end())
						throw std::invalid_argument("Reached end of formula before finding degree");
					if (*it < '0' || *it > '9')
						throw std::invalid_argument(std::string("Invalid character in formula, expected a digit got : ") + *it);
					temporary_degree = *it - '0';
					if (temporary_degree > _polynomial_degree && is_constant == false)
						_polynomial_degree = temporary_degree;
					break;
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
					is_constant = true;
					temporary_coefficient = std::stof(formula.substr(it - formula.begin()), &convert_index);
					it += convert_index - 1;
					break;
				default:
					throw std::invalid_argument("Invalid character in formula at position: " + std::to_string(it - formula.begin()));
					break;
				}
			}
			if (is_first_expression == true)
				throw std::invalid_argument("Only one expression in the formula, two are required");
			if (is_constant == true)
				current_expression_terms->push_back(EquationTerm(temporary_coefficient, temporary_degree, 0));
			else
				current_expression_terms->back().unknowns_degree = temporary_degree;
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
					it = _first_expression_terms.begin();
					if (it == _first_expression_terms.end())
						break;
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
			for (std::vector<EquationTerm>::iterator it = _second_expression_terms.begin();
				it != _second_expression_terms.end();
				++it)
			{
				if (it->coefficient == 0.0f)
				{
					_second_expression_terms.erase(it);
					it = _second_expression_terms.begin();
					if (it == _second_expression_terms.end())
						break;
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
			for (std::vector<EquationTerm>::iterator it = _second_expression_terms.begin();
				it != _second_expression_terms.end();
				++it)
			{
				bool found(false);
				for (std::vector<EquationTerm>::iterator it2 = _reduced_expression_terms.begin();
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
					_reduced_expression_terms.push_back(EquationTerm(-it->coefficient, 0, it->unknowns_degree));
			}
		}

		void _compute_discriminant(void)
		{
			float a(0.0f), b(0.0f), c(0.0f);

			for (std::vector<EquationTerm>::iterator it = _reduced_expression_terms.begin();
				it != _reduced_expression_terms.end();
				++it)
			{
				if (it->unknowns_degree == 2)
					a = it->coefficient;
				else if (it->unknowns_degree == 1)
					b = it->coefficient;
				else if (it->unknowns_degree == 0)
					c = it->coefficient;
			}
			_discriminant = b * b - 4 * a * c;
			if (_discriminant < 0.0f)
				std::cout << "Discriminant is strictly negative, the two solutions are complex" << std::endl;
			else if (_discriminant == 0.0f)
				std::cout << "Discriminant is equal to zero, one solution exist" << std::endl;
			else
				std::cout << "Discriminant is strictly positive, two solutions exist" << std::endl;
		}

		EquationSolver(EquationSolver const &obj);
		EquationSolver &operator=(EquationSolver const &obj);

		std::vector<EquationTerm>	_first_expression_terms;
		std::vector<EquationTerm>	_second_expression_terms;
		std::vector<EquationTerm>	_reduced_expression_terms;
		int								_polynomial_degree;
		float							_discriminant;
		float							_solutions[2];
};

#endif