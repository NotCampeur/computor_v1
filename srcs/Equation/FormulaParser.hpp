/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FormulaParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 04:42:43 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/03 04:49:37 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTER_V1_FORMULA_PARSER_HPP
# define COMPUTER_V1_FORMULA_PARSER_HPP

# include <string>
# include <vector>
# include <exception>
# include <cmath>
# include "EquationTerm.hpp"

class FormulaParser
{
	public:
		FormulaParser(const std::string &formula)
		: _first_expression_terms(), _second_expression_terms()
		{
			std::string spaceless_formula(formula);
			for (auto it(spaceless_formula.begin()); it != spaceless_formula.end(); ++it)
			{
				if (*it == ' ')
				{
					spaceless_formula.erase(it);
					it--;
				}
			}
			parse_formula(spaceless_formula);
		}
		
		~FormulaParser()
		{}

		FormulaParser(FormulaParser const &obj)
		: _first_expression_terms(obj._first_expression_terms)
		, _second_expression_terms(obj._second_expression_terms)
		{}

		FormulaParser &operator=(FormulaParser const &obj)
		{
			if (this != &obj)
			{
				_first_expression_terms = obj._first_expression_terms;
				_second_expression_terms = obj._second_expression_terms;
			}
			return (*this);
		}

		void get_expression_terms(std::vector<EquationTerm> &first_expression_terms, std::vector<EquationTerm> &second_expression_terms)
		{
			first_expression_terms = _first_expression_terms;
			second_expression_terms = _second_expression_terms;
		}

	private:
		void parse_formula(const std::string &formula)
		{
			std::vector<EquationTerm>	*current_expression_terms(&_first_expression_terms);
			float current_coefficient(0.0f);
			float temporary_exponant(0.0f);
			int check_unknown_degree(0);
			int current_unknown_degree(0);
			bool is_constant(true);
			bool is_negative(false);
			bool is_exponant(false);
			bool is_multiplication(false);
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
								current_coefficient = std::pow(current_coefficient, temporary_exponant);
							else if (is_constant == false)
							{
								if (is_multiplication == true)
									current_unknown_degree += temporary_exponant;
								else if (is_multiplication == false)
									current_unknown_degree = temporary_exponant;
								check_unknown_degree = temporary_exponant;
								if (temporary_exponant != check_unknown_degree)
									throw std::invalid_argument("Invalid exponant");
								is_multiplication = false;
							}
							is_exponant = false;
						}
						else
						{
							if (is_multiplication == true)
								current_coefficient *= std::stof(formula.substr(i), &convert_offset);
							else
							{
								if (current_coefficient != 0.0f)
									throw std::invalid_argument("Looks like the formula is not well formatted");
								current_coefficient = std::stof(formula.substr(i), &convert_offset);
							}
							current_coefficient = is_negative ? -current_coefficient : current_coefficient;
						}
						i += convert_offset - 1;
						break;
					case '-':
						is_negative = true;
						if (is_multiplication == true)
							break;
						if (i != 0)
						{
							current_expression_terms->push_back(EquationTerm(current_coefficient
																		, current_unknown_degree));
							current_coefficient = 0.0f;
							current_unknown_degree = 0;
							is_constant = true;
							is_exponant = false;
							is_multiplication = false;
						}
						break;
					case '\0':
					case '+':
					case '=':
						current_expression_terms->push_back(EquationTerm(current_coefficient
																		, current_unknown_degree));
						current_coefficient = 0.0f;
						current_unknown_degree = 0;
						is_negative = false;
						is_constant = true;
						is_exponant = false;
						is_multiplication = false;
						if (formula[i] == '=')
							current_expression_terms = &_second_expression_terms;
						break;
					case '*':
						is_multiplication = true;
						break;
					case 'X':
						if (i == 0
							|| formula[i - 1] == '+'
							|| formula[i - 1] == '-'
							|| formula[i - 1] == '=')
							current_coefficient = is_negative ? -1.0f : 1.0f;
						is_constant = false;
						if (formula[i + 1] != '^')
						{
							if (is_multiplication == true)
								++current_unknown_degree;
							else
								current_unknown_degree = 1;
							is_multiplication = false;
						}
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
				_second_expression_terms.push_back(EquationTerm(0.0f, 0));
		}

		std::vector<EquationTerm>	_first_expression_terms;
		std::vector<EquationTerm>	_second_expression_terms;
};

#endif