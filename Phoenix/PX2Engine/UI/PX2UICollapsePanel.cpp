// PX2UICollapsePanel.cpp

#include "PX2UICollapsePanel.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, UIFrame, UICollapsePanel);
PX2_IMPLEMENT_STREAM(UICollapsePanel);
PX2_IMPLEMENT_FACTORY(UICollapsePanel);
//----------------------------------------------------------------------------
UICollapsePanel::UICollapsePanel() :
mIsNeedRecalCollpase(true),
mIsNeedUpdateContentPos(true)
{
	SetName("UICollapsePanel");

	float sliderSize = 10;

	mSlider = new0 UISlider();
	mSlider->LocalTransform.SetTranslateY(-1.0f);
	mSlider->SetDirectionType(UISlider::DT_VERTICALITY);
	mSlider->EnableAnchorLayout(true);
	mSlider->SetAnchorHor(1.0f, 1.0f);
	mSlider->SetAnchorParamHor(-sliderSize*0.5f, 0.0f);
	mSlider->SetAnchorVer(0.0f, 1.0f);
	mSlider->SetSize(sliderSize, 0.0f);
	mSlider->SetPivot(0.5f, 0.5f);
	AttachChild(mSlider);
	mSlider->SetSliderLength(200.0f);
	mSlider->SetContentFrame(mContentFrame);
	mSlider->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UICollapsePanel::_SliderCallback));

	mContentFrame = new0 UIFrame();
	AttachChild(mContentFrame);
	mContentFrame->SetAnchorHor(0.0f, 1.0f);
	mContentFrame->SetAnchorParamHor(0.0f, -sliderSize);
	mContentFrame->SetAnchorVer(1.0f, 1.0f);
	mContentFrame->SetPivot(0.5f, 1.0f);

	mSlider->SetPercent(0.0f);
}
//----------------------------------------------------------------------------
UICollapsePanel::~UICollapsePanel()
{
}
//----------------------------------------------------------------------------
void UICollapsePanel::RemoveAllItems()
{
	mContentFrame->DetachAllChildren();
	mCollpaseFrames.clear();
	mCollpaseFrameMap.clear();
}
//----------------------------------------------------------------------------
UICollapseItem *UICollapsePanel::AddItem(const std::string &name)
{
	UICollapseItem *item = new0 UICollapseItem();
	mContentFrame->AttachChild(item);
	item->SetName(name);
	item->SetCollapsedPanel(this);

	mCollpaseFrames.push_back(item);
	mCollpaseFrameMap[name] = item;

	mIsNeedRecalCollpase = true;
	mIsNeedUpdateContentPos = true;

	return item;
}
//----------------------------------------------------------------------------
void UICollapsePanel::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	mIsNeedUpdateContentPos = true;
}
//----------------------------------------------------------------------------
void UICollapsePanel::_MarkCollpaseRecal()
{
	mIsNeedRecalCollpase = true;
}
//----------------------------------------------------------------------------
void UICollapsePanel::_SliderCallback(UIFrame *frame, UICallType type)
{
	UISlider *slider = DynamicCast<UISlider>(frame);
	if (slider)
	{
		if (UICT_SLIDERCHANGED == type)
		{
			mIsNeedUpdateContentPos = true;
		}
	}
}
//----------------------------------------------------------------------------
void UICollapsePanel::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if (mIsNeedRecalCollpase)
	{
		_CalCollpase();

		mIsNeedUpdateContentPos = true;
	}

	if (mIsNeedUpdateContentPos)
	{
		_UpdateContentPos();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UICollapsePanel::_UpdateContentPos()
{
	float heightDist = mContentFrame->GetSize().Height - GetSize().Height;
	if (heightDist > 0.0f)
	{
		float downMove = heightDist * mSlider->GetPercent();
		mContentFrame->SetAnchorParamVer(downMove, 0.0f);
	}
	else
	{
		mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	}

	mIsNeedUpdateContentPos = false;
}
//----------------------------------------------------------------------------
void UICollapsePanel::_CalCollpase()
{
	mIsNeedRecalCollpase = false;

	float height = 0.0f;
	for (int i = 0; i < (int)mCollpaseFrames.size(); i++)
	{
		UICollapseItem *item = mCollpaseFrames[i];
		item->SetAnchorParamVer(-height, -height);

		float itemHeight = item->GetAllHeight();
		height += itemHeight;
	}

	mContentFrame->SetHeight(height);

	SetHeight(height);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UICollapsePanel::RegistProperties()
{
	UIFrame::RegistProperties();
	AddPropertyClass("UICollapsePanel");
}
//----------------------------------------------------------------------------
void UICollapsePanel::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// �־û�֧��
//----------------------------------------------------------------------------
UICollapsePanel::UICollapsePanel(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UICollapsePanel::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UICollapsePanel, source);
}
//----------------------------------------------------------------------------
void UICollapsePanel::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UICollapsePanel::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UICollapsePanel::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UICollapsePanel::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UICollapsePanel, target);
}
//----------------------------------------------------------------------------
int UICollapsePanel::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------