/*
 * Rotate3d.cpp
 *
 *  Created on: 31/03/2014
 *      Author: Jonathan
 */

#include "Rotate3d.h"




Mat* Rotate(Mat src) {
	 cv::Mat* frameArray = new cv::Mat[2];

	 Point2f srcTri[3];
	 Point2f dstTri[3];

	 Mat rot_mat(2, 3, CV_32FC1);
	 Mat warp_mat(2, 3, CV_32FC1);
	 Mat warp_dst, warp_rotate_dst;



	 // Initialising destination image the same size and type as the input
	 warp_dst = Mat::zeros(src.rows,src.cols, src.type());

	 // Choosing the three points to conduct the Affine transform on
	 srcTri[0] = Point2f( 0,0 );
	 srcTri[1] = Point2f( src.cols - 1, 0 );
	 srcTri[2] = Point2f( 0, src.rows - 1 );

	 dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
	 dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
	 dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );

	 // Get the Affine transform
	 warp_mat = getAffineTransform(srcTri, dstTri);

	 // Apply the Affine transform
	 warpAffine(src, warp_dst, warp_mat, warp_dst.size());

	 // Define rotational parameters for the xy plane
	 Point center = Point(warp_dst.cols/2, warp_dst.rows/2);
	 double angle = -50.0;
	 double scale = 0.6;

	 // Use the parameters to construct the translation matrix
	 rot_mat = getRotationMatrix2D(center, angle, scale);

	 // Perform an Affine transformation to complete the rotation
	 warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

	 frameArray[0] = warp_dst;
	 frameArray[1] = warp_rotate_dst;

	 return frameArray;





}


