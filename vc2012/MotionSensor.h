#pragma once

#include <string>
#include <OpenNI.h>
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


class MotionSensor
{
public:
	MotionSensor(void);
	void setup();
	openni::Status start();
	openni::Status calibrate(int numImages);
	void displayVideoMode(const openni::VideoMode &vMode, std::string title = "");

	void close();
	~MotionSensor(void);
	openni::Status createStream(openni::Device &device, openni::SensorType type, openni::VideoStream &stream);
	std::string getSensorName(openni::SensorType type);
	void closeStream(openni::VideoStream & stream);
	openni::Status setVideoMode(openni::VideoStream & stream, openni::PixelFormat format, int resX, int resY,  int fps);
	int mState;
private:
	openni::Device mDevice;
	openni::VideoStream mDepth;
	openni::VideoStream mColor;
	openni::VideoFrameRef mFrame ;
	openni::PixelFormat mPixelFormat;

	int mDepthWidth;
	int mDepthHeight;
	int mFPS;

	ci::gl::Texture mImage;
	std::vector<std::vector<double>> refImage;
	std::vector<std::vector<double>> Image;

	int depthFilterValue;
	bool isTakeMin;
	bool isTakeAverage;

};

