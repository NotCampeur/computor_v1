/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:58:54 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/23 13:05:21 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_COMPLEX_HPP
# define COMPUTOR_V1_COMPLEX_HPP

# include <iostream>
# include <gmpxx.h>

void to_string_formatter(std::string & str);

// Structure to store complex numbers in the form of a + bi
struct Complex
{
	mpf_class		real;
	mpf_class		imaginary;

	Complex(mpf_class real, mpf_class imaginary)
	: real(real), imaginary(imaginary)
	{}

	Complex(mpf_class real)
	: real(real), imaginary(0.0f)
	{}

	Complex()
	: real(0.0f), imaginary(0.0f)
	{}

	Complex(const Complex &obj)
	: real(obj.real), imaginary(obj.imaginary)
	{}
	
	Complex &operator=(const Complex &obj)
	{
		if (this != &obj)
		{
			real = obj.real;
			imaginary = obj.imaginary;
		}
		return *this;
	}

	Complex operator+(const Complex &obj) const
	{
		return Complex(real + obj.real, imaginary + obj.imaginary);
	}

	Complex operator-(const Complex &obj) const
	{
		return Complex(real - obj.real, imaginary - obj.imaginary);
	}

	Complex operator*(const Complex &obj) const
	{
		return Complex(real * obj.real - imaginary * obj.imaginary,
					   real * obj.imaginary + imaginary * obj.real);
	}

	Complex operator/(const Complex &obj) const
	{
		// Multiplying a complex by its conjugate is equal to a^2 + b^2.
		mpf_class denominator = obj.real * obj.real + obj.imaginary * obj.imaginary;
		
		return Complex((real * obj.real + imaginary * obj.imaginary) / denominator,
					   (imaginary * obj.real - real * obj.imaginary) / denominator);
	}

	friend std::ostream &operator<<(std::ostream &os, const Complex &obj)
	{
		std::string form;

		if (obj.real != 0.0f)
		{
			form = std::to_string(obj.real.get_d());
			to_string_formatter(form);
		}
		if (obj.imaginary != 0.0f)
		{
			if (obj.real != 0.0f)
				form += ' ';
			if (obj.imaginary > 0.0f && form.size() != 0)
				form += "+ ";
			else if (obj.imaginary < 0.0f)
				form += "-";
			if (obj.real != 0 && obj.imaginary < 0.0f)
				form += ' ';
			form += std::to_string(std::abs(obj.imaginary.get_d()));
			to_string_formatter(form);
			if (form.back() == '1'
				&& (form[form.size() - 2] < '0' || form[form.size() - 2] > '9')
				&& form[form.size() - 2] != '.')
				form.pop_back();
			form += "i";
		}
		if (form.size() == 0)
			form = "0";
		os << form;
		return os;
	}
};

#endif