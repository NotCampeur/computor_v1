/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FormulaParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 04:42:43 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/30 16:14:33 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTER_V1_FORMULA_PARSER_HPP
# define COMPUTER_V1_FORMULA_PARSER_HPP

# include <string>
# include <vector>
# include <optional>
# include <exception>
# include "EquationTerm.hpp"
# include "math_functions.hpp"
# include "strings_functions.hpp"

class FormulaParser
{
	public:
		FormulaParser(const std::string &formula)
		: _first_expression_terms(), _second_expression_terms(), _formula_index(0)
		, _spaceless_formula(formula), _current_expression_terms(&_first_expression_terms)
		, _current_coefficient(), _current_unknown_degree(0)
		, _is_constant(true), _is_negative(false), _is_exponant(false)
		, _is_multiplication(false), _is_division(false)
		{
			_remove_spaces();
			parse_formula();
		}
		
		~FormulaParser()
		{}

		FormulaParser(FormulaParser const &obj)
		: _first_expression_terms(obj._first_expression_terms)
		, _second_expression_terms(obj._second_expression_terms)
		, _formula_index(obj._formula_index), _spaceless_formula(obj._spaceless_formula)
		, _current_expression_terms(obj._current_expression_terms)
		, _current_coefficient(obj._current_coefficient)
		, _current_unknown_degree(obj._current_unknown_degree)
		, _is_constant(obj._is_constant), _is_negative(obj._is_negative)
		, _is_exponant(obj._is_exponant), _is_multiplication(obj._is_multiplication)
		, _is_division(obj._is_division)
		{}

		FormulaParser &operator=(FormulaParser const &obj)
		{
			if (this != &obj)
			{
				_first_expression_terms = obj._first_expression_terms;
				_second_expression_terms = obj._second_expression_terms;
				_formula_index = obj._formula_index;
				_spaceless_formula = obj._spaceless_formula;
				_current_expression_terms = obj._current_expression_terms;
				_current_coefficient = obj._current_coefficient;
				_current_unknown_degree = obj._current_unknown_degree;
				_is_constant = obj._is_constant;
				_is_negative = obj._is_negative;
				_is_exponant = obj._is_exponant;
				_is_multiplication = obj._is_multiplication;
				_is_division = obj._is_division;
			}
			return (*this);
		}

		void get_expression_terms(std::vector<EquationTerm> &first_expression_terms, std::vector<EquationTerm> &second_expression_terms)
		{
			first_expression_terms = _first_expression_terms;
			second_expression_terms = _second_expression_terms;
		}

	private:
		void _remove_spaces(void)
		{
			for (size_t i(0); i < _spaceless_formula.size(); i++)
			{
				if (_spaceless_formula[i] == ' ')
				{
					_spaceless_formula.erase(i, 1);
					if (i > 0
						&& isdigit(_spaceless_formula[i - 1])
						&& isdigit(_spaceless_formula[i]))
						throw std::invalid_argument("Invalid formula, dangling digits");
					--i;
				}
			}
		}

