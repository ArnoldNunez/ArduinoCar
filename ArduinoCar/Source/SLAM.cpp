#include "..\Header\SLAM.h"

using namespace std;
using namespace ArduinoCar_Core;

ArduinoCar_Core::SLAM::SLAM()
{
	mInitLocConstr = 1.0;
	mMotionSigma = 1.0;
	mMeasurementSigma = 1.0;
	mDim = 0;
	mEstimatedHeading = 0.0;
}

ArduinoCar_Core::SLAM::~SLAM()
{
}

ArduinoCar_Core::Point2D ArduinoCar_Core::SLAM::ProcessMeasurements(const std::map<unsigned int, GemMeasurement>& measurements)
{
	// Initial matrix setup
	if (mDim == 0)
	{
		// Set the dimensions of the filter		
		mDim = 2 * (1 + measurements.size());

		// Initialize constraint information matrix and position vector
		mOmega.Zero(mDim, mDim);
		mXi.Zero(mDim, 1);

		// Set Initial location constraints
		mOmega(0, 0, mInitLocConstr);
		mOmega(1, 1, mInitLocConstr);

		// Set initial robot position constraint
		mXi(0, 0, 0);
		mXi(1, 0, 0);

		// Create mapping from landmark id to omega matrix index
		int ndx = 0;
		for (const auto& kvp : measurements)
		{
			// m is the index of the landmark coordinates in the matrix/vector
			int m = 2 * (1 + ndx);

			// Tie location id with matrix index
			mLandmarkNdxMap[kvp.first] = m;

			ndx += 1;
		}
	}

	// Integrate measurements
	for (const auto& kvp : measurements)
	{
		// Get the measurement index from the landmark to index map
		int m = mLandmarkNdxMap[kvp.first];

		// Get the x and y coordinates of the landmark based on the
		// distance and heading to landmark from robot
		double landmX = kvp.second.GetDistance() * cos(kvp.second.GetBearing() + mEstimatedHeading);
		double landmY = kvp.second.GetDistance() * sin(kvp.second.GetBearing() + mEstimatedHeading);
		double deltaLandmPos[2] = { landmX, landmY };

		// We have higher belief in measurements closer to the robot due to sensor
		double noiseSigma = pow(exp(1.0), kvp.second.GetDistance()) / 40.0;
		noiseSigma = 1.0;

		// Update the information matrix based on the measurement
		for (int b = 0; b < 2; b++)
		{
			double ulVal = mOmega(b, b) + (1.0 / noiseSigma);
			double lrVal = mOmega(m + b, m + b) + (1.0 / noiseSigma);

			double urVal = mOmega(b, m + b) + (-1.0 / noiseSigma);
			double llVal = mOmega(m + b, b) + (-1.0 / noiseSigma);

			mOmega(b, b, ulVal);
			mOmega(m + b, m + b, lrVal);
			mOmega(b, m + b, urVal);
			mOmega(m + b, b, llVal);

			mXi(b, 0, mXi(b, 0) - deltaLandmPos[b] * (1.0 / noiseSigma));
			mXi(m + b, 0, mXi(m + b, 0) + deltaLandmPos[b] * (1.0 / noiseSigma));
		}
	}

	// Compute best estimate
	Matrix mu = mOmega.Inverse().Multiply(mXi);
	double x = mu(0, 0);
	double y = mu(1, 0);

	return Point2D(x, y);
}

ArduinoCar_Core::Point2D ArduinoCar_Core::SLAM::ProcessMovement(double steering, double distance, double motionNoise)
{
	// Compute motion
	double deltaRobX = distance * cos(steering + mEstimatedHeading);
	double deltaRobY = distance * sin(steering + mEstimatedHeading);
	double deltaRobMotion[] = { deltaRobX, deltaRobY };


	// Perform the matrix expansion (Online Graph SLAM)
	vector<int> indices = { 0, 1 };
	vector<int> xiIndices = { 0 };
	for (int i = 4; i < mDim + 2; i++) { indices.push_back(i); }
	mOmega = mOmega.Expand(mDim + 2, mDim + 2, indices, indices);
	mXi = mXi.Expand(mDim + 2, 1, indices, xiIndices);


	// Update the information matrix based on the robot motion
	for (int b = 0; b < 4; b++)
	{
		mOmega(b, b, mOmega(b, b) + (1.0 / motionNoise));
	}
	for (int b = 0; b < 2; b++)
	{
		double topRight = mOmega(b, b + 2) + (-1.0 / motionNoise);
		double bottomLeft = mOmega(b + 2, b) + (-1.0 / motionNoise);

		mOmega(b, b + 2, topRight);
		mOmega(b + 2, b, bottomLeft);

		double topXi = mXi(b, 0) + (-deltaRobMotion[b] / motionNoise);
		double bottomXi = mXi(b + 2, 0) + (deltaRobMotion[b] / motionNoise);

		mXi(b, 0, topXi);
		mXi(b + 2, 0, bottomXi);
	}


	// Factour out the previous pose (Matrix integration)
	vector<int> tempIndices = { 0, 1 };
	vector<int> takeIndices;
	vector<int> emptyVector;
	for (int i = 2; i < mOmega.NumRows(); i++)
	{
		takeIndices.push_back(i);
	}
	Matrix a = mOmega.Take(tempIndices, takeIndices);
	Matrix b = mOmega.Take(tempIndices, emptyVector);
	Matrix c = mXi.Take(tempIndices, xiIndices);
	mOmega = mOmega.Take(takeIndices, emptyVector) - a.Transpose().Multiply(b.Inverse().Multiply(a));
	mXi = mXi.Take(takeIndices, xiIndices) - a.Transpose().Multiply(b.Inverse().Multiply(c));


	// Compute best estimate
	//Matrix mu = mOmega.Inverse().Multiply(mXi);
	this->mMu = mOmega.Inverse().Multiply(mXi);
	double x = this->mMu(0, 0);
	double y = mMu(1, 0);

	Point2D pt(x, y);
	mPrevEstimatedPos = mEstimatedPos;
	mEstimatedPos = pt;
	mEstimatedHeading += steering;

	return mEstimatedPos;
}
