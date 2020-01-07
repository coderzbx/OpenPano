//File: imageref.hh
//Author: Yuxin Wu

#pragma once
#include <sys/stat.h>
#include <string>
#include <memory>
#include "lib/mat.h"
#include "lib/imgproc.hh"
#include "match_info.hh"
#include "common/common.hh"
namespace pano {
// A transparent reference to a image in file
struct ImageRef {
  std::string fname;
  std::string mask_name;
  Mat32f* img = nullptr;
  Matuc *mask_img = nullptr;
  int _width, _height;

  ImageRef(const std::string& fname):
      fname(fname) {
//    int pos = fname.find_last_of(".");
//    std::string file_name = fname.substr(0, pos);
    mask_name = fname + ".png";
    //
    struct stat buffer;
    if (stat(mask_name.c_str(), &buffer) != 0) {
      mask_name = "";
    }
  }
  //ImageRef(const ImageRef& ) = delete;  // TODO make it work
  ~ImageRef() {
    if (img)
      delete img;
    if (mask_img)
      delete mask_img;
  }

  void load() {
    if (img)
      return;
    img = new Mat32f{read_img(fname.c_str())};
    if (mask_name.length() > 0)
      mask_img = new Matuc{read_mask(mask_name.c_str())};
    _width = img->width();
    _height = img->height();
  }

  void release() {
    if (img) {
      delete img;
      img = nullptr;
    }
    if (mask_img) {
      delete mask_img;
      mask_img = nullptr;
    }
  }

  int width() const { return _width; }
  int height() const { return _height; }
  Shape2D shape() const { return {_width, _height}; }

};

}
