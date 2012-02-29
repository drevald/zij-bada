/*
 * SkyFactory.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: Denis
 */

#include "SkyFactory.h"
#include "SkyIterator.h"

SkyFactory::SkyFactory() {
	// TODO Auto-generated constructor stub

}

SkyFactory::~SkyFactory() {
	// TODO Auto-generated destructor stub
}

SkyIterator*
SkyFactory::getStars(float magnitude)
{
	AppLog("Getting stars with magnitude < ", magnitude);
	Osp::Base::String fileName(L"/Home/catalogue-");
	fileName.Append(magnitude);
	return new SkyIterator(fileName);
}



