/*
 * Constants.h
 *
 *  Created on: 03.06.2014
 *      Author: ddreval
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <FGraphics.h>

using Osp::Graphics::Color;

class Constants {
public:
	Constants();
	virtual ~Constants();
};

static const int EVENT_INFO_CLOSED = 80;
static const int EVENT_HELP_CLOSED = 81;

static const Osp::Graphics::Color COLOR_SKY(Color::COLOR_BLACK);
static const Osp::Graphics::Color COLOR_BUTTON_BKG(Color::COLOR_BLACK);
static const Osp::Graphics::Color COLOR_BUTTON_TEXT(Color::COLOR_WHITE);
static const Osp::Graphics::Color COLOR_FORM_BKG(Color(0, 100, 120));
static const Osp::Graphics::Color COLOR_FORM_TEXT(Color(150, 200, 200));
static const Osp::Graphics::Color COLOR_CONSTEL_BORDER(Color(150, 200, 200));
static const Osp::Graphics::Color COLOR_BRIGHT_STAR(Color::COLOR_WHITE);
static const Osp::Graphics::Color COLOR_DIM_STAR(Color(32, 32, 32));
static const Osp::Graphics::Color COLOR_DIMMEST_STAR(Color(8, 8, 8));


#endif /* CONSTANTS_H_ */