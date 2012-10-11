//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/ImageArray.h>

using namespace std;

namespace ce
{
	ImageArray *ImageArray::createFromImage(Image *image)
	{
		if(!image)
		{
			setError("ImageArray::createFromImage: Image does not exist.");
			return 0;
		}

		Vector2<int> size = image->getSize();
		Color null = image->getPixel(0,0);
		vector<unsigned int> columnStart, columnEnd;
		bool isColumn = false;

		for(int a = 0; a < size[0]; a++)
		{
			Color color = image->getPixel(a, 1);
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
		
		ImageArray *imageArray = new ImageArray;
		imageArray->m_image = image;

		unsigned numColumns = columnStart.size();
		imageArray->m_numColumns = numColumns;

		vector<unsigned int> *rowStart = new vector<unsigned int>[numColumns], *rowEnd = new vector<unsigned int>[numColumns];
		imageArray->m_array = new Rect<int> *[numColumns];
		imageArray->m_relativeArray = new Rect<float> *[numColumns];
		imageArray->m_numRows = new unsigned int[numColumns];

		for(unsigned int a = 0; a < numColumns; a++)
		{
			unsigned int x = columnStart.at(a);
			bool isRow = false;

			for(int b = 1; b < size[1]; b++)
			{
				Color color = image->getPixel(x, b);
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
			imageArray->m_numRows[a] = numRows;

			imageArray->m_array[a] = new Rect<int>[numRows];
			imageArray->m_relativeArray[a] = new Rect<float>[numRows];
			for(unsigned int b = 0; b < numRows; b++)
			{
				unsigned int left = columnStart.at(a);
				unsigned int top = rowStart[a].at(b);
				unsigned int right = columnEnd.at(a);
				unsigned int bottom = rowEnd[a].at(b);
				imageArray->m_array[a][b] = Rect<int>(left, top, right, bottom);
				imageArray->m_relativeArray[a][b] = Rect<float>(((float)left) / size[0], ((float)top) / size[1], ((float)right) / size[0], ((float)bottom) / size[1]);
			}
		}

		delete [] rowStart;
		delete [] rowEnd;

		return imageArray;
	}
	ImageArray::ImageArray()
	{
	}
	ImageArray::~ImageArray()
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
	Image *ImageArray::getImage() const
	{
		return m_image;
	}
	Rect<int> ImageArray::getRect(unsigned int column, unsigned int row) const
	{
		if(column >= m_numColumns)
			return Rect<int>();
		if(row >= m_numRows[column])
			return Rect<int>();
		return m_array[column][row];
	}
	Rect<float> ImageArray::getRelativeRect(unsigned int column, unsigned int row) const
	{
		if(column >= m_numColumns)
			return Rect<float>();
		if(row >= m_numRows[column])
			return Rect<float>();
		return m_relativeArray[column][row];
	}
}
