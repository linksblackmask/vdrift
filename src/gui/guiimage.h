#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "gui/guiwidget.h"
#include "mathvector.h"
#include "scenenode.h"
#include "vertexarray.h"

class TEXTURE;

class GUIIMAGE : public GUIWIDGET
{
public:
	GUIIMAGE() {};

	~GUIIMAGE() {};

	virtual void SetAlpha(SCENENODE & node, float newalpha);

	virtual void SetVisible(SCENENODE & node, bool newvis);

	const MATHVECTOR <float, 2> & GetCorner1() const {return corner1;}

	const MATHVECTOR <float, 2> & GetCorner2() const {return corner2;}

	void SetupDrawable(
		SCENENODE & scene,
		const std::tr1::shared_ptr<TEXTURE> teximage,
		float x, float y, float w, float h,
		int order = 0,
		bool button_mode = false,
		float screenhwratio = 1.0);

private:
	MATHVECTOR <float, 2> corner1;
	MATHVECTOR <float, 2> corner2;
	VERTEXARRAY varray;
	keyed_container <DRAWABLE>::handle draw;

	DRAWABLE & GetDrawable(SCENENODE & scene)
	{
		return scene.GetDrawlist().twodim.get(draw);
	}
};

#endif