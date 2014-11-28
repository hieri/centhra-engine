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
			Color null = image->GetPixel(0, 0);
			vector<unsigned int> columnStart, columnEnd;
			bool isColumn = false;

			for(unsigned int a = 0; a < size[0]; a++)
			{
				Color color = image->GetPixel(a, 1);
				if(isColumn)
				{
					if(color == null)
					{
						columnEnd.insert(columnEnd.end(), a);
						isColumn = false;
					}
				}
				else
				{
					if(color != null)
					{
						columnStart.insert(columnStart.end(), a);
						isColumn = true;
					}
				}
			}
			if(columnStart.size() > columnEnd.size())
				columnEnd.insert(columnEnd.end(), size[0]);

			unsigned numColumns = columnStart.size();
			m_numColumns = numColumns;

			vector<unsigned int> *rowStart = new vector<unsigned int>[numColumns], *rowEnd = new vector<unsigned int>[numColumns];
			m_array = new Rect<int> *[numColumns];
			m_relativeArray = new Rect<float> *[numColumns];
			m_numRows = new unsigned int[numColumns];

			for(unsigned int a = 0; a < numColumns; a++)
			{
				unsigned int x = columnStart.at(a);
				bool isRow = false;

				for(unsigned int b = 1; b < size[1]; b++)
				{
					Color color = image->GetPixel(x, b);
					if(isRow)
					{
						if(color == null)
						{
							rowEnd[a].insert(rowEnd[a].end(), b);
							isRow = false;
						}
					}
					else
					{
						if(color != null)
						{
							rowStart[a].insert(rowStart[a].end(), b);
							isRow = true;
						}
					}
				}

				if(rowStart[a].size() > rowEnd[a].size())
					rowEnd[a].insert(rowEnd[a].end(), size[1]);

				unsigned int numRows = rowStart[a].size();
				m_numRows[a] = numRows;

				m_array[a] = new Rect<int>[numRows];
				m_relativeArray[a] = new Rect<float>[numRows];
				for(unsigned int b = 0; b < numRows; b++)
				{
					unsigned int left = columnStart.at(a);
					unsigned int top = rowStart[a].at(b);
					unsigned int right = columnEnd.at(a);
					unsigned int bottom = rowEnd[a].at(b);
					m_array[a][b] = Rect<int>(left, top, right, bottom);
					m_relativeArray[a][b] = Rect<float>(((float)left) / size[0], ((float)top) / size[1], ((float)right) / size[0], ((float)bottom) / size[1]);
				}
			}

			delete [] rowStart;
			delete [] rowEnd;
		}
		Skin::~Skin()
		{
			for(unsigned int a = 0; a < m_numColumns; a++)
			{
				delete [] m_array[a];
				delete [] m_relativeArray[a];
			}
			delete [] m_array;
			delete [] m_relativeArray;
			delete [] m_numRows;
		}
		Image *Skin::GetImage() const
		{
			return m_image;
		}
		Rect<int> Skin::GetRect(unsigned int column, unsigned int row) const
		{
			if(column >= m_numColumns)
				return Rect<int>();
			if(row >= m_numRows[column])
				return Rect<int>();
			return m_array[column][row];
		}
		Rect<float> Skin::GetRelativeRect(unsigned int column, unsigned int row) const
		{
			if(column >= m_numColumns)
				return Rect<float>();
			if(row >= m_numRows[column])
				return Rect<float>();
			return m_relativeArray[column][row];
		}
	}
}
