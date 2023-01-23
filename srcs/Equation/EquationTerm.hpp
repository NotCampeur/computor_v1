/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EquationTerm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 22:45:53 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 13:05:21 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTER_V1_EQUATION_TERM_HPP
# define COMPUTER_V1_EQUATION_TERM_HPP

# include <iostream>
# include <gmpxx.h>

struct EquationTerm
{
		mpf_class	coefficient;
		mpf_class	unknowns_degree;

		EquationTerm()
		: coefficient(0.0f), unknowns_degree(0)
		{}
		
		EquationTerm(mpf_class p_coefficient, mpf_class p_unknowns_degree)
		: coefficient(p_coefficient)
		, unknowns_degree(p_unknowns_degree)
		{}
		
		~EquationTerm() {}
		
		EquationTerm(EquationTerm const &obj)
		: coefficient(obj.coefficient)
		, unknowns_degree(obj.unknowns_degree)
		{}
		
		EquationTerm operator-() const
		{
			return (EquationTerm(-coefficient, unknowns_degree));
		}

		EquationTerm &operator=(EquationTerm const &obj)
		{
			if (this != &obj)
			{
				coefficient = obj.coefficient;
				unknowns_degree = obj.unknowns_degree;
			}
			return (*this);
		}
};

std::ostream &operator<<(std::ostream &os, const EquationTerm &obj)
{
	if (obj.coefficient != 1.0f || obj.unknowns_degree == 0)
		os << obj.coefficient;
	if (obj.unknowns_degree != 0)
	{
		os << "X";
		if (obj.unknowns_degree != 1)
			os << "^" << obj.unknowns_degree;
	}
	os << " ";
	return (os);
}

#endif