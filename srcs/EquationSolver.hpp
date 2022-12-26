/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2022/12/26 10:41:07 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_EQUATION_SOLVER_HPP
# define COMPUTOR_V1_EQUATION_SOLVER_HPP

# include <stack>
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
		: coefficient(p_coefficient), degree(p_degree), unknowns_degree(p_unknowns_degree)
		{}
		
		~EquationTerm() {}
		
		EquationTerm(EquationTerm const &obj)
		: coefficient(obj.coefficient), degree(obj.degree), unknowns_degree(obj.unknowns_degree)
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

std::ostream &operator<<(std::ostream &os, EquationTerm const &obj)
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
			// std::cout << "First expression:" << std::endl;
			// while (_first_expression_terms.empty() == false)
			// {
			// 	std::cout << _first_expression_terms.top() << std::endl;
			// 	_first_expression_terms.pop();
			// }
			// std::cout << "Second expression:" << std::endl;
			// while (_second_expression_terms.empty() == false)
			// {
			// 	std::cout << _second_expression_terms.top() << std::endl;
			// 	_second_expression_terms.pop();
			// }
		}
		
		~EquationSolver() {}
	
		int polynomial_degree() const
		{
			return _polynomial_degree;
		}

	private:
		void	_parse_formula(const std::string & formula)
		{
			bool	is_first_expression(true);
			std::stack<EquationTerm>	*_current_expression_terms(&_first_expression_terms);
			float temporarycoefficient(0.0f);
			int temporary_degree(1);
			bool is_constant(false);
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
						// std::cout << "Pushing constant term: " << temporarycoefficient << std::endl;
						_current_expression_terms->push(EquationTerm(temporarycoefficient, temporary_degree, 0));
						// std::cout << "Expression size: " << _current_expression_terms->size() << std::endl;
					}
					else
					{
						_current_expression_terms->top().unknowns_degree = temporary_degree;
						// std::cout << "Pushing unknown term: " << _current_expression_terms->top() << std::endl;
					}
					temporarycoefficient = 0.0f;
					temporary_degree = 1;
					is_constant = false;
					break;
				case '=':
					if (is_first_expression == false)
						throw std::invalid_argument("Too many expressions in the formula, only two are allowed");
					// std::cout << "Switching to second expression" << std::endl;
					is_first_expression = false;
					_current_expression_terms = &_second_expression_terms;
					if (*(it + 1) == ' ')
						++it;
					break;
				case '+':
				case '-':
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
						throw std::invalid_argument("Invalid character in formula, expected a digit got : " + *it);
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
					temporarycoefficient = std::stof(formula.substr(it - formula.begin()), &convert_index);
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
			{
				// std::cout << "Pushing constant term: " << temporarycoefficient << std::endl;
				_current_expression_terms->push(EquationTerm(temporarycoefficient, temporary_degree, 0));
				// std::cout << "Expression size: " << _current_expression_terms->size() << std::endl;
			}
			else
			{
				_current_expression_terms->top().unknowns_degree = temporary_degree;
			}
		}

		EquationSolver(EquationSolver const &obj);
		EquationSolver &operator=(EquationSolver const &obj);

		std::stack<EquationTerm>	_first_expression_terms;
		std::stack<EquationTerm>	_second_expression_terms;
		std::stack<EquationTerm>	_reduced_expression_terms;
		int							_polynomial_degree;
		float						_discriminant;
		float						_solutions[2];
};

#endif