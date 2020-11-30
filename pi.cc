/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pi.cc                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktakami <ktakami@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 18:40:43 by ktakami           #+#    #+#             */
/*   Updated: 2020/11/30 18:50:06 by ktakami          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtweekend.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

int main2() {
	int inside_circle = 0;
	int runs = 0;

	std::cout << std::fixed << std::setprecision(12);
	while(true) {
		runs++;
		auto x = random_double(-1, 1);
		auto y = random_double(-1, 1);
		if (x * x + y * y < 1)
			inside_circle++;
		if (runs % 100000 == 0)
			std::cout << "Estimate of Pi = " << 4 * double(inside_circle) / runs << '\n';

	}
}

int main() {
	int inside_circle = 0;
	int inside_circle_stratified = 0;
	int sqrt_N = 10000;
	for (int i = 0; i < sqrt_N; i++) {
		for (int j = 0; j < sqrt_N; j++) {
			auto x = random_double(-1, 1);
			auto y = random_double(-1, 1);
			if (x * x + y * y < 1)
				inside_circle++;
			x = 2*((i + random_double()) / sqrt_N) - 1;
			y = 2*((j + random_double()) / sqrt_N) - 1;
			if (x * x + y * y < 1)
				inside_circle_stratified++;

		}
	}
	auto N = static_cast<double>(sqrt_N) * sqrt_N;
	std::cout << std::fixed << std::setprecision(12);
	std::cout
		<< "Regular Estimate of Pi = "
		<< 4 * double(inside_circle) / N << '\n'
		<< "Stratified Estimate of Pi = "
		<< 4 * double(inside_circle_stratified) / N << '\n';
}
