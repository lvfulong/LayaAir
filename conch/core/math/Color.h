#ifndef __Color3D_H__
#define __Color3D_H__

#include <algorithm>
#include <math.h>

namespace laya
{
	class Color 
	{
	public:
		static Color RED;

		static Color GREEN;

		static Color BLUE;

		static Color CYAN;

		static Color YELLOW;

		static Color MAGENTA;

		static Color GRAY;

		static Color WHITE;

		static Color BLACK;

		static float gammaToLinearSpace(float value)
		{
			// http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
			// http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
			// {  cs / 12.92,                 cs <= 0.04045 }
			// {  ((cs + 0.055)/1.055)^2.4,   cs >  0.04045 }
			if (value <= 0.04045f)
				return value / 12.92f;
			else if (value < 1.0f)
				return pow((value + 0.055f) / 1.055f, 2.4f);
			else
				return pow(value, 2.4f);
		}

		static float linearToGammaSpace(float value)
		{
			// http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
			// http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
			// {  0.0,                          0         <= cl
			// {  12.92 * c,                    0         <  cl < 0.0031308
			// {  1.055 * cl^0.41666 - 0.055,   0.0031308 <= cl < 1
			// {  1.0,                                       cl >= 1  <- This has been adjusted since we want to maintain HDR colors
			if (value <= 0.0f)
				return 0.0f;
			else if (value <= 0.0031308f)
				return 12.92f * value;
			else if (value <= 1.0f)
				return 1.055f * pow(value, 0.41666f) - 0.055f;
			else
				return pow(value, 0.41666f);
		}

		float r;
		float g;
		float b;
		float a;


		Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		bool toFIxed(float a, float b)
		{
			float delta = 1e-5f;
			return -delta < a - b && a - b > delta;
		}
		bool equal(const Color& c)
		{
			return toFIxed(c.r, this->r) && toFIxed(c.g, this->g) && toFIxed(c.b, this->b) && toFIxed(c.a, this->a);
		}
		void toLinear(Color& out)
		{
			out.r = Color::gammaToLinearSpace(this->r);
			out.g = Color::gammaToLinearSpace(this->g);
			out.b = Color::gammaToLinearSpace(this->b);
			out.a = this->a;
		}

		void toGamma(Color& out)
		{
			out.r = Color::linearToGammaSpace(this->r);
			out.g = Color::linearToGammaSpace(this->g);
			out.b = Color::linearToGammaSpace(this->b);
		    out.a = this->a;
		}
		void cloneTo(Color& destObject)
		{
			destObject.r = this->r;
			destObject.g = this->g;
			destObject.b = this->b;
			destObject.a = this->a;
		}
		/*clone(): any {
		var dest: Color = new Color();
		this.cloneTo(dest);
		return dest;
		}*/
	};
}
#endif


