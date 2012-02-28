/*
 * SkyObject.cpp
 *
 *  Created on: 17.02.2012
 *      Author: ddreval
 */

#include "SkyObject.h"

using namespace Osp::Graphics;
using namespace Osp::Base::Utility;

SkyObject::SkyObject() {
	// TODO Auto-generated constructor stub

}

SkyObject::~SkyObject() {
	// TODO Auto-generated destructor stub
}

void
SkyObject::setRA(Osp::Base::String& raH, Osp::Base::String&  raM, Osp::Base::String&  raS)
{
	float f1, f2, f3;
	Osp::Base::Float::Parse(raH, f1);
	Osp::Base::Float::Parse(raM, f2);
	Osp::Base::Float::Parse(raS, f3);
	RAH = f1 + (f2/60) + (f3/3600);
	AppLog("Right Ascentation is %f", RAH);
}

float
SkyObject::getRAH(void) {
	return RAH;
}

float
SkyObject::getDED(void) {
	return DED;
}

void
SkyObject::setDE(Osp::Base::String& deD, Osp::Base::String&  deM, Osp::Base::String&  deS)
{
	float f1, f2, f3;
	Osp::Base::Float::Parse(deD, f1);
	Osp::Base::Float::Parse(deM, f2);
	Osp::Base::Float::Parse(deS, f3);
	DED = f1 + (f2/60) + (f3/3600);
	AppLog("Declination is %f", DED);
}

void
SkyObject::setMagnitude(Osp::Base::String& magnitudeStr) {
	float f;
	Osp::Base::Float::Parse(magnitudeStr, f);
	magnitude = f;
}

void
SkyObject::setSign(bool isNorthern) {
	sign = isNorthern;
}

void
SkyObject::setName(Osp::Base::String& aName) {
	name = aName;
}

void
SkyObject::setDraperName(Osp::Base::String& aDraperName) {
	draperName = aDraperName;
}

Osp::Base::String&
SkyObject::getName(void) {
	return name;
}

Osp::Base::String&
SkyObject::getDraperName(void) {
	return draperName;
}

bool
SkyObject::isNorthern(void) {
	return sign;
}

float
SkyObject::getMagnitude(void) {
	return magnitude;
}

void
SkyObject::draw(Sky* sky) {
	Canvas* canvas = sky -> getCanvas();
//	double r = (sky->getRadius())*
//    double r = (R * p.getCosAlt());
//    int top = (int)(R - r * p.getCosAz());
//    int left = (int)(R + r *p.getSinAz());

//public static edu.astro.PositionTrig getObjectTrigPosition(float aRah, float aDec) {

	float lstDeg = 15*(sky -> getSiderialHours());
	AppLog("lstDeg = %f", lstDeg);
	float raDeg = RAH * 15; // 24 hours is 360 degrees, so 1 hour is 15 degrees
	AppLog("raDeg = %f", raDeg);
	float ha = lstDeg > raDeg ? lstDeg - raDeg : 360 + lstDeg - raDeg;
	AppLog("ha = %f", ha);
	double radInDegree = 0.0174532925;

    double sinAlt =
        Math::Sin(radInDegree*DED)*Math::Sin(radInDegree*(sky->getLatitude()))
        +Math::Cos(radInDegree*DED)*Math::Cos(radInDegree*(sky->getLatitude()))*Math::Cos(radInDegree*(ha));
	AppLog("sinAlt = %f", sinAlt);

    double cosAlt =
        Math::Sqrt(1-sinAlt*sinAlt);
	AppLog("cosAlt = %f", cosAlt);

    double sinAz =
        -(Math::Sin(radInDegree*(ha))*Math::Cos(radInDegree*(DED)))/cosAlt;
	AppLog("sinAz = %f", sinAz);

    double cosAz =
        (Math::Sin(radInDegree*DED)-Math::Sin(radInDegree*(sky->getLatitude()))*sinAlt)/
            (Math::Cos(radInDegree*(sky->getLatitude()))*cosAlt);
	AppLog("cosAz = %f", cosAz);

    AppLog("Azimuth = %f", Math::Acos(cosAz));
    AppLog("Altitude = %f", Math::Acos(cosAlt));

    double R = sky->getRadius();
    double r = R * cosAlt;
    int top  = (int)((sky->getZenithY()) - r * cosAz);
    int left = (int)((sky->getZenithX()) + r * sinAz);

    //AppLog("Position on screen left %d top %d", left, top);

    if (magnitude < 1) {
    	sky->getCanvas()->FillEllipse(Color::COLOR_WHITE, Rectangle(left,top,3,3));
    } else if (magnitude < 2) {
    	sky->getCanvas()->FillEllipse(Color::COLOR_WHITE, Rectangle(left,top,2,2));
    } else {
    	sky->getCanvas()->FillEllipse(Color::COLOR_WHITE, Rectangle(left,top,1,1));
    }

    sky->getCanvas()->Show();

//    PositionTrig positionTrig = new PositionTrig();
//    positionTrig.setCosAlt(cosAlt);
//    positionTrig.setSinAlt(sinAlt);
//    positionTrig.setCosAz(cosAz);
//    positionTrig.setSinAz(sinAz);
//    return positionTrig;
//}

}

void SkyObject::setDED(float DED)
{
    this->DED = DED;
}

void SkyObject::setRAH(float RAH)
{
    this->RAH = RAH;
}


