#include "KinectManager.h"


KinectManager::KinectManager() : resolution(NUI_IMAGE_RESOLUTION_640x480)
{
}


KinectManager::~KinectManager()
{
}

void KinectManager::Initialize(){
	
	int iSensorCount;

	ERROR_CHECK(NuiGetSensorCount(&iSensorCount));

	if (iSensorCount == 0){
		throw std::runtime_error("Not found Kinect  device.");
	}

	ERROR_CHECK(NuiCreateSensorByIndex(0, &mNuiSensor));


	HRESULT status = mNuiSensor->NuiStatus();

	if (!SUCCEEDED(status))
		throw std::runtime_error("Error in create Kinect Sensor Object");

	ERROR_CHECK(mNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON));
		
	NuiImageResolutionToSize(resolution, width, height);
}


void KinectManager::InitializeImageRGBStream(){
	eventHandles[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	ERROR_CHECK(mNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, resolution, 0, 2, eventHandles[0], &imageStreamHandle));
}

void KinectManager::InitializeDepthStream(){
	eventHandles[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	ERROR_CHECK(mNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, resolution, 0, 2, eventHandles[1], &depthStreamHandle));
}


void KinectManager::InitializaSkeletonTraking(){
	eventHandles[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
	ERROR_CHECK(mNuiSensor->NuiSkeletonTrackingEnable(eventHandles[2], NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE));
}

NUI_IMAGE_FRAME KinectManager::getRGBFrame(){
	NUI_IMAGE_FRAME imageFrame = { 0 };
	mNuiSensor->NuiImageStreamGetNextFrame(imageStreamHandle, 0, &imageFrame);
	return imageFrame;
}

void KinectManager::releseRGBFrame(NUI_IMAGE_FRAME * imageFrame){
	ERROR_CHECK(mNuiSensor->NuiImageStreamReleaseFrame(imageStreamHandle, imageFrame));
}

NUI_IMAGE_FRAME KinectManager::getDepthFrame(HRESULT * ret){
	NUI_IMAGE_FRAME depthFrame = { 0 };
	*ret = mNuiSensor->NuiImageStreamGetNextFrame(depthStreamHandle, 0, &depthFrame);
	return depthFrame;
}

void KinectManager::releseDepthFrame(NUI_IMAGE_FRAME * depthFrame){
	ERROR_CHECK(mNuiSensor->NuiImageStreamReleaseFrame(depthStreamHandle, depthFrame));
}

INuiFrameTexture* KinectManager::getDepthImagePixelFrameTexture(NUI_IMAGE_FRAME * imageFrame){
	BOOL nearMode;
	INuiFrameTexture* pTexture;
	ERROR_CHECK(mNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(
		depthStreamHandle, imageFrame, &nearMode, &pTexture));
	return pTexture;
}

NUI_SKELETON_FRAME KinectManager::getSkeletonFrame(HRESULT * ret){
	NUI_SKELETON_FRAME skeletonFrame = { 0 };
	*ret = mNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	return skeletonFrame;
}

INuiCoordinateMapper * KinectManager::createCoordinateMapper(){
	INuiCoordinateMapper * mCordinateMapper;
	ERROR_CHECK(mNuiSensor->NuiGetCoordinateMapper(&mCordinateMapper));
	return mCordinateMapper;
}

void KinectManager::getColorPixelCoordinatesFromDepthPixelAtResolution(LONG depthX, LONG depthY, LONG * colorX, LONG * colorY){
	ERROR_CHECK(mNuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(resolution, resolution, 0, depthX, depthY, 0, colorX, colorY));
}