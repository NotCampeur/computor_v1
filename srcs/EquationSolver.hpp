/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationSolver.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 11:21:46 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/01 01:55:20 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_EQUATION_SOLVER_HPP
# define COMPUTOR_V1_EQUATION_SOLVER_HPP

# include <stack>
# include <vector>
# include <string>
# include <exception>
# include <stdexcept>

struct EquationComponent
{
	bool	is_term;

	EquationComponent()
	: is_term(false)
	{}

	EquationComponent(bool p_is_term)
	: is_term(p_is_term)
	{}

	virtual ~EquationComponent() {}

	virtual void print(void) const
	{
		if (is_term)
			std::cout << "Term";
		else
			std::cout << "Operator";
	}
};

struct EquationTerm : public EquationComponent
{
		float	coefficient;
		int		degree;
		int		unknowns_degree;

		EquationTerm()
		: EquationComponent(true), coefficient(0.0f), degree(1), unknowns_degree(0)
		{}
		
		EquationTerm(float p_coefficient, int p_degree, int p_unknowns_degree)
		: EquationComponent(true), coefficient(p_coefficient), degree(p_degree)
		, unknowns_degree(p_unknowns_degree)
		{}
		
		~EquationTerm() {}
		
		EquationTerm(EquationTerm const &obj)
		: EquationComponent(true), coefficient(obj.coefficient), degree(obj.degree)
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

		void print(void) const
		{
			std::cout << coefficient;
			if (degree != 1)
				std::cout << "^" << degree;
			if (unknowns_degree != 0)
			{
				std::cout << "X";
				if (unknowns_degree != 1)
					std::cout << "^" << unknowns_degree;
			}
			std::cout << " ";
		}
};

struct EquationOperator : public EquationComponent
{
		char operator_type;

		EquationOperator()
		: EquationComponent(false), operator_type('+')
		{}

		EquationOperator(char p_operator)
		: EquationComponent(false), operator_type(p_operator)
		{}

		~EquationOperator() {}

		EquationOperator(EquationOperator const &obj)
		: EquationComponent(false), operator_type(obj.operator_type)
		{}

		EquationOperator &operator=(EquationOperator const &obj)
		{
			if (this != &obj)
				operator_type = obj.operator_type;
			return (*this);
		}

		void print(void) const
		{
			std::cout << operator_type << " ";
		}
};

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
			std::cout << "Equation is : " << std::endl;
			for (auto it = _first_expression_terms.begin(); it != _first_expression_terms.end(); ++it)
				(*it)->print();
			std::cout << "= ";
			for (auto it = _second_expression_terms.begin(); it != _second_expression_terms.end(); ++it)
				(*it)->print();
			std::cout << std::endl;
			// _reduce_expression();
		}
		
		~EquationSolver()
		{
			for (auto it = _first_expression_terms.begin(); it != _first_expression_terms.end(); ++it)
				delete *it;
			for (auto it = _second_expression_terms.begin(); it != _second_expression_terms.end(); ++it)
				delete *it;
			for (auto it = _reduced_expression_terms.begin(); it != _reduced_expression_terms.end(); ++it)
				delete *it;
		}
	
		int polynomial_degree() const
		{
			return _polynomial_degree;
		}

	private:
		void	_parse_formula(const std::string & formula)
		{
			bool	is_first_expression(true);
			std::vector<EquationComponent *>	*current_expression_terms(&_first_expression_terms);
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
						current_expression_terms->push_back(new EquationTerm(temporarycoefficient, temporary_degree, 0));
					else
					{
						try
						{
							EquationTerm &term = dynamic_cast<EquationTerm &>(*current_expression_terms->back());
							term.unknowns_degree = temporary_degree;
						}
						catch(const std::exception& e)
						{
						}
					}
					temporarycoefficient = 0.0f;
					temporary_degree = 1;
					is_constant = false;
					break;
				case '=':
					if (is_first_expression == false)
						throw std::invalid_argument("Too many expressions in the formula, only two are allowed");
					is_first_expression = false;
					current_expression_terms = &_second_expression_terms;
					if (*(it + 1) == ' ')
						++it;
					break;
				case '+':
				case '-':
					current_expression_terms->push_back(new EquationOperator(*it));
					break;
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
				current_expression_terms->push_back(new EquationTerm(temporarycoefficient, temporary_degree, 0));
			else
			{
				try
				{
					EquationTerm &term = dynamic_cast<EquationTerm &>(*current_expression_terms->back());
					term.unknowns_degree = temporary_degree;
				}
				catch(const std::exception& e)
				{
				}
			}
		}

		EquationSolver(EquationSolver const &obj);
		EquationSolver &operator=(EquationSolver const &obj);

		std::vector<EquationComponent *>	_first_expression_terms;
		std::vector<EquationComponent *>	_second_expression_terms;
		std::vector<EquationComponent *>	_reduced_expression_terms;
		int								_polynomial_degree;
		float							_discriminant;
		float							_solutions[2];
};

#endif