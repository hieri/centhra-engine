#ifndef CE_QUATERNION_H
#define CE_QUATERNION_H

//- Standard Library -
#include <cassert>
#include <math.h>

//- Centhra Engine -
#include <CE/Matrix4x4.h>

namespace ce
{
	/* 	Quaternion Class
	 * 	- Data structure holding a quaternion.
	 */
	template<class Type> class Quaternion
	{
		Type m_data[4];

	public:
		static Quaternion interpolate(Quaternion &start, Quaternion &end, float factor)
		{
			// calc cosine theta
			float cosom = start[1] * end[1] + start[2] * end[2] + start[3] * end[3] + start[0] * end[0];

			Quaternion _end = end;

			// adjust signs (if necessary)
			if(cosom < 0.0f)
			{
				cosom = -cosom;
				_end[0] = -end[0];	 // Reverse all signs
				_end[1] = -end[1];
				_end[2] = -end[2];
				_end[3] = -end[3];
			} 

			// Calculate coefficients
			float sclp, sclq;
			if((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
			{
				// Standard case (slerp)
				float omega, sinom;
				omega = acos(cosom); // extract theta from dot product's cos theta
				sinom = sin(omega);
				sclp = sin((1.0f - factor) * omega) / sinom;
				sclq = sin(factor * omega) / sinom;
			}
			else
			{
				// Very close, do linear interp (because it's faster)
				sclp = 1.0f - factor;
				sclq = factor;
			}

			Quaternion out;
			out[0] = sclp * start[0] + sclq * end[0];
			out[1] = sclp * start[1] + sclq * end[1];
			out[2] = sclp * start[2] + sclq * end[2];
			out[3] = sclp * start[3] + sclq * end[3];
			return out;
		}

		Quaternion()
		{
			m_data[0] = (Type)1;
			for(unsigned int a = 1; a < 4; a++)
				m_data[a] = (Type)0;
		}
		Quaternion(Type x, Type y, Type z, bool inDegrees = false)
		{
			if(inDegrees)
			{
				Type degToRad = (Type)(atan(1.f) / 45.f);
				
				x *= degToRad;
				y *= degToRad;
				z *= degToRad;
			}

			m_data[0] = cos(x / 2) * cos(y / 2) * cos(z / 2) + sin(x / 2) * sin(y / 2) * sin(z / 2);
			m_data[1] = sin(x / 2) * cos(y / 2) * cos(z / 2) - cos(x / 2) * sin(y / 2) * sin(z / 2);
			m_data[2] = cos(x / 2) * sin(y / 2) * cos(z / 2) + sin(x / 2) * cos(y / 2) * sin(z / 2);
			m_data[3] = cos(x / 2) * cos(y / 2) * sin(z / 2) - sin(x / 2) * sin(y / 2) * cos(z / 2);
		}
		Quaternion(Type w, Type x, Type y, Type z)
		{
			m_data[0] = w;
			m_data[1] = x;
			m_data[2] = y;
			m_data[3] = z;
		}
		Quaternion(Type *p)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = p[a];
		}
		Quaternion(const Quaternion &t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = t[a];
		}
		inline Type getLength() const
		{
			return (Type)sqrt((double)(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3]));
		}
		inline Matrix4x4<Type> getMatrix4x4() const
		{
			Type xx = m_data[1] * m_data[1];
			Type yy = m_data[2] * m_data[2];
			Type zz = m_data[3] * m_data[3];

			Type wx = m_data[0] * m_data[1];
			Type wy = m_data[0] * m_data[2];
			Type wz = m_data[0] * m_data[3];
			
			Type xy = m_data[1] * m_data[2];
			Type yz = m_data[2] * m_data[3];
			Type zx = m_data[3] * m_data[1];

			Type data[16];
			data[0] = 1 - 2 * (yy + zz);
			data[1] = 2 * (xy - wz);
			data[2] = 2 * (zx + wy);
			data[3] = 0.f;

			data[4] = 2 * (xy + wz);
			data[5] = 1 - 2 * (xx + zz);
			data[6] = 2 * (yz - wx);
			data[7] = 0.f;

			data[8] = 2 * (zx - wy);
			data[9] = 2 * (yz + wx);
			data[10] = 1 - 2 * (xx + yy);
			data[11] = 0.f;

			data[12] = 0.f;
			data[13] = 0.f;
			data[14] = 0.f;
			data[15] = 1.f;

			return Matrix4x4<Type>(data);
		}
		inline Matrix4x4<Type> getTransposeMatrix4x4() const
		{
			Type xx = m_data[1] * m_data[1];
			Type yy = m_data[2] * m_data[2];
			Type zz = m_data[3] * m_data[3];

			Type wx = m_data[0] * m_data[1];
			Type wy = m_data[0] * m_data[2];
			Type wz = m_data[0] * m_data[3];
			
			Type xy = m_data[1] * m_data[2];
			Type yz = m_data[2] * m_data[3];
			Type zx = m_data[3] * m_data[1];

			Type data[16];
			data[0] = 1 - 2 * (yy + zz);
			data[4] = 2 * (xy - wz);
			data[8] = 2 * (zx + wy);
			data[12] = 0.f;

			data[1] = 2 * (xy + wz);
			data[5] = 1 - 2 * (xx + zz);
			data[9] = 2 * (yz - wx);
			data[13] = 0.f;

			data[2] = 2 * (zx - wy);
			data[6] = 2 * (yz + wx);
			data[10] = 1 - 2 * (xx + yy);
			data[14] = 0.f;

			data[3] = 0.f;
			data[7] = 0.f;
			data[11] = 0.f;
			data[15] = 1.f;

			return Matrix4x4<Type>(data);
		}
		inline Type getW() const
		{
			return m_data[0];
		}
		inline Type getX() const
		{
			return m_data[1];
		}
		inline Type getY() const
		{
			return m_data[2];
		}
		inline Type getZ() const
		{
			return m_data[3];
		}
		inline Quaternion &normalize()
		{
			Type length = getLength();
			if(length)
				*this /= length;
			return *this;
		}
		inline void setW(Type w)
		{
			m_data[0] = w;
		}
		inline void setX(Type x)
		{
			m_data[1] = x;
		}
		inline void setY(Type y)
		{
			m_data[2] = y;
		}
		inline void setZ(Type z)
		{
			m_data[3] = z;
		}
		inline Quaternion &operator*=(Type t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] *= t;
			return *this;
		}
		inline Quaternion &operator/=(Type t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] /= t;
			return *this;
		}
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline Type operator[](int i) const
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline Quaternion operator+(const Quaternion &v) const
		{
			return Quaternion(m_data[0] + v[0], m_data[1] + v[1], m_data[2] + v[2], m_data[3] + v[3]);
		}
		inline Quaternion operator-(const Quaternion &v) const
		{
			return Quaternion(m_data[0] - v[0], m_data[1] - v[1], m_data[2] - v[2], m_data[3] - v[3]);
		}
		inline Quaternion operator*(const Quaternion &v) const
		{
			Type data[4];
			data[0] = m_data[0] * v[0] - m_data[1] * v[1] - m_data[2] * v[2] - m_data[3] * v[3];
			data[1] = m_data[0] * v[1] + m_data[1] * v[0] + m_data[2] * v[3] - m_data[3] * v[2];
			data[2] = m_data[0] * v[2] - m_data[1] * v[3] + m_data[2] * v[0] + m_data[3] * v[1];
			data[3] = m_data[0] * v[3] + m_data[1] * v[2] - m_data[2] * v[1] + m_data[3] * v[0];

			return Quaternion(data);
		}
	};
}

#endif
