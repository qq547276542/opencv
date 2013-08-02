/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "precomp.hpp"

namespace cv
{

/*
 *  TrackerModel
 */

TrackerModel::TrackerModel()
{
	stateEstimator = NULL;
}

TrackerModel::~TrackerModel()
{

}

bool TrackerModel::setTrackerStateEstimator( Ptr<TrackerStateEstimator> trackerStateEstimator )
{
	if( stateEstimator != NULL )
	{
		return false;
	}

	stateEstimator = trackerStateEstimator;
	return true;
}

Ptr<TrackerStateEstimator> TrackerModel::getTrackerStateEstimator() const
{
	return stateEstimator;
}

void TrackerModel::modelEstimation( const std::vector<Mat>& responses )
{
	modelEstimationImpl( responses );

}

void TrackerModel::clearCurrentConfidenceMap()
{
	currentConfidenceMap.clear();
}

void TrackerModel::modelUpdate()
{
	modelUpdateImpl();

	confidenceMaps.push_back( currentConfidenceMap );
	stateEstimator->update( confidenceMaps );

	clearCurrentConfidenceMap();

}

bool TrackerModel::runStateEstimator()
{
	if( stateEstimator == NULL )
	{
		CV_Error(-1, "Tracker state estimator is not setted");
		return false;
	}
	Ptr<TrackerTargetState> targetState = stateEstimator->estimate(confidenceMaps);
	if( targetState == NULL )
		return false;

	setLastTargetState(targetState);
	return true;
}

void TrackerModel::setLastTargetState( const Ptr<TrackerTargetState>& lastTargetState )
{
	trajectory.push_back(lastTargetState);
}

Ptr<TrackerTargetState> TrackerModel::getLastTargetState() const
{
	return trajectory.back();
}

const std::vector<ConfidenceMap>& TrackerModel::getConfidenceMaps() const
{
	return confidenceMaps;
}

const ConfidenceMap& TrackerModel::getLastConfidenceMap() const
{
	return confidenceMaps.back();
}


/*
 *  TrackerTargetState
 */

Point2f TrackerTargetState::getTargetPosition() const
{
	return targetPosition;
}


void TrackerTargetState::setTargetPosition( const Point2f& position )
{
	targetPosition = position;
}


} /* namespace cv */
