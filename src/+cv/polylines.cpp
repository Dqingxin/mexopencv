/**
 * @file polylines.cpp
 * @brief mex interface for polylines
 * @author Kota Yamaguchi
 * @date 2012
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
    // Check the number of arguments
    if (nrhs<2 || (nrhs%2)!=0 || nlhs>1)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");

    // Argument vector
    vector<MxArray> rhs(prhs,prhs+nrhs);

    // Option processing
    bool isClosed = true;
    Scalar color;
    int thickness = 1;
    int lineType = cv::LINE_8;
    int shift = 0;
    for (int i=2; i<nrhs; i+=2) {
        string key(rhs[i].toString());
        if (key=="Color")
            color = rhs[i+1].toScalar();
        else if (key=="Thickness")
            thickness = (rhs[i+1].isChar()) ?
                ThicknessType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key=="LineType")
            lineType = (rhs[i+1].isChar()) ?
                LineType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key=="Shift")
            shift = rhs[i+1].toInt();
        else if (key=="Closed")
            isClosed = rhs[i+1].toBool();
        else
            mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
    }

    typedef vector<Point> VecP;
    const_mem_fun_ref_t<VecP,MxArray> f(&MxArray::toVector<Point>);
    vector<VecP> pts(rhs[1].toVector(f));

    // Process
    Mat img(rhs[0].toMat());
    polylines(img, pts, isClosed, color, thickness, lineType, shift);
    plhs[0] = MxArray(img);
}
