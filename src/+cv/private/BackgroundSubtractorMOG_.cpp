/**
 * @file BackgroundSubtractorMOG_.cpp
 * @brief mex interface for BackgroundSubtractorMOG_
 * @author Kota Yamaguchi
 * @date 2012
 */
#include "mexopencv.hpp"
#include "opencv2/video/background_segm.hpp"
using namespace std;
using namespace cv;

// Persistent objects

/// Last object id to allocate
int last_id = 0;
/// Object container
map<int,BackgroundSubtractorMOG> obj_;

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
	if (nrhs<2 || nlhs>1)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    
	// Determine argument format between constructor or (id,method,...)
	vector<MxArray> rhs(prhs,prhs+nrhs);
	int id = 0;
	string method;
	if (nrhs>1 && rhs[0].isNumeric() && rhs[1].isChar()) {
		id = rhs[0].toInt();
		method = rhs[1].toString();
	}
	else
        mexErrMsgIdAndTxt("mexopencv:error","Invalid arguments");
	
	// Big operation switch
	if (method == "new") {
		if (nrhs>5  && (nrhs%2)==1) {
			int history = rhs[3].toInt();
			int nmixtures = rhs[4].toInt();
			double backgroundRatio = rhs[5].toDouble();
			double noiseSigma=0;
			for (int i=6;i<nrhs;i+=2) {
				string key(rhs[i].toString());
				if (key=="NoiseSigma")
					noiseSigma = rhs[i+1].toDouble();
				else
					mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
			}
			obj_[++last_id] = BackgroundSubtractorMOG(
				history,nmixtures,backgroundRatio,noiseSigma);
		}
		else if (nrhs==2)
			obj_[++last_id] = BackgroundSubtractorMOG();
		else
			mexErrMsgIdAndTxt("mexopencv:error","Invalid arguments");
		plhs[0] = MxArray(last_id);
		return;
	}
	
	BackgroundSubtractorMOG& obj = obj_[id];
    if (method == "delete") {
    	if (nrhs!=2 || nlhs!=0)
    		mexErrMsgIdAndTxt("mexopencv:error","Output not assigned");
    	obj_.erase(id);
    }
    else if (method == "apply") {
    	if (nrhs<3 || (nrhs%2)!=1 || nlhs>1)
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    	double learningRate=0;
    	for (int i=3; i<nrhs; i+=2) {
    		string key(rhs[i].toString());
    		if (key=="LearningRate")
    			learningRate = rhs[i+1].toDouble();
    		else
    			mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
    	}
    	Mat image(rhs[2].toMat()), fgmask;
    	obj(image, fgmask, learningRate);
    	plhs[0] = MxArray(fgmask);
    }
    else if (method == "getBackgroundImage") {
    	if (nrhs!=2 || nlhs>1)
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    	Mat im;
    	obj.getBackgroundImage(im);
    	plhs[0] = MxArray(im);
    }
    else if (method == "frameSize") {
    	if (nrhs==3 && nlhs==0)
    		obj.frameSize = rhs[2].toSize();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.frameSize);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "nframes") {
    	if (nrhs==3 && nlhs==0)
    		obj.nframes = rhs[2].toInt();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.nframes);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "history") {
    	if (nrhs==3 && nlhs==0)
    		obj.history = rhs[2].toInt();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.history);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "nmixtures") {
    	if (nrhs==3 && nlhs==0)
    		obj.nmixtures = rhs[2].toInt();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.nmixtures);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "varThreshold") {
    	if (nrhs==3 && nlhs==0)
    		obj.varThreshold = rhs[2].toDouble();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.varThreshold);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "backgroundRatio") {
    	if (nrhs==3 && nlhs==0)
    		obj.backgroundRatio = rhs[2].toDouble();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.backgroundRatio);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else if (method == "noiseSigma") {
    	if (nrhs==3 && nlhs==0)
    		obj.noiseSigma = rhs[2].toDouble();
    	else if (nrhs==2 && nlhs==1)
    		plhs[0] = MxArray(obj.noiseSigma);
    	else
    		mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    }
    else
		mexErrMsgIdAndTxt("mexopencv:error","Unrecognized operation");
}
