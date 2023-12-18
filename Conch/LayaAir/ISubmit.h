#ifndef __ISubmit_H__
#define __ISubmit_H__

namespace laya
{
	enum class SubmitType
	{
		TwoDimension = 0,
		ThreeDimension,
	};
	class ISubmit
	{
	public:
		ISubmit(SubmitType type): m_type(type) {}
		virtual ~ISubmit() {}
		virtual int renderSubmit() = 0;
		virtual void recycle() = 0;
		SubmitType getType() { return m_type; }
	protected:
		SubmitType m_type;
	};
}
#endif //__ISubmit_H__
