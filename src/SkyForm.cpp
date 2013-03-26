#include "SkyForm.h"

using namespace Osp::Base;
using namespace Osp::Base::Utility;
using namespace Osp::Base::Runtime;
using namespace Osp::Base::Collection;
using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Locales;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Locations;
using namespace Osp::Io;

SkyForm::SkyForm() {
	zoom = 1;
	shiftX = 0;
	shiftY = 0;
}

SkyForm::~SkyForm(void)
{
}

bool
SkyForm::Initialize()
{
	Construct(L"SKY_FORM");
	return true;
}

result
SkyForm::OnInitializing(void)
{
	result r = E_SUCCESS;
	AddTouchModeChangedEventListener(*this);
	AddTouchEventListener(*this);

	__pButtonZoomIn = static_cast<Button *>(GetControl(L"IDC_BUTTON_ZOOM_IN"));
	if (__pButtonZoomIn != null)
	{
		__pButtonZoomIn->SetActionId(ID_BUTTON_ZOOM_IN);
		__pButtonZoomIn->AddActionEventListener(*this);
	}

	__pButtonZoomOut = static_cast<Button *>(GetControl(L"IDC_BUTTON_ZOOM_OUT"));
	if (__pButtonZoomOut != null)
	{
		__pButtonZoomOut->SetActionId(ID_BUTTON_ZOOM_OUT);
		__pButtonZoomOut->AddActionEventListener(*this);
	}

	__pButtonRefresh = static_cast<Button *>(GetControl(L"IDC_BUTTON_REFRESH"));
	if (__pButtonRefresh != null)
	{
		__pButtonRefresh->SetActionId(ID_BUTTON_REFRESH);
		__pButtonRefresh->AddActionEventListener(*this);
	}

	__pButtonConstellations = static_cast<Button *>(GetControl(L"IDC_BUTTON_CONSTELLATIONS"));
	if (__pButtonConstellations != null)
	{
		__pButtonConstellations->SetActionId(1);
		__pButtonConstellations->AddActionEventListener(*this);
		__pButtonConstellations->SetActionId(ID_BUTTON_CONSTELLATIONS);
		__pButtonConstellations->AddActionEventListener(*this);
	}

	//IDC_BUTTON_CONSTELLATIONS

	__pLabelLocation = static_cast<Label *>(GetControl(L"IDC_LABEL1"));
	__pZoomLabel = static_cast<Label *>(GetControl("IDC_LABEL_ZOOM"));

	Control* control = GetControl(L"SKY_FORM");
	destWidth = control -> GetWidth();
	destHeight = control -> GetHeight();

	Button *pButton1 = static_cast<Button *>(GetControl("IDC_BUTTON1"));  
	if (pButton1)
	{
		pButton1->SetActionId(1);
		pButton1->AddActionEventListener(*this);
	}
	Button *pButton_refresh = static_cast<Button *>(GetControl("IDC_BUTTON_REFRESH"));  
	if (pButton_refresh)
	{
		pButton_refresh->SetActionId(3);
		pButton_refresh->AddActionEventListener(*this);
	}

	return r;
}

result
SkyForm::OnTerminating(void)
{
	result r = E_SUCCESS;
	return r;
}

void
SkyForm::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_BUTTON_ZOOM_IN: {
			if (zoom < MAX_ZOOM) {
				zoom = zoom * 2;
				String zoomStr;
				zoomStr.Format(5, L"x %d", zoom);
				__pZoomLabel -> SetText(zoomStr);
				__pZoomLabel -> Draw();
				AppLog("Zooming label value is %S", zoomStr.GetPointer());
				Update();
			}
		}
		break;
	case ID_BUTTON_ZOOM_OUT: {
			if (zoom > MIN_ZOOM) {
				zoom = (int)(zoom/2);
				if (zoom > 1) {
					String zoomStr;
					zoomStr.Format(5, L"x %d", zoom);
					__pZoomLabel -> SetText(zoomStr);
					__pZoomLabel -> Draw();
					AppLog("Zooming label value is %S", zoomStr.GetPointer());
				} else {
					__pZoomLabel -> SetText("");
					__pZoomLabel -> Draw();
				}
				Update();
			}
		}
		break;
	case ID_BUTTON_CONSTELLATIONS: {
		Osp::App::Application::GetInstance() -> SendUserEvent(SELECT_CONSTELLATION, null);
	}
			break;
	default:
		break;
	}
}

void
SkyForm::OnLocationUpdated(Osp::Locations::Location& location) {
	AppLog("Location Updated\n");
	const QualifiedCoordinates* coordinates = location.GetQualifiedCoordinates();
	String str;
	if (coordinates != 0) {
		AppLog("Coordinates taken\n");
		String locationStr;
		locationStr.Format(21, L"%S %S",
				(DegreeToGrad(coordinates->GetLatitude(), "N", "S")->GetPointer()),
				(DegreeToGrad(coordinates->GetLongitude(), "E", "W")->GetPointer()));
		__pLabelLocation->SetText(locationStr.GetPointer());
		__pLabelLocation->Draw();
		sky->draw();
	} else {
		AppLog("#");
	}
}


// Converts from decimal format -12,34456789 to N12°34'56
String*
SkyForm::DegreeToGrad(float angle, const char* posPrefix, const char* negPrefix) {
	AppLog("Converting degrees to grad");
	const char* prefix = angle < 0 ? negPrefix : posPrefix;
	AppLog("orig: %f", angle);
	float latAbs = angle * 1000000;
	float deg = Math::Abs(Math::Floor(latAbs/1000000));
	AppLog("deg: %f", deg);
	float min = Math::Floor(((latAbs/1000000) - Math::Floor(latAbs/1000000))*60);
	AppLog("min: %f", min);
	float sec = Math::Floor(((((latAbs/1000000) - Math::Floor(latAbs/1000000))*60) - Math::Floor(((latAbs/1000000) - Math::Floor(latAbs/1000000))*60))*100000)*60/100000;
	AppLog("sec: %f", sec);
	String* result = new String();
	result->Format(10, L"%s%d°%d\'%d\"", prefix, (int)deg, (int)min, (int)sec);
	return result;
}

void
SkyForm::updateConstList(IList* list) {
//	__pConstForm -> UpdateConstellationList(list);
}

void
SkyForm::Update(void) {

	Osp::Graphics::Canvas* canvas;
	Control* control = GetControl(L"SKY_FORM");
	canvas = control -> GetCanvasN();
	canvas -> SetBackgroundColor(Color::COLOR_RED);

	canvas -> Clear();
	Osp::Graphics::Rectangle rect = canvas -> GetBounds();						//Getting size of current canvas


	//Image should not be moved outside screen boundaries, so fixing shift value
	int zoomedShiftX = zoom == 1 ? 0 : (int)(shiftX * zoom);
	int zoomedShiftY = zoom == 1 ? 0 : (int)(shiftY * zoom);
	AppLog("Zoomed shift to %d, %d", zoomedShiftX, zoomedShiftY);
	zoomedShiftX += destWidth*(zoom-1)/2;
	zoomedShiftY += destHeight*(zoom-1)/2;
	zoomedShiftX = zoomedShiftX < 0 ? 0 : zoomedShiftX;
	zoomedShiftX = zoomedShiftX > (destWidth * (zoom -1)) ? (destWidth * (zoom -1)) : zoomedShiftX;
	zoomedShiftY = zoomedShiftY < 0 ? 0 : zoomedShiftY;
	zoomedShiftY = zoomedShiftY > (destHeight * (zoom-1)) ? (destHeight * (zoom-1)) : zoomedShiftY;
	rect.SetPosition(zoomedShiftX, zoomedShiftY);
	AppLog("Rect %d %d %d %d", rect.GetTopLeft().x, rect.GetTopLeft().y, rect.GetBottomRight().x, rect.GetBottomRight().y);

	//Getting buffered canvas for given zoom
	Osp::Graphics::Canvas* bufferedCanvas = SkyCanvas::GetStarCanvas(zoom);

	//Setting start point as top left
	Osp::Graphics::Point point(0, 0);

	canvas->Copy(point, *bufferedCanvas, rect);

	String* constAcronym = SkyCanvas::GetSelectedConstellation();
	if (constAcronym != null) {
		ConstellationBuilder::DrawCanvas(canvas, zoom, zoomedShiftX, zoomedShiftY);

		//Showing name of selected constellation
		String constName;
		AppResource* appResource = Osp::App::Application::GetInstance()->GetAppResource();
		appResource ->GetString(*constAcronym, constName);
		__pLabelLocation -> SetText(constName);
		__pLabelLocation -> Draw();


	} else {
		__pLabelLocation -> SetText("");
		AppLog("Constellation is not selected");
	}

	canvas -> Show();
}

void
SkyForm::OnTouchDoublePressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo) { }

void
SkyForm::OnTouchFocusIn(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo){ }

void
SkyForm::OnTouchFocusOut(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo){ }

void
SkyForm::OnTouchLongPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo){ }

void
SkyForm::OnTouchMoved(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo) {
	AppLog("OnTouchMoved - started(%d,%d) now(%d,%d)",
			touchInfo.GetStartPosition().x,
			touchInfo.GetStartPosition().y,
			touchInfo.GetCurrentPosition().x,
			touchInfo.GetCurrentPosition().y);
}

void
SkyForm::OnTouchPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo) { }

void
SkyForm::OnTouchReleased(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo) {
	if (zoom > 1) {
		AppLog("OnTouchReleased - started(%d,%d) now(%d,%d)",
				touchInfo.GetStartPosition().x,
				touchInfo.GetStartPosition().y,
				touchInfo.GetCurrentPosition().x,
				touchInfo.GetCurrentPosition().y);
		shiftX -= (touchInfo.GetCurrentPosition().x - touchInfo.GetStartPosition().x)/zoom;
		shiftY -= (touchInfo.GetCurrentPosition().y - touchInfo.GetStartPosition().y)/zoom;
		Update();
	}
}

void
SkyForm::OnTouchModeChanged(const Osp::Ui::Control& source, bool isInTouchMode)
{
	AppLog("OnTouchModeChanged");
}
