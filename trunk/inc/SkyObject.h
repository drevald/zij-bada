/*
 * SkyObject.h
 *
 *  Created on: 17.02.2012
 *      Author: ddreval
 */

#ifndef SKYOBJECT_H_
#define SKYOBJECT_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>
#include <FApp.h>
#include <FUi.h>
#include <FSystem.h>
#include <FLocations.h>
#include <FLocales.h>
#include <FIo.h>

#include "Sky.h"


class SkyObject
{

public:

	SkyObject();

	virtual ~SkyObject();

	// Sets and gets name
	void setName(Osp::Base::String& name);
	Osp::Base::String& getName(void);

	// Sets true if object in northern hemisphere
	void setSign(bool isNorthern);

	// Sets visual magnitude, i.e. brightness of object
	void setMagnitude(float magnitude);
	float getMagnitude(void);

	// Gets and sets right ascension in hours in decimal format
	float getRAH(void);
    void setRAH(float RAH);

	// Gets and sets declination in degrees in decimal format
	float getDED(void);
    void setDED(float DED);

	// Whether object is in Northern hemisphere?
	bool isNorthern(void);

	// Draws object on canvas
	void draw(Sky* sky);

private:
    Osp::Base::String name;
    float RAH;
    float DED;
    float magnitude;
    bool sign;

};

#endif /* SKYOBJECT_H_ */