		void parse_formula(void)
		{
			for (; _formula_index != _spaceless_formula.size() + 1; ++_formula_index)
			{
				switch (_spaceless_formula[_formula_index])
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
						_digit_case();
						break;
					case '-':
						if (_formula_index != 0
							&& _spaceless_formula[_formula_index - 1] == '+')
							throw std::invalid_argument("Signs should not be next to each other");
						_minus_case();
						break;
					case '+':
						if (_formula_index == 0 || _spaceless_formula[_formula_index - 1] == '=')
							throw std::invalid_argument("Expression should not start with a '+'");
						else if (isdigit(_spaceless_formula[_formula_index - 1]) == false
							&& _spaceless_formula[_formula_index - 1] != 'X')
							throw std::invalid_argument("Signs should not be next to each other");
						[[fallthrough]];
					case '\0':
					case '=':
						_plus_equal_null_case();
						break;
					case '*':
					case '/':
						if (_current_coefficient.has_value() == false)
							throw std::invalid_argument("Invalid formula, dangling signs");
						if (_formula_index != 0
							&& isdigit(_spaceless_formula[_formula_index - 1]) == false
							&& _spaceless_formula[_formula_index - 1] != 'X')
							throw std::invalid_argument("Signs should not be next to each other");
						if (_spaceless_formula[_formula_index] == '*')
							_is_multiplication = true;
						else
							_is_division = true;
						break;
					case 'X':
						_unknown_case();
						break;
					case '^':
						if(_formula_index == 0 || (_formula_index != 0
							&& isdigit(_spaceless_formula[_formula_index - 1]) == false
							&& _spaceless_formula[_formula_index - 1] != 'X'))
							throw std::invalid_argument("Invalid formula, dangling exponent");
						_is_exponant = true;
						break;
					default:
						throw std::invalid_argument(std::string("Unexpected character found in the formula: ") + _spaceless_formula[_formula_index]);
				}
			}
			if (isdigit(_spaceless_formula.back()) == false && _spaceless_formula.back() != 'X')
				throw std::invalid_argument("Formula looks incomplete");
			if (_current_expression_terms == &_first_expression_terms)
				_second_expression_terms.push_back(EquationTerm(0.0f, 0));
		}

		void _digit_case(void)
		{
			mpf_class digit(0.0f);
			mpz_class check_unknown_degree(0);
			size_t convert_offset(0);
			
			std::string str_digit(get_digits(_spaceless_formula.substr(_formula_index)));
			digit.set_str(str_digit, 10);
			convert_offset = str_digit.size();
			while (_spaceless_formula[_formula_index + convert_offset] == '^'
				&& _is_constant == true)
			{
				if (isdigit(_spaceless_formula[_formula_index + convert_offset + 1]) == false
					&& _spaceless_formula[_formula_index + convert_offset + 1] != '-')
				{
					_formula_index += convert_offset - 1;
					return;
				}
				str_digit = get_digits(_spaceless_formula.substr(_formula_index + convert_offset + 1));
				convert_offset += str_digit.size() + 1;
				digit = power(digit, mpf_class(str_digit, 10));
			}
			if (_is_exponant == true)
			{
				if (_is_negative == true)
				{
					digit *= -1;
					_is_negative = false;
				}
				if (_is_constant == true)
				{
					_current_coefficient.value() = power(_current_coefficient.value(), digit);
				}
				else if (_is_constant == false)
				{
					if (_is_division == true)
						_current_unknown_degree -= digit;
					else if (_is_multiplication == true)
						_current_unknown_degree += digit;
					else
						_current_unknown_degree = digit;
					check_unknown_degree = digit;
					if (digit != check_unknown_degree)
						throw std::invalid_argument("Invalid exponant");
					while (_spaceless_formula[_formula_index + convert_offset] == '^')
					{
						if (isdigit(_spaceless_formula[_formula_index + convert_offset + 1]) == false
							&& _spaceless_formula[_formula_index + convert_offset + 1] != '-')
						{
							_formula_index += convert_offset - 1;
							return;
						}
						str_digit = get_digits(_spaceless_formula.substr(_formula_index + convert_offset + 1));
						convert_offset += str_digit.size() + 1;
						digit = str_digit;
						_current_unknown_degree *= digit;
						_current_coefficient = power(_current_coefficient.value(), digit);
						check_unknown_degree = digit;
						if (digit != check_unknown_degree)
							throw std::invalid_argument("Invalid exponant");
					}
					_is_multiplication = false;
					_is_division = false;
				}
				_is_exponant = false;
			}
			else
			{
				if (_is_division == true && _current_coefficient.has_value() == true)
				{
					if (digit == 0)
						throw std::invalid_argument("Division by zero");
					_current_coefficient.value() /= digit;
					if (_spaceless_formula[_formula_index + convert_offset] != 'X')
						_is_division = false;
				}
				else if (_is_multiplication == true && _current_coefficient.has_value() == true)
				{
					_current_coefficient.value() *= digit;
					if (_spaceless_formula[_formula_index + convert_offset] != 'X')
						_is_division = false;
				}
				else
				{
					if (_current_coefficient.has_value() == true)
						throw std::invalid_argument("Looks like the formula is not well formatted");
					_current_coefficient.emplace(digit);
				}
				if (_is_negative == true)
				{
					if (_current_coefficient.has_value() == true)
						_current_coefficient.value() *= -1;
					_is_negative = false;
				}
			}
			_formula_index += convert_offset - 1;			
		}

		void _minus_case(void)
		{
			if (_spaceless_formula[_formula_index + 1] == '-'
				|| _spaceless_formula[_formula_index + 1] == '+')
				throw std::invalid_argument("Signs should not be next to each other");
			_is_negative = !_is_negative;
			if (_is_division == true || _is_multiplication == true || _is_exponant == true)
				return;
			if (_formula_index != 0 && _spaceless_formula[_formula_index - 1] != '=')
			{
				if (_current_coefficient.has_value() == true)
					_current_expression_terms->push_back(EquationTerm(_current_coefficient.value()
															, _current_unknown_degree));
				_current_coefficient.reset();
				_current_unknown_degree = 0;
				_is_constant = true;
				_is_exponant = false;
				_is_multiplication = false;
				_is_division = false;
			}
		}

		void _plus_equal_null_case(void)
		{
			if (_current_coefficient.has_value() == true)
				_current_expression_terms->push_back(EquationTerm(_current_coefficient.value()
															, _current_unknown_degree));
			_current_coefficient.reset();
			_current_unknown_degree = 0;
			_is_negative = false;
			_is_constant = true;
			_is_exponant = false;
			_is_multiplication = false;
			_is_division = false;
			if (_spaceless_formula[_formula_index] == '=')
			{
				if (_current_expression_terms == &_second_expression_terms)
					throw std::invalid_argument("Formula has too many expressions");
				_current_expression_terms = &_second_expression_terms;
			}
		}

		void _unknown_case(void)
		{
			if (_is_division == true && _formula_index != 0	&&
				isdigit(_spaceless_formula[_formula_index - 1]) == false)
				throw std::invalid_argument("Division by unknown is not allowed");
			if (_current_coefficient.has_value() == false)
			{
				_current_coefficient.emplace(_is_negative ? -1.0f : 1.0f);
				_is_negative = false;
			}
			_is_constant = false;
			if (_spaceless_formula[_formula_index + 1] != '^')
			{
				if (_is_multiplication == true)
					++_current_unknown_degree;
				else
					_current_unknown_degree = 1;
				_is_multiplication = false;
			}
		}

		std::vector<EquationTerm>	_first_expression_terms;
		std::vector<EquationTerm>	_second_expression_terms;
		size_t						_formula_index;
		std::string					_spaceless_formula;
		std::vector<EquationTerm>	*_current_expression_terms;
		std::optional<mpf_class>	_current_coefficient;
		mpz_class					_current_unknown_degree;
		bool 						_is_constant;
		bool 						_is_negative;
		bool 						_is_exponant;
		bool 						_is_multiplication;
		bool 						_is_division;
};

#endif