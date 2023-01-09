/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Complex.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldutriez <ldutriez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:58:54 by ldutriez          #+#    #+#             */
/*   Updated: 2023/01/09 22:20:58 by ldutriez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V1_COMPLEX_HPP
# define COMPUTOR_V1_COMPLEX_HPP

# include <iostream>

// Structure to store complex numbers in the form of a + bi
struct Complex
{
	float		real;
	float		imaginary;

	Complex(float real, float imaginary)
	: real(real), imaginary(imaginary)
	{}

	Complex(float real)
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
		float denominator = obj.real * obj.real + obj.imaginary * obj.imaginary;
		
		return Complex((real * obj.real + imaginary * obj.imaginary) / denominator,
					   (imaginary * obj.real - real * obj.imaginary) / denominator);
	}

	friend std::ostream &operator<<(std::ostream &os, const Complex &obj)
	{
		if (obj.real == 0 && obj.imaginary == 0)
		{
			os << 0;
			return os;
		}
		if (obj.real != 0)
			os << obj.real;
		if (obj.imaginary > 0 && obj.real != 0)
			os << " + ";
		else if (obj.imaginary < 0 && obj.real != 0)
			os << " - ";
		if (obj.imaginary != 0)
		{
			if (obj.imaginary < -1 && obj.real != 0)
				os << -obj.imaginary;
			else if (obj.imaginary > 1 || obj.imaginary < -1)
				os << obj.imaginary;
			os << "i";
		}
		return os;
	}
};

#endif