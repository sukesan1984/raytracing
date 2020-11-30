/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integrate_x_sq.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktakami <ktakami@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 18:55:37 by ktakami           #+#    #+#             */
/*   Updated: 2020/11/30 18:57:12 by ktakami          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

int main() {
	int N = 1000000;
	auto sum = 0.0;

	for (int i = 0; i < N; i++) {
		auto x = random_double(0, 2);
		sum += x*x;
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "I = " << 2 * sum/N << '\n';
}
