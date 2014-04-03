/////////////////////////////////////////////////////
//
//CMatShow class for output mat data as terminal show
//
////////////////////////////////////////////////////

#pragma once

//Include OpenCV header files
#include "opencv2\core\core.hpp"
//C++standard header files
#include <stdio.h>
using namespace std;

class CMatShow
{
public:
	CMatShow();
	virtual ~CMatShow();

	// Load Image Resource 
	void LoadImgResource(string ImgSrcPath);
};

