/*
 * SkyObject.cpp
 *
 *  Created on: 17.02.2012
 *      Author: ddreval
 */

#include "SkyObject.h"

using namespace Osp::Base;
using namespace Osp::Base::Utility;
using namespace Osp::Graphics;

SkyObject::SkyObject() {

}

SkyObject::~SkyObject() {

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
SkyObject::setMagnitude(float pMagnitude) {
	this -> magnitude = pMagnitude;
}

void
SkyObject::setSign(bool isNorthern) {
	sign = isNorthern;
}

void
SkyObject::setName(Osp::Base::String& aName) {
	name = aName;
}

Osp::Base::String&
SkyObject::getName(void) {
	return name;
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
SkyObject::setDED(float DED) {
    this->DED = DED;
}

void
SkyObject::setRAH(float RAH) {
    this->RAH = RAH;
}

void
SkyObject::Print(void) {
	if (name==null) {
		AppLog("NAME IS NULL");
	}
	AppLog("Object name=%S, magnitude=%f, ra=%f, de=%f, north=%d", name.GetPointer(), magnitude, RAH, DED, sign);
}

void
SkyObject::Draw(SkyCanvas* skyCanvas) {
	Canvas* canvas;
	canvas = skyCanvas -> GetBufferedCanvas(1);
	DrawCanvas(canvas);
	canvas = skyCanvas -> GetBufferedCanvas(2);
	DrawCanvas(canvas);
	canvas = skyCanvas -> GetBufferedCanvas(4);
	DrawCanvas(canvas);
	canvas = skyCanvas -> GetBufferedCanvas(8);
	DrawCanvas(canvas);
}

void
SkyObject::DrawCanvas(Canvas* canvas) {

	Point* point = Projector::
			GetProjection(RAH, DED, sign,
					canvas->GetBounds().width,
					canvas->GetBounds().height);

	if (point == null) {
		return;
	}

	int diameter = (int)(7 - magnitude);

		canvas ->FillEllipse(Color::COLOR_WHITE,
			Rectangle(point->x, point->y, diameter, diameter));


}



