#include "ImageLabelButton.h"
#include "Image.h"
#include "Label.h"
#include "ScaleImage.h"
#include "Box.h"
#include "HBox.h"

TINA_NS_BEGIN
ImageLabelButton* ImageLabelButton::create()
{
	auto _ref = new ImageLabelButton();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

bool ImageLabelButton::init()
{
	auto _upState = Box::create();
	{
		m_bg = ScaleImage::create();
		_upState->addWidget(m_bg);

	}
	return true;
}
TINA_NS_END