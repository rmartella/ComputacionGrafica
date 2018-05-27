#pragma once

#include <objbase.h>
#include <NuiApi.h>
#include <NuiSkeleton.h>
#include <KinectInteraction.h>
#include <iostream>
#include <sstream>

#define ERROR_CHECK(ret)                                      \
	if(ret != S_OK) {                                           \
		std::stringstream ss;                                       \
		ss << "failed " #ret " " << std::hex << ret << std::endl;   \
		throw std::runtime_error(ss.str().c_str());                 \
	}

class KinectManager
{
public:
	HANDLE streamEvent;
	HANDLE eventHandles[4];
	DWORD width;
	DWORD height;

	KinectManager();
	~KinectManager();
	void Initialize();
	void InitializeImageRGBStream();
	void InitializeDepthStream();
	void InitializaSkeletonTraking();

	NUI_IMAGE_FRAME getRGBFrame();
	NUI_IMAGE_FRAME getDepthFrame(HRESULT * ret);
	NUI_SKELETON_FRAME getSkeletonFrame(HRESULT * ret);
	INuiFrameTexture* getDepthImagePixelFrameTexture(NUI_IMAGE_FRAME * imageFrame);

	void releseRGBFrame(NUI_IMAGE_FRAME * imageFrame);
	void releseDepthFrame(NUI_IMAGE_FRAME * depthFrame);
	void releseSkeletonFrame(NUI_SKELETON_FRAME skeletonFrame);

	INuiCoordinateMapper * createCoordinateMapper();

	HANDLE * getEventHandles(){
		return this->eventHandles;
	}

	void setResolution(_NUI_IMAGE_RESOLUTION resolution){
		this->resolution = resolution;
	}

	_NUI_IMAGE_RESOLUTION getResolution(){
		return this->resolution;
	}

	void getColorPixelCoordinatesFromDepthPixelAtResolution(LONG depthX, LONG depthY, LONG * colorX, LONG * colorY);

protected:


private:

	INuiSensor*  mNuiSensor;
	_NUI_IMAGE_RESOLUTION resolution;

	HANDLE imageStreamHandle;
	HANDLE depthStreamHandle;

};

