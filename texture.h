/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktakami <ktakami@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 18:22:41 by ktakami           #+#    #+#             */
/*   Updated: 2020/11/09 18:22:41 by ktakami          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define STB_IMAGE_IMPLEMENTATION

#include "rtweekend.h"
#include "rtw_stb_image.h"
#include "perline.h"
#include <iostream>

class texture {
public:
	virtual ~texture() {}
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
	solid_color() {}
	solid_color(color c) : color_value(c) {}
	solid_color(double red, double green, double blue)
		: solid_color(color(red, green, blue)) {}
	virtual color value(double u, double v, const vec3& p) const {
		return color_value;
	}
private:
	color color_value;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1)
		: even(t0), odd(t1) {}
	virtual color value(double u, double v, const point3& p) const {
		auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
		if (sines <0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
public:
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(double sc) : scale(sc) {}
	virtual color value(double u, double v, const point3& p) const {
		return color(1,1,1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}
public:
	perlin noise;
	double scale;
};

class turbulence_texture : public texture {
public:
	turbulence_texture() {}
	virtual color value(double u, double v, const point3& p) const {
		return color(1, 1, 1) * noise.turb(p);
	}
	perlin noise;
};

class image_texture : public texture {
public:
	const static int bytes_per_pixel = 3;

	image_texture()
		: data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
	image_texture(const char* filename) {
		auto components_per_pixel = bytes_per_pixel;
		data =  stbi_load(
				filename, &width, &height, &components_per_pixel, components_per_pixel);
		if (!data) {
			std::cerr << "ERROR: Could not load texture image file '"
			<< filename
			<< "'.\n";
			width = height = 0;
		}
		bytes_per_scanline = bytes_per_pixel * width;
	}
	~image_texture() {
		delete data;
	}

	virtual color value(double u, double v, const vec3& p) const {
		//テクスチャのデータがない場合は、そのことがわかるようにシアン色を返す
		if (data == nullptr)
			return color(0, 1, 0);
		// 入力されたテクスチャ座標を[0, 1]で切り捨てる
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0);


		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		// 整数座標を更に切り捨てる（テクスチャ座標は1.0になってはいけない）。
		if (i >= width) i = width - 1;
		if (j >= height) j = height - 1;
		const auto color_scale = 1.0 / 255.0;
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
		return color(color_scale * pixel[0],
					color_scale * pixel[1],
					color_scale * pixel[2]);
	}
private:
	unsigned char *data;
	int width, height;
	int bytes_per_scanline;
};
