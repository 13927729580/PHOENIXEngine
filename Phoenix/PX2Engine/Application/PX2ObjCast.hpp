// PX2ObjCast.hpp

#ifndef PX2OBJCAST_HPP
#define PX2OBJCAST_HPP

#include "PX2Core.hpp"
#include "PX2Node.hpp"
#include "PX2ScriptController.hpp"
#include "PX2EffectableController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Cast
	{
	public:
		static Controller *ToController(Object *obj);
		static Controlledable *ToControlledable(Object *obj);
		static Movable *ToMovable(Object *obj);
		static Node *ToNode(Object *obj);
		static EffectableController *ToEffectController(Object *obj);
		static Object *ToO(void *obj);
		static ScriptController *ToSC(void *obj);
	};

}

#endif