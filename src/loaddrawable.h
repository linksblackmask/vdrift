/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _LOADDRAWABLE_H
#define _LOADDRAWABLE_H

#include "scenenode.h"

class ContentManager;
class PTree;

// Load drawable functor, returns false on error.
//
// [foo]
// texture = diff.png, spec.png, norm.png		#required
// mesh = model.joe								#required
// position = 0.736, 1.14, -0.47				#optional relative to parent
// rotation = 0, 0, 30							#optional relative to parent
// scale = -1, 1, 1								#optional
// color = 0.8, 0.1, 0.1						#optional color rgb
// draw = transparent							#optional type (transparent, emissive)
//
struct LoadDrawable
{
	const std::string & path;
	const int anisotropy;
	ContentManager & content;
	std::list<std::tr1::shared_ptr<MODEL> > & modellist;
	std::ostream & error;

	LoadDrawable(
		const std::string & path,
		const int anisotropy,
		ContentManager & content,
		std::list<std::tr1::shared_ptr<MODEL> > & modellist,
		std::ostream & error);

	bool operator()(
		const PTree & cfg,
		SCENENODE & topnode,
		keyed_container<SCENENODE>::handle * nodehandle = 0,
		keyed_container<DRAWABLE>::handle * drawhandle = 0);

	bool operator()(
		const std::string & meshname,
		const std::vector<std::string> & texname,
		const PTree & cfg,
		SCENENODE & topnode,
		keyed_container<SCENENODE>::handle * nodeptr = 0,
		keyed_container<DRAWABLE>::handle * drawptr = 0);
};

#endif // _LOADDRAWABLE_H
