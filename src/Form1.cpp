#include "Form1.h"
#include "SkyObject.h"

//using namespace Osp::Base;
//using namespace Osp::Ui;
//using namespace Osp::Ui::Controls;

using namespace Osp::Base;
using namespace Osp::Base::Utility;
using namespace Osp::Base::Runtime;
using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Locales;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Locations;
using namespace Osp::Io;

Form1::Form1(void)
{
}

Form1::~Form1(void)
{
}

bool
Form1::Initialize()
{
	Construct(L"IDF_FORM1");
	return true;
}

result
Form1::OnInitializing(void)
{
	result r = E_SUCCESS;

	__pButtonOk = static_cast<Button *>(GetControl(L"IDC_BUTTON_OK"));
	if (__pButtonOk != null)
	{
		__pButtonOk->SetActionId(ID_BUTTON_OK);
		__pButtonOk->AddActionEventListener(*this);
	}

	Button *pButton_gps = static_cast<Button *>(GetControl("IDC_BUTTON_GPS"));  
	if (pButton_gps)
	{
		pButton_gps->SetActionId(102);
		pButton_gps->AddActionEventListener(*this);
	}

	__pLabel = static_cast<Label *>(GetControl("IDC_LABEL1"));
	__pLabel -> SetText("Initialized");
	__pLabel -> SetText("Again Initialized");

	Button *pButton1 = static_cast<Button *>(GetControl("IDC_BUTTON_STARS"));
	if (pButton1)
	{
		pButton1->SetActionId(103);
		pButton1->AddActionEventListener(*this);
	}
	return r;
}

result
Form1::OnTerminating(void)
{
	result r = E_SUCCESS;
	return r;
}

void
Form1::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_BUTTON_OK:
		{
			AppLog("OK Button is clicked! \n");
			Canvas* __pCanvas;
			Control* control = GetControl(L"IDF_FORM1");
			Color COLOR_CHOST = Color(48,48,144);
			__pCanvas = control->GetCanvasN();
			__pCanvas->SetLineWidth(1);
			__pCanvas->SetForegroundColor(COLOR_CHOST);
			int x, y, width, height, r;
			int margin = 20;
			Application::GetInstance()->GetAppFrame()->GetFrame()->GetBounds(x, y, width, height);
			r = (Math::Min(width, height) - margin*2)/2;
			Rectangle rect (margin, 2*margin, 2*r, 2*r);
			__pCanvas->DrawEllipse(rect);
			__pCanvas->Show();
			__pLabel->SetText("OK\n");
			__pLabel->RequestRedraw();
		}
		break;
	case ID_BUTTON_GPS:
		{
			AppLog("GPS Button is clicked! \n");
			locProvider.Construct(LOC_METHOD_HYBRID);
			AppLog("Location Provider! \n");
			locProvider.RequestLocationUpdates(*this, 5, true);
		}
		break;
	case ID_BUTTON_STARS:
		{
			AppLog("STARS Button is clicked! \n");
			IterateStars();
		}
		break;
	default:
		break;
	}
}

void
Form1::OnLocationUpdated(Osp::Locations::Location& location) {
	AppLog("Location Updated\n");
	const QualifiedCoordinates* coordinates = location.GetQualifiedCoordinates();
	AppLog("Coordinates taken\n");
	AppLog("Label\n");
	String str;
	if (coordinates != 0) {
		double latd = coordinates->GetLatitude();
		AppLog("Latitude (double) taken\n");
		AppLog("Latitude (float) taken\n");
		Log("Latitude = ",(float)(coordinates->GetLatitude()));
		Log("Longitude = ",(float)(coordinates->GetLongitude()));
		Log("SLT = ", GetLocalSiderialTime((float)(coordinates->GetLongitude())));
		locProvider.CancelLocationUpdates();
	} else {
		LogSameLine("#");
	}
}

void
Form1::OnProviderStateChanged(Osp::Locations::LocProviderState newState) {
	AppLog("Location Provider state changed\n");
	Log("Location Provider state changed\n");
	if (newState == LOC_PROVIDER_AVAILABLE) {
		AppLog("LOC_PROVIDER_AVAILABLE");
		Log("LOC_PROVIDER_AVAILABLE");
	} else if (newState == LOC_PROVIDER_OUT_OF_SERVICE) {
		AppLog("LOC_PROVIDER_AVAILABLE");
		Log("LOC_PROVIDER_AVAILABLE");
	} else if (newState == LOC_PROVIDER_TEMPORARILY_UNAVAILABLE) {
		AppLog("LOC_PROVIDER_TEMPORARILY_UNAVAILABLE");
		Log("LOC_PROVIDER_TEMPORARILY_UNAVAILABLE");
	} else {
		AppLog("State unknown");
		Log("State unknown");
	}
}

float
Form1::GetLocalSiderialTime(float longitude) {
	TimeZone timeZone(60, L"Europe/London");
	DateTime* currTime = new DateTime();
	SystemTime::GetCurrentTime(*currTime);
	LogSameLine("currTime=");
	Log(currTime->ToString());
	calendar = Calendar::CreateInstanceN(timeZone, CALENDAR_GREGORIAN);
	calendar->SetTime(*currTime);
	float hrs = (calendar->GetTimeField(TIME_FIELD_HOUR_OF_DAY))-1;
	float minHrs = calendar->GetTimeField(TIME_FIELD_MINUTE)/60.0;
	float dayFract = (hrs + minHrs)/24.0;
	float dayNum = calendar->GetTimeField(TIME_FIELD_DAY_OF_YEAR);
	float year = calendar->GetTimeField(TIME_FIELD_YEAR);
	double daysSinceJ2000 = -1.5 + dayNum + (year-2000)*365 + (int)((year-2000)/4) + dayFract;
	double slt = 100.46 + 0.985647 * daysSinceJ2000 + longitude + 15*(hrs + minHrs);
	int sltInt = (int)(slt/360);
	float sltHrs = (slt-(360*sltInt))/15;
	Log("lstHours=",sltHrs);
	return sltHrs;
}

void
Form1::Log(const Osp::Base::String& aText, const float value) {
	String str;
	String text;
	text = aText;
	str = __pLabel->GetText();
	text.Append(value);
	str.Append(text);
	str.Append('\n');
	__pLabel->SetText(str);
	__pLabel->RequestRedraw();
}

void
Form1::Log(const Osp::Base::String& text) {
	String str;
	str = __pLabel->GetText();
	str.Append(text);
	str.Append('\n');
	__pLabel->SetText(str);
	__pLabel->RequestRedraw();
}

void
Form1::ClearLog(const Osp::Base::String& text) {
	__pLabel->SetText(text);
	__pLabel->RequestRedraw();
}

void
Form1::LogSameLine(const Osp::Base::String& text) {
	String str;
	str = __pLabel->GetText();
	str.Append(text);
	__pLabel->SetText(str);
	__pLabel->RequestRedraw();
}

void
Form1::IterateStars (void) {
	ClearLog("Start Reading Catalog");
	String fileName(L"/Home/catalog");
	File file;
	result r = file.Construct(fileName, L"r+");
	char buffer[1];
	String substr1;
	String substr2;
	String substr3;
	String line(L"");
	r = E_SUCCESS;
	SkyObject skyObj;
	int counter = 0;
	do {
		file.Read(buffer, 1);
		if(buffer[0]!='\n') {
			line.Append(buffer[0]);
		} else {
			if (line.GetLength()>108) {
				++counter;
				line.SubString(5, 9, substr1);
				substr1.Trim();
				skyObj.setName(substr1);
				line.SubString(25, 6, substr1);
				substr1.Trim();
				skyObj.setDraperName(substr1);
				line.SubString(102, 5, substr1);
				substr1.Trim();
				skyObj.setMagnitude(substr1);
				line.SubString(83, 1, substr1);
				skyObj.setSign(substr1.Equals("+", false));
				skyObj.setDraperName(substr1);
				line.SubString(75, 2, substr1);
				line.SubString(77, 2, substr2);
				line.SubString(79, 4, substr3);
				AppLog("%dRA=%lsh%lsm%lss", counter, substr1.GetPointer(),substr2.GetPointer(),substr3.GetPointer());
				skyObj.setRA(substr1, substr2, substr3);
				line.SubString(84, 2, substr1 );
				line.SubString(86, 2, substr2 );
				line.SubString(88, 2, substr3 );
				skyObj.setDE(substr1, substr2, substr3);
				AppLog("%dDE=%ls°%ls'%ls\"", counter, substr1.GetPointer(), substr2.GetPointer(), substr3.GetPointer());
				Log("RAH=", skyObj.getRAH());
				Log("DED=", skyObj.getDED());
			}
			line.Clear();
		}
		r = GetLastResult();
	} while (!IsFailed(r));
	Log("\nReading ended");

}


