//File: imgproc.hh
//Date:
//Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#pragma once
#include "mat.h"
#include "color.hh"

#include <list>

Mat32f read_rgb(const char* fname);
void write_rgb(const char* fname, const Mat32f& mat);

Mat32f hconcat(const std::list<Mat32f>& mats);

Color interpolate(const Mat32f& mat, float r, float c);

Mat32f crop(const Mat32f& mat);

Mat32f rgb2grey(const Mat32f& mat);

bool is_edge_color(const Mat32f& mat, float y, float x);

void fill(Mat32f& mat, const Color& c);

template <typename T>
void resize(const Mat<T> &src, Mat<T> &dst);