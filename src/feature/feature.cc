//File: feature.cc
//Date:
//Author: Yuxin Wu

#include <iostream>
#include "feature.hh"

#include "extrema.hh"
#include "orientation.hh"
#include "sift.hh"
#include "dog.hh"
#include "brief.hh"
#include "lib/imgproc.hh"
using namespace std;
using namespace config;


namespace pano {

// return half-shifted image coordinate
vector<Descriptor> FeatureDetector::detect_feature(const Mat32f& img) const {
	auto ret = do_detect_feature(img);
	// convert scale-coordinate to half-offset image coordinate
	for (auto& d: ret) {
		d.coor.x = (d.coor.x - 0.5) * img.width();
		d.coor.y = (d.coor.y - 0.5) * img.height();
        std::cout << d.coor.x << "," << d.coor.y << std::endl;
	}
	return ret;
}

// return half-shifted image coordinate
vector<Descriptor> FeatureDetector::detect_feature(const Mat32f& img, const Matuc &mask) const {
  auto ret = do_detect_feature(img, mask);
  // convert scale-coordinate to half-offset image coordinate
  vector<Descriptor> mask_ret;
  for (auto& d: ret) {
    d.coor.x = (d.coor.x - 0.5) * img.width();
    d.coor.y = (d.coor.y - 0.5) * img.height();
//    std::cout << d.coor.x << "," << d.coor.y << std::endl;
    int pixel_x = std::lround(d.coor.x) + img.width() / 2;
    int pixel_y = std::lround(d.coor.y) + img.height() / 2;
    unsigned char id = mask.at(pixel_y, pixel_x, 0);
    if (id != 0) {
      mask_ret.emplace_back(d);
    }
  }
  std::cout << "Before mask:" << ret.size() << ", after mask:" << mask_ret.size() << std::endl;
  return mask_ret;
}

// return [0, 1] coordinate
vector<Descriptor> SIFTDetector::do_detect_feature(const Mat32f& mat) const {
	// perform sift at this resolution
//	float ratio = SIFT_WORKING_SIZE * 2.0f / (mat.width() + mat.height());
//	Mat32f resized(mat.rows() * ratio, mat.cols() * ratio, 3);
//	resize(mat, resized);

	ScaleSpace ss(mat, NUM_OCTAVE, NUM_SCALE);
	DOGSpace sp(ss);

	ExtremaDetector ex(sp);
	auto keyp = ex.get_extrema();
	OrientationAssign ort(sp, ss, keyp);
	keyp = ort.work();
	SIFT sift(ss, keyp);
	auto descp = sift.get_descriptor();
	return descp;
}

// return [0, 1] coordinate
vector<Descriptor> SIFTDetector::do_detect_feature(const Mat32f& mat, const Matuc &mask) const {
  // perform sift at this resolution
//  float ratio = SIFT_WORKING_SIZE * 2.0f / (mat.width() + mat.height());
//  Mat32f resized(mat.rows() * ratio, mat.cols() * ratio, 3);
//  resize(mat, resized);
//  Matuc resized_mask(mat.rows() * ratio, mat.cols() * ratio, 1);
//  resize(mask, resized_mask);

  ScaleSpace ss(mat, NUM_OCTAVE, NUM_SCALE);
  DOGSpace sp(ss);

  ExtremaDetector ex(sp);
  auto keyp = ex.get_extrema();
  OrientationAssign ort(sp, ss, keyp);
  keyp = ort.work();
  SIFT sift(ss, keyp);
  auto descp = sift.get_descriptor();
  return descp;
}

BRIEFDetector::BRIEFDetector() {
	pattern.reset(new BriefPattern(
				BRIEF::gen_brief_pattern(BRIEF_PATH_SIZE, BRIEF_NR_PAIR)));
}

BRIEFDetector::~BRIEFDetector() {}

vector<Descriptor> BRIEFDetector::do_detect_feature(const Mat32f& mat) const {
	ScaleSpace ss(mat, NUM_OCTAVE, NUM_SCALE);
	DOGSpace sp(ss);

	ExtremaDetector ex(sp);
	auto keyp = ex.get_extrema();
	//OrientationAssign ort(sp, ss, keyp);
	//keyp = ort.work();
	BRIEF brief(mat, keyp, *pattern);

	auto ret = brief.get_descriptor();
	return ret;
}

vector<Descriptor> BRIEFDetector::do_detect_feature(const Mat32f& mat, const Matuc &mask) const {
  ScaleSpace ss(mat, NUM_OCTAVE, NUM_SCALE);
  DOGSpace sp(ss);

  ExtremaDetector ex(sp);
  auto keyp = ex.get_extrema();
  //OrientationAssign ort(sp, ss, keyp);
  //keyp = ort.work();
  BRIEF brief(mat, keyp, *pattern);

  auto ret = brief.get_descriptor();
  return ret;
}

}
