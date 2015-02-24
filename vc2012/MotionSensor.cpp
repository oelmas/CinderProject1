#include <stdio.h>
#include <string>
#include <iostream>
#include <OpenNI.h>
#include "MotionSensor.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"


using namespace openni;
using namespace ci;
using namespace std;


#define READ_FRAME_WAIT_TIMEOUT 500;

MotionSensor::MotionSensor(void)
{
	mPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	mDepthWidth = 640;
	mDepthHeight = 480;
	mFPS = 30;

	mState = 1;

	// set the depth filter value
	depthFilterValue = 200;

	// set the default technique to be used to form reference background
	isTakeMin = false;
	isTakeAverage = !isTakeMin;
	if (isTakeMin)
		refImage = vector<vector<double>>(mDepthWidth, vector<double>(mDepthHeight, 1000000));
	else
		refImage = vector<vector<double>>(mDepthWidth, vector<double>(mDepthHeight, 0));

}


MotionSensor::~MotionSensor(void)
{
}

void MotionSensor::setup()
{
	// initialize the sensor and taking data from it
	start();

	// Form a reference background image
	calibrate(300); //TODO: 1. metodun kodu yazilacak 


}

openni::Status MotionSensor::start()
{
	// Create a Status object to hold results of function calls
	Status result = STATUS_OK;

	// Call initialize to make sure all drivers are loaded. If no drivers are found, this function will fail.
	// OpenNI::initialize() must have been run to make the device drivers on the system available to the API.
	result =  OpenNI::initialize();

	if (result != STATUS_OK)
	{
		app::console() <<"ERROR: Initialize failed !\n" << OpenNI::getExtendedError() << endl;
		close();
		return result;
	}

	result = mDevice.open(ANY_DEVICE);
	if (result != STATUS_OK)
	{
		app::console() << "ERROR : Device cannot open!" << OpenNI::getExtendedError() << endl;
		mDevice.close();
		return result;
	} 
	else
	{
		// Display the supported video modes of depth sensor
		const SensorInfo *sensorInfo = mDevice.getSensorInfo(SENSOR_DEPTH); //

		app::console() << " \nSUPPORTED VIDEO MODES OF DEPTH SENSOR\n " << endl;

		for (int i= 0; i< sensorInfo->getSupportedVideoModes().getSize() ;++i)
		{
			displayVideoMode(sensorInfo->getSupportedVideoModes()[i]); //TODO: 2. Methodun govedsi yazilacak.
		}

	}

	// Create the VideoStream object which encapsulates the depth video stream of the device
	result =  createStream(mDevice, SENSOR_DEPTH, mDepth);
	if (result != STATUS_OK)
	{ 
		app::console() << "ERROR: Depth video stream failed !" << OpenNI::getExtendedError() << endl;
		close();
		return result;
	} 
	else
	{
		displayVideoMode(mDepth.getVideoMode(), "SELECTED VIDEO MODE");
	}

	/*
		// Create the VideoStream object which encapsulates color video stream of the device.
		result = createStream(mDevice, SENSOR_COLOR, mColor);
		if(result != STATUS_OK)
		{
			// If color video stream fails, some basic log can be gotten by calling getExtendedError function
			app::console() << "ERROR: Color video stream failed.\n" << OpenNI::getExtendedError() << endl;

			// Shutdown the motion sensor
			close();
			// return corresponding error code
			return result;
		}
		else
		{
			// set the video mode of the color stream
			setVideoMode(mColor, PIXEL_FORMAT_RGB888, mDepthWidth, mDepthHeight, 10);
			// display the video mode of the color sensor
			displayVideoMode(mColor.getVideoMode());
		}
		*/


}

openni::Status MotionSensor::calibrate(int numImages)
{


}

void MotionSensor::close() // TODO: 3. Methodun gövdesi yazilacak
{

}

void MotionSensor::displayVideoMode(const openni::VideoMode &vMode, std::string title /*= ""*/) //TODO: 4.Methodun gövdesi yazilacak.
{

}

openni::Status MotionSensor::createStream(openni::Device &device, openni::SensorType type , openni::VideoStream &stream)
{
	openni::Status result = STATUS_OK;


	const SensorInfo *pSensorInfo= device.getSensorInfo(type);
	if (pSensorInfo == nullptr)
	{
		// If sensor is not available, get some basic log
		app::console() <<"ERROR: " << getSensorName(type) << "Sensor is not available. \n" << endl;
		return STATUS_ERROR;
	}


	// create a stream of frames from the given sensor type,
	result = stream.create(device, type);
	if (result != STATUS_OK)
	{
		app::console() << "ERROR: Could not create the " << getSensorName(type) << " Stream. \n" << OpenNI::getExtendedError() << endl;
		closeStream(stream);
		return result;
	}

	// set the video mode of the depth stream
	setVideoMode(mDepth, mPixelFormat, mDepthWidth, mDepthHeight, mFPS);

}

string MotionSensor::getSensorName(openni::SensorType type)
{
	switch (type)
	{
		case SENSOR_DEPTH : return "Depth";
		case  SENSOR_COLOR: return "Color";
		case SENSOR_IR: return "IR";
		default: return "Unknown";
		
	}
}

void MotionSensor::closeStream(openni::VideoStream & stream)//TODO: 5.Methodun gövdesi yazilacak.
{

}

openni::Status MotionSensor::setVideoMode(openni::VideoStream &stream, openni::PixelFormat format, int resX, int resY, int fps)
{
	// get to sensor info to perform necessary checks
	const SensorInfo &sInfo = stream.getSensorInfo();

	// get a list of supported video modes of the given stream
	const Array<VideoMode> &supportedModes = sInfo.getSupportedVideoModes();
	for (int i=0; i<supportedModes.getSize();++i)
	{
		const VideoMode* pSupportedMode = &supportedModes[i];
		if (pSupportedMode->getPixelFormat()== format)
			if(pSupportedMode->getResolutionX() == resX)
				if(pSupportedMode->getResolutionY() == resY)
					if(pSupportedMode->getFps() == fps)
					{
						stream.setVideoMode(*pSupportedMode);
						return STATUS_OK;
					}
					
	}

	return STATUS_NOT_SUPPORTED;

}


