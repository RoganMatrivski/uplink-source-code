#include "HD_UI_Container.h"
#include "../HD_Screen.h"

HD_UI_Container::HD_UI_Container()
{

}

//============================
// Public Functions
//============================

HD_UI_Container::HD_UI_Container(char* name, int nIndex, float fX, float fY, HD_UI_Container *newParent)
{
	setObjectProperties(name, fX, fY, 0.0f, 0.0f, newParent, nIndex);
}

HD_UI_Container::~HD_UI_Container()
{

}

void HD_UI_Container::Create()
{
	//To be replaced by derived objects
}

void HD_UI_Container::Update()
{
	//If it' a child, update normally.
	//If not, it's the root object.
	if (parent)
	{
		HD_UI_Object::Update();
		setMouseOver();
	}
	else if (parent == NULL && isVisible)
	{
		tweensContainer.step(HDScreen->deltaTime);

		globalX = x;
		globalY = y;

		globalScaleX = scaleX;
		globalScaleY = scaleY;

		drawWidth = width * globalScaleX;
		drawHeight = height * globalScaleY;

		drawAlpha = alpha;
		if (drawAlpha > 1.0f) drawAlpha = 1.0f;
		else if (drawAlpha <= 0.0f) drawAlpha = 0.0f;

		if (alpha <= 0.0f) visible = false;

		setMouseOver();

		redraw = true;
	}

	//update children
	for (unsigned int ii = 0; ii < children.size(); ii++)
	{
		children[ii]->Update();
	}
}

void HD_UI_Container::Draw()
{
	for (unsigned int ii = 0; ii < children.size(); ii++)
	{
		children[ii]->Draw();
	}
}

void HD_UI_Container::Clear()
{
	//base clear function
	removeAllChildren();

	delete this;
}

bool HD_UI_Container::setMouseOver()
{
	int mX = HDScreen->mouse->GetState()->x;
	int mY = HDScreen->mouse->GetState()->y;

	if (!isVisible)
	{
		if (HDScreen->mouse->GetTarget() == this)
			HDScreen->mouse->SetTarget(NULL);
		return false;
	}

	if (mX > globalX && mX < globalX + width &&
		mY > globalY && mY < globalY + height)
	{
		//the mouse is over this
		//set it's target to this
		//if it succeeds, we have contact!
		if (HDScreen->mouse->SetTarget(this))
			return true;
		else
			return false;

	}
	else
	{
		//the mouse is out
		//reset the target if it has been set by this
		if (HDScreen->mouse->GetTarget() == this)
			HDScreen->mouse->SetTarget(NULL);
		return false;
	}
}

bool HD_UI_Container::isMouseTarget()
{
	HD_UI_Object *mouseTarget = HDScreen->mouse->GetTarget();

	if (mouseTarget == this)
		return true;
	else
		return false;
}

void HD_UI_Container::addChild(HD_UI_Object *child)
{
	children.push_back(child);

	//Set the index
	child->index = children.size() - 1;
	child->gIndex = child->index + this->gIndex;
}

void HD_UI_Container::addChildAt(HD_UI_Object *child, unsigned index)
{
	//add the child directly at index
	std::vector<HD_UI_Object*>::iterator it = children.begin();
	children.insert(it + index, child);

	//child->setParent(this);
	child->index = index;
	child->gIndex = child->index + this->gIndex;
}

//Child retreaval!
HD_UI_Object* HD_UI_Container::getChildByIndex(unsigned int index)
{
	HD_UI_Object *child = children[index];

	return child;
}

HD_UI_Object* HD_UI_Container::getChildByName(const char *name)
{
	HD_UI_Object *child = NULL;

	for (unsigned ii = 0; ii < children.size(); ii++)
	{
		if (strcmp(children[ii]->name, name) == 0)
		{
			child = children[ii];
			break;
		}
	}

	return child;
}

//Child removal!
void HD_UI_Container::removeChild(HD_UI_Object *child)
{
	removeChildFromIndex(child->index);
}

void HD_UI_Container::removeChildByName(char *childName)
{
	//Search for the child with name "childName", then erase it
	std::vector<HD_UI_Object*>::iterator it;
	for (unsigned ii = 0; ii < children.size(); ii++)
	{
		if (strcmp(children[ii]->name, childName) == 0)
		{
			children.erase(it + ii);
			break;
		}
	}
}

void HD_UI_Container::removeChildFromIndex(unsigned int index)
{
	//Remove child at index
	std::vector<HD_UI_Object*>::iterator it = children.begin();
	children[index]->Clear();
	children.erase(it + index);
}

void HD_UI_Container::removeAllChildren()
{
	//Remove all child objects
	//Only called when the layout is destroyed
	for (unsigned ii = 0; ii < children.size(); ii++)
	{
		children[ii]->Clear();
	}

	children.clear();
}