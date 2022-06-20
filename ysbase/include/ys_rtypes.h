#pragma once
#include "ys_rbase.h"

namespace ys
{
	R_STRUCT_BEGIN(RSize)
		R_INT(cx)
		R_INT(cy)
		R_STRUCT_END;

	R_STRUCT_BEGIN(RPoint)
		R_INT(x)
		R_INT(y)
		R_STRUCT_END;

	R_STRUCT_BEGIN(RRect)
		R_INT(left)
		R_INT(top)
		R_INT(right)
		R_INT(bottom)
		R_STRUCT_END;

	//R_STRUCT_DERIVE ��һ���������������ڶ��������Ǽ̳еĻ���
	R_STRUCT_DERIVE(RRectD, RRect)
		R_INT(type)
		R_STRUCT_END;

	R_STRUCT_BEGIN(RBox)
		R_INT(x)
		R_INT(y)
		R_INT(width)
		R_INT(height)
		R_STRUCT_END;

	R_STRUCT_DERIVE(RBoxD, RBox)
		R_INT(type)
		R_STRUCT_END;


	R_STRUCT_BEGIN(RParamType)
		R_STRING(ptype)
		R_STRUCT_END;
}