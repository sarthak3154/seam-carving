
#include "sc.h"

using namespace cv;
using namespace std;
/*
 * References:
 * http://www.cs.cmu.edu/afs/andrew/scs/cs/15-463/f07/proj2/www/wwedler/
 * http://www.cs.princeton.edu/courses/archive/spring14/cos226/assignments/seamCarving.html
 */

bool seam_carving(Mat &in_image, int new_width, int new_height, Mat &out_image) {

    // some sanity checks
    // Check 1 -> new_width <= in_image.cols
    if (new_width > in_image.cols) {
        cout << "Invalid request!!! new_width has to be smaller than the current size!" << endl;
        return false;
    }
    if (new_height > in_image.rows) {
        cout << "Invalid request!!! ne_height has to be smaller than the current size!" << endl;
        return false;
    }

    if (new_width <= 0) {
        cout << "Invalid request!!! new_width has to be positive!" << endl;
        return false;

    }

    if (new_height <= 0) {
        cout << "Invalid request!!! new_height has to be positive!" << endl;
        return false;

    }


    return execute_seam_carving(in_image, new_width, new_height, out_image);
}

/**
 * Calculate pixel energy using dual gradient energy function
 * (Reference: http://www.cs.princeton.edu/courses/archive/spring14/cos226/assignments/seamCarving.html)
 *
 * @param in_image
 * @param i
 * @param j
 * @return
 */
double calculate_pixel_energy(Mat &in_image, int i, int j) {

    Vec3b pixel_xl = in_image.at<Vec3b>((i != 0) ? i - 1 : in_image.rows - 1, j);
    Vec3b pixel_xr = in_image.at<Vec3b>((i != in_image.rows - 1) ? i + 1 : 0, j);
    Vec3b pixel_yt = in_image.at<Vec3b>(i, (j != 0) ? j - 1 : in_image.cols - 1);
    Vec3b pixel_yb = in_image.at<Vec3b>(i, (j != in_image.cols - 1) ? j + 1 : 0);
    double dx = pow(pixel_xr[0] - pixel_xl[0], 2) + pow(pixel_xr[1] - pixel_xl[1], 2) + pow(pixel_xr[2] - pixel_xl[2], 2);
    double dy = pow(pixel_yb[0] - pixel_yt[0], 2) + pow(pixel_yb[1] - pixel_yt[1], 2) + pow(pixel_yb[2] - pixel_yt[2], 2);

    return sqrt(dx + dy);
}

/**
 * Map a pixel into energy value
 *
 * @param in_image
 */
vector<vector<int> > assign_energy_to_pixels(Mat &in_image, Mat &out_image) {
    int rows = in_image.rows;
    int cols = in_image.cols;

    out_image = in_image.clone();
    vector<vector<int> > energy(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            energy[i].push_back((int) calculate_pixel_energy(in_image, i, j));
            out_image.at<Vec3b>(i, j) = energy[i][j];
        }
    }

    return energy;
}

vector<int> find_seam(Mat &in_image, Mat &out_image, vector<vector<int> > energy) {
    int rows = in_image.rows;
    int cols = in_image.cols;

    vector<vector<int> > T(rows);
    for (int k = 0; k < cols; k++) {
        T[0].push_back(energy[0][k]);
    }

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(j == 0)
                T[i].push_back(energy[i][j] + min(T[i-1][j], T[i-1][j+1]));
            else if(j == cols-1)
                T[i].push_back(energy[i][j] + min(T[i-1][j-1], T[i-1][j]));
            else
                T[i].push_back(energy[i][j] + min(min(T[i-1][j], T[i-1][j+1]), T[i-1][j-1]));

        }
    }

    int min_value = numeric_limits<int>::max();
    // To backtrack from the lower most row for the seam path
    int min_index = -1;

    for (int l = 0; l < cols; l++) {
        if(T[rows-1][l] < min_value) {
            min_value = T[rows-1][l];
            min_index = l;
        }
    }


    vector<int> path(rows);
    Point point(rows - 1, min_index);
    path[point.x] = point.y;

    while(point.x != 0) {
        int row = point.x, col = point.y;
        int value = T[row][col] - energy[row][col];
        if(col == 0) {
            if(value == T[row-1][col]) {
                point = Point(row-1, col);
            } else {
                point = Point(row-1, col+1);
            }
        } else if(col == cols - 1) {
            if(value == T[row-1][col]) {
                point = Point(row-1, col);
            } else {
                point = Point(row-1, col-1);
            }
        } else {
            if(value == T[row-1][col-1]) {
                point = Point(row-1, col-1);
            } else if(value == T[row-1][col]) {
                point = Point(row-1, col);
            } else {
                point = Point(row-1, col+1);
            }
        }
        path[point.x] = point.y;
    }

    return path;
}


bool execute_seam_carving(Mat &in_image, int new_width, int new_height, Mat &out_image) {

    Mat iimage = in_image.clone();
    Mat oimage = in_image.clone();

    vector<vector<int> > energy = assign_energy_to_pixels(in_image, out_image);
    vector<int> path = find_seam(in_image, out_image, energy);

    return true;
}
