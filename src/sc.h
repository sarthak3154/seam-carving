#ifndef SEAMCARVINGCOMP665156
#define SEAMCARVINGCOMP665156

#include <opencv2/opencv.hpp>

// the function you need to implement - by default it calls execute_seam_carving
bool seam_carving(cv::Mat& in_image, int new_width, int new_height, cv::Mat& out_image);

std::vector<std::vector<int> > assign_energy_to_pixels(cv::Mat& in_image, cv::Mat& out_image);

std::vector<int> find_seam(cv::Mat& in_image, cv::Mat& out_image, std::vector<std::vector<int> > energy);

bool execute_seam_carving(cv::Mat& in_image, int new_width, int new_height, cv::Mat& out_image);


#endif
