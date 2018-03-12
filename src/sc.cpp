
#include "sc.h"

using namespace cv;
using namespace std;




bool seam_carving(Mat& in_image, int new_width, int new_height, Mat& out_image){

    // some sanity checks
    // Check 1 -> new_width <= in_image.cols
    if(new_width>in_image.cols){
        cout<<"Invalid request!!! new_width has to be smaller than the current size!"<<endl;
        return false;
    }
    if(new_height>in_image.rows){
        cout<<"Invalid request!!! ne_height has to be smaller than the current size!"<<endl;
        return false;
    }
    
    if(new_width<=0){
        cout<<"Invalid request!!! new_width has to be positive!"<<endl;
        return false;

    }
    
    if(new_height<=0){
        cout<<"Invalid request!!! new_height has to be positive!"<<endl;
        return false;
        
    }

    
    return assign_energy_to_pixels(in_image, out_image);
}

/**
 * Calculate pixel energy using dual gradient energy function
 * (Ref: http://www.cs.princeton.edu/courses/archive/spring14/cos226/assignments/seamCarving.html)
 *
 * @param in_image
 * @param i
 * @param j
 * @return
 */
double calculate_pixel_energy(Mat &in_image, int i, int j) {

    Vec3b pixel_xl = in_image.at<Vec3b>((i != 0) ? i - 1 : in_image.rows - 1, j);
    Vec3b pixel_xr = in_image.at<Vec3b>((i != in_image.rows - 1) ? i + 1 : 0, j);
    Vec3b pixel_yt= in_image.at<Vec3b>(i, (j != 0) ? j - 1 : in_image.cols - 1);
    Vec3b pixel_yb = in_image.at<Vec3b>(i, (j != in_image.cols - 1) ? j + 1 : 0);
    double dx = abs(pixel_xr[0] - pixel_xl[0]) + abs(pixel_xr[1] - pixel_xl[1]) + abs(pixel_xr[2] - pixel_xl[2]);
    double dy = abs(pixel_yb[0] - pixel_yt[0]) + abs(pixel_yb[1] - pixel_yt[1]) + abs(pixel_yb[2] - pixel_yt[2]);

    return dx + dy;
}

/**
 * Map a pixel into energy value
 *
 * @param in_image
 */
bool assign_energy_to_pixels(Mat &in_image, Mat &out_image) {
    int rows = in_image.rows;
    int cols = in_image.cols;

    out_image = in_image.clone();

    for (int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {

            out_image.at<Vec3b>(i, j)  = calculate_pixel_energy(in_image, i, j);
        }
    }
    return true;
}

