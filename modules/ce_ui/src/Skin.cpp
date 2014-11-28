//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/UI/Skin.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		Skin::Skin(Image *image) : m_image(image)
		{
			Vector2<unsigned int> size = image->GetSize();
			Color null = image->GetPixel(size[0] - 1, size[1] - 1);
			vector<unsigned int> rowStart, rowEnd;
			bool isRow = false;

			for(unsigned int a = 0; a < size[1]; a++)
			{
				Color color = image->GetPixel(0, a);
				if(isRow)
				{
					if(color == null)
					{
						rowEnd.insert(rowEnd.end(), a);
						isRow = false;
					}
				}
				else
				{
					if(color != null)
					{
						rowStart.insert(rowStart.end(), a);
						isRow = true;
					}
				}
			}
			if(rowStart.size() > rowEnd.size())
				rowEnd.insert(rowEnd.end(), size[0]);

			unsigned numRows = rowStart.size();
			m_numRows = numRows;

			vector<unsigned int> *columnStart = new vector<unsigned int>[numRows], *columnEnd = new vector<unsigned int>[numRows];
			m_array = new Rect<int> *[numRows];
			m_relativeArray = new Rect<float> *[numRows];
			m_numColumns = new unsigned int[numRows];

			for(unsigned int a = 0; a < numRows; a++)
			{
				unsigned int y = rowStart.at(a);
				bool isColumn = false;

				for(unsigned int b = 0; b < size[0]; b++)
				{
					Color color = image->GetPixel(b, y);
					if(isColumn)
					{
						if(color == null)
						{
							columnEnd[a].insert(columnEnd[a].end(), b);
							isColumn = false;
						}
					}
					else
					{
						if(color != null)
						{
							columnStart[a].insert(columnStart[a].end(), b);
							isColumn = true;
						}
					}
				}

				if(columnStart[a].size() > columnEnd[a].size())
					columnEnd[a].insert(columnEnd[a].end(), size[0]);

				unsigned int numColumns = columnStart[a].size();
				m_numColumns[a] = numColumns;

				m_array[a] = new Rect<int>[numColumns];
				m_relativeArray[a] = new Rect<float>[numColumns];
				for(unsigned int b = 0; b < numColumns; b++)
				{
					unsigned int left = columnStart[a].at(b);
					unsigned int right = columnEnd[a].at(b);
					unsigned int top = rowStart.at(a);
					unsigned int bottom = rowEnd.at(a);
					m_array[a][b] = Rect<int>(left, top, right, bottom);
					m_relativeArray[a][b] = Rect<float>(((float)left) / size[0], ((float)top) / size[1], ((float)right) / size[0], ((float)bottom) / size[1]);
				}
			}

			delete [] columnStart;
			delete [] columnEnd;
		}
		Skin::~Skin()
		{
			for(unsigned int a = 0; a < m_numRows; a++)
			{
				delete [] m_array[a];
				delete [] m_relativeArray[a];
			}
			delete [] m_array;
			delete [] m_relativeArray;
			delete [] m_numColumns;
		}
		Image *Skin::GetImage() const
		{
			return m_image;
		}
		Rect<int> Skin::GetRect(unsigned int column, unsigned int row) const
		{
			if(row >= m_numRows)
				return Rect<int>();
			if(column >= m_numColumns[row])
				return Rect<int>();
			return m_array[row][column];
		}
		Rect<float> Skin::GetRelativeRect(unsigned int column, unsigned int row) const
		{
			if(row >= m_numRows)
				return Rect<float>();
			if(column >= m_numColumns[row])
				return Rect<float>();
			return m_relativeArray[row][column];
		}
	}
}
