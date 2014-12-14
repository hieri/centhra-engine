#ifndef _CE_BASE_MATRIX4X4_H_
#define _CE_BASE_MATRIX4X4_H_

//- Standard Library -
#include <cassert>

//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Vector2.h>
#include <CE/Vector3.h>

namespace ce
{
	/* 	4x4 Matrix Class
	 * 	- Data structure holding a 4x4 matrix.
	 */
	template<class Type> class Matrix4x4
	{
		Type m_data[16];

	public:
		static Matrix4x4 BuildFromTranslation(Vector2<int_canvas> translation)
		{
			Matrix4x4 m;
			m[12] = (Type)translation[0];
			m[13] = (Type)translation[1];
			return m;
		}
		static Matrix4x4 BuildFromTranslation(Vector3<Type> translation)
		{
			Matrix4x4 m;
			m[12] = translation[0];
			m[13] = translation[1];
			m[14] = translation[2];
			return m;
		}

		static Matrix4x4 BuildFromScale(Vector2<int_canvas> scale)
		{
			Matrix4x4 m;
			m[0] = (Type)scale[0];
			m[5] = (Type)scale[1];
			return m;
		}
		static Matrix4x4 BuildFromScale(Vector3<Type> scale)
		{
			Matrix4x4 m;
			m[0] = scale[0];
			m[5] = scale[1];
			m[10] = scale[2];
			return m;
		}

		Matrix4x4()
		{
			for(unsigned char a = 0; a < 16; a++)
				m_data[a] = 0.f;
			for(unsigned char a = 0; a < 16; a += 5)
				m_data[a] = 1.f;
		}
		Matrix4x4(Type *p)
		{
			for(unsigned char a = 0; a < 16; a++)
				m_data[a] = p[a];
		}
		Matrix4x4(const Matrix4x4 &t)
		{
			for(unsigned char a = 0; a < 16; a++)
				m_data[a] = t[a];
		}
		inline Matrix4x4 GetRotationMatrix() const
		{
			Matrix4x4 m(*this);
			m[12] = 0.f;
			m[13] = 0.f;
			m[14] = 0.f;
			return m;
		}
		inline Vector3<float> GetTranslation() const
		{
			return Vector3<float>(m_data[12], m_data[13], m_data[14]);
		}
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 16);
			return m_data[i];
		}
		inline Type operator[](int i) const
		{
			assert(i >= 0 && i < 16);
			return m_data[i];
		}
		inline Matrix4x4 operator*(const Matrix4x4 &m) const
		{
			Matrix4x4 mat;

			mat[0]  = m[0]  * m_data[0] + m[1]  * m_data[4] + m[2]  * m_data[8]  + m[3]  * m_data[12];
			mat[1]  = m[0]  * m_data[1] + m[1]  * m_data[5] + m[2]  * m_data[9]  + m[3]  * m_data[13];
			mat[2]  = m[0]  * m_data[2] + m[1]  * m_data[6] + m[2]  * m_data[10] + m[3]  * m_data[14];
			mat[3]  = m[0]  * m_data[3] + m[1]  * m_data[7] + m[2]  * m_data[11] + m[3]  * m_data[15];

			mat[4]  = m[4]  * m_data[0] + m[5]  * m_data[4] + m[6]  * m_data[8]  + m[7]  * m_data[12];
			mat[5]  = m[4]  * m_data[1] + m[5]  * m_data[5] + m[6]  * m_data[9]  + m[7]  * m_data[13];
			mat[6]  = m[4]  * m_data[2] + m[5]  * m_data[6] + m[6]  * m_data[10] + m[7]  * m_data[14];
			mat[7]  = m[4]  * m_data[3] + m[5]  * m_data[7] + m[6]  * m_data[11] + m[7]  * m_data[15];

			mat[8]  = m[8]  * m_data[0] + m[9]  * m_data[4] + m[10] * m_data[8]  + m[11] * m_data[12];
			mat[9]  = m[8]  * m_data[1] + m[9]  * m_data[5] + m[10] * m_data[9]  + m[11] * m_data[13];
			mat[10] = m[8]  * m_data[2] + m[9]  * m_data[6] + m[10] * m_data[10] + m[11] * m_data[14];
			mat[11] = m[8]  * m_data[3] + m[9]  * m_data[7] + m[10] * m_data[11] + m[11] * m_data[15];

			mat[12] = m[12] * m_data[0] + m[13] * m_data[4] + m[14] * m_data[8]  + m[15] * m_data[12];
			mat[13] = m[12] * m_data[1] + m[13] * m_data[5] + m[14] * m_data[9]  + m[15] * m_data[13];
			mat[14] = m[12] * m_data[2] + m[13] * m_data[6] + m[14] * m_data[10] + m[15] * m_data[14];
			mat[15] = m[12] * m_data[3] + m[13] * m_data[7] + m[14] * m_data[11] + m[15] * m_data[15];

			return mat;
		}
		inline Matrix4x4 &Transpose()
		{
			Type tempData[16];
			for(unsigned int a = 0; a < 16; a++)
				tempData[a] = m_data[a];

			for(unsigned int a = 0; a < 4; a++)
				for(unsigned int b = 0; b < 4; b++)
					m_data[a * 4 + b] = tempData[b * 4 + a];

			return *this;
		}
	};
}

#endif
