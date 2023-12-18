#ifndef __Matrix_H__
#define __Matrix_H__
#include <math.h> 
#include "Point.h"

namespace laya
{
	class Matrix 
	{
	public:
		static Matrix EMPTY;

		/*static TEMP: Matrix = new Matrix();

		static _createFun: Function | null = null;*/

		float a;
		float b;
		float c;
		float d;
		float tx;
		float ty;
		//bool _bTransform = false;

		Matrix()
		{
			this->a = this->d = 1;
			this->b = this->tx = this->ty = this->c = 0;
			//this->_bTransform = false;
		}

		Matrix(float a, float b, float c, float d, float tx, float ty/*, float nums*/)
		{
			/*if (Matrix._createFun != null) {
				return Matrix._createFun(a, b, c, d, tx, ty, nums);
			}*/
			this->a = a;
			this->b = b;
			this->c = c;
			this->d = d;
			this->tx = tx;
			this->ty = ty;
			//this->_checkTransform();
		}
		Matrix& identity()
		{
			this->a = this->d = 1;
			this->b = this->tx = this->ty = this->c = 0;
			//this->_bTransform = false;
			return *this;
		}
		/*bool _checkTransform()//TODO
		{
			return this->_bTransform = (this->a != 1 || this->b != 0 || this->c != 0 || this->d != 1);
		}*/
		Matrix& setTo(float a, float b, float c, float d, float tx, float ty)
		{
			this->a = a, this->b = b, this->c = c, this->d = d, this->tx = tx, this->ty = ty;
			return *this;
		}

		static void mul(Matrix& m1, Matrix& m2, Matrix& out)
		{
			float aa = m1.a, ab = m1.b, ac = m1.c, ad = m1.d, atx = m1.tx, aty = m1.ty;
			float ba = m2.a, bb = m2.b, bc = m2.c, bd = m2.d, btx = m2.tx, bty = m2.ty;
			//if (bb != 0 || bc != 0) 
			{
				out.a = aa * ba + ab * bc;
				out.b = aa * bb + ab * bd;
				out.c = ac * ba + ad * bc;
				out.d = ac * bb + ad * bd;
				out.tx = ba * atx + bc * aty + btx;
				out.ty = bb * atx + bd * aty + bty;
			}
			/*else
			{
				out.a = aa * ba;
				out.b = ab * bd;
				out.c = ac * ba;
				out.d = ad * bd;
				out.tx = ba * atx + btx;
				out.ty = bd * aty + bty;
			}*/
		}
		void rotateEx(float angle)
		{
			float fcos = cos(angle);
			float fsin = sin(angle);
			float ba = this->a, bb  = this->b, bc = this->c, bd = this->d;
			//if (bb != = 0 || bc != = 0) {
				this->a = fcos * ba + fsin * bc;
				this->b = fcos * bb + fsin * bd;
				this->c = -fsin * ba + fcos * bc;
				this->d = -fsin * bb + fcos * bd;
			/*}
			else {
				this.a = cos * ba;
				this.b = sin * bd;
				this.c = -sin * ba;
				this.d = cos * bd;
			}
			this._bTransform = true;*/
		}
		void scaleEx(float x, float y)
		{
			float ba = this->a, bb = this->b, bc = this->c, bd = this->d;
			//if (bb != = 0 || bc != = 0) {
				this->a = x * ba;
				this->b = x * bb;
				this->c = y * bc;
				this->d = y * bd;
			/*}
			else {
				this.a = x * ba;
				this.b = 0 * bd;
				this.c = 0 * ba;
				this.d = y * bd;
			}
			this._bTransform = true;*/
		}
		float getScaleX()
		{
			return this->b == 0 ? this->a : sqrt(this->a * this->a + this->b * this->b);
		}
		float getScaleY()
		{
			return this->c == 0 ? this->d : sqrt(this->c * this->c + this->d * this->d);
		}
		Matrix& copyTo(Matrix& dec)
		{
			dec.a = this->a;
			dec.b = this->b;
			dec.c = this->c;
			dec.d = this->d;
			dec.tx = this->tx;
			dec.ty = this->ty;
			//dec._bTransform = this._bTransform;
			return dec;
		}
		Matrix& invert()
		{
			float a1 = this->a;
			float b1 = this->b;
			float c1 = this->c;
			float d1 = this->d;
			float tx1 = this->tx;
			float n = a1 * d1 - b1 * c1;
			this->a = d1 / n;
			this->b = -b1 / n;
			this->c = -c1 / n;
			this->d = a1 / n;
			this->tx = (c1 * this->ty - d1 * tx1) / n;
			this->ty = -(a1 * this->ty - b1 * tx1) / n;
			return *this;
		}
		Point& transformPoint(Point& out)
		{
			out.x = this->a * out.x + this->c * out.y + this->tx;
			out.y = this->b * out.x + this->d * out.y + this->ty;
			return out;
		}

		Matrix& translate(float x, float y)
		{
			this->tx += x;
			this->ty += y;
			return *this;
		}
	};
}
#endif

