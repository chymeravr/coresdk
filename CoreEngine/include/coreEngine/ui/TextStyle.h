#ifndef COREENGINE_TEXTSTYLE_H
#define COREENGINE_TEXTSTYLE_H

namespace cl{
	class TextStyle{
	public:
		float lineHeight;
		float characterWidth;
		float scale = 1.0f;
		int fontSize = 16;
		CL_Vec4 color = CL_Vec4(1.0, 1.0, 1.0, 1.0);
	};
}

#endif //COREENGINE_TEXTSTYLE_H