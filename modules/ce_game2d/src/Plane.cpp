//- Standard Library -
#include <math.h>

//- Centhra Engine -
#include <CE/Game2D/Plane.h>
#include <CE/Base.h>

namespace ce
{
	namespace game2d
	{
		Plane::Plane(unsigned int width, unsigned int height, float zoneSize)
		{
			m_width = width;
			m_height = height;
			m_zoneSize = zoneSize;
			m_zones = new Zone **[m_width];
			for(unsigned int a = 0; a < m_width; a++)
			{
				m_zones[a] = new Zone *[m_height];
				for(unsigned b = 0; b  < m_height; b++)
				{
					m_zones[a][b] = new Zone(a * m_zoneSize, b * m_zoneSize, (a + 1) * m_zoneSize, (b + 1) * m_zoneSize);
					m_zones[a][b]->SetPlane(this);
				}
			}
		}
		Plane::~Plane()
		{
			for(unsigned int a = 0; a < m_width; a++)
				if(m_zones[a])
				{
					for(unsigned int b = 0; b < m_height; b++)
						if(m_zones[a][b])
							delete m_zones[a][b];
					delete [] m_zones[a];
				}
			delete [] m_zones;
		}
		Zone *Plane::GetZone(unsigned int x, unsigned int y) const
		{
			return m_zones[x][y];
		}
		void Plane::Render(float minX, float minY, float maxX, float maxY)
		{
			int _minX, _minY, _maxX, _maxY;
			_minX = (int)floor(minX / m_zoneSize);
			_minY = (int)floor(minY / m_zoneSize);
			_maxX = (int)floor(maxX / m_zoneSize);
			_maxY = (int)floor(maxY / m_zoneSize);

			if(_minX < 0)
				_minX = 0;
			else if(_minX >= m_width)
				_minX = m_width - 1;
			if(_minY < 0)
				_minY = 0;
			else if(_minY >= m_height)
				_minY = m_height - 1;
			if(_maxX < 0)
				_maxX = 0;
			else if(_maxX >= m_width)
				_maxX = m_width - 1;
			if(_maxY < 0)
				_maxY = 0;
			else if(_maxY >= m_height)
				_maxY = m_height - 1;

			for(int a = _minX; a <= _maxX; a++)
				for(int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->Render();

			for(int a = _minX; a <= _maxX; a++)
				for(int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->FinishRender();
		}
		void Plane::Place(ZoneEntity *entity)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();
			unsigned int _minX, _minY, _maxX, _maxY;
			_minX = (unsigned int)floor(position[0] / m_zoneSize);
			_minY = (unsigned int)floor(position[1] / m_zoneSize);
			_maxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
			_maxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);

			if(_minX < m_width && _minY < m_height && _maxX < m_width && _maxY < m_height)
			{
				for(int a = _minX; a <= _maxX; a++)
					for(int b = _minY; b <= _maxY; b++)
						m_zones[a][b]->Add(entity);
			}
		}
		void Plane::MoveEntity(ce::game2d::ZoneEntity *entity, Vector2<float> movement)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();

			int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;
			aMinX = (int)floor(position[0] / m_zoneSize);
			aMinY = (int)floor(position[1] / m_zoneSize);
			aMaxX = (int)floor((position[0] + extent[0]) / m_zoneSize);
			aMaxY = (int)floor((position[1] + extent[1]) / m_zoneSize);

			position += movement;
			entity->SetPosition(position);

			bMinX = (int)floor(position[0] / m_zoneSize);
			bMinY = (int)floor(position[1] / m_zoneSize);
			bMaxX = (int)floor((position[0] + extent[0]) / m_zoneSize);
			bMaxY = (int)floor((position[1] + extent[1]) / m_zoneSize);

			if(aMinX < 0)
				aMinX = 0;
			else if(aMinX >= m_width)
				aMinX = m_width - 1;
			if(aMinY < 0)
				aMinY = 0;
			else if(aMinY >= m_height)
				aMinY = m_height - 1;
			if(aMaxX < 0)
				aMaxX = 0;
			else if(aMaxX >= m_width)
				aMaxX = m_width - 1;
			if(aMaxY < 0)
				aMaxY = 0;
			else if(aMaxY >= m_height)
				aMaxY = m_height - 1;

			if(bMinX < 0)
				bMinX = 0;
			else if(bMinX >= m_width)
				bMinX = m_width - 1;
			if(bMinY < 0)
				bMinY = 0;
			else if(bMinY >= m_height)
				bMinY = m_height - 1;
			if(bMaxX < 0)
				bMaxX = 0;
			else if(bMaxX >= m_width)
				bMaxX = m_width - 1;
			if(bMaxY < 0)
				bMaxY = 0;
			else if(bMaxY >= m_height)
				bMaxY = m_height - 1;

			for(int a = aMinX; a <= aMaxX; a++)
				for(int b = aMinY; b <= aMaxY; b++)
					if(a < bMinX || b < bMinY || a > bMaxX || b > bMaxY)
						m_zones[a][b]->Remove(entity);

			for(int a = bMinX; a <= bMaxX; a++)
				for(int b = bMinY; b <= bMaxY; b++)
					if(a < aMinX || b < aMinY || a > aMaxX || b > aMaxY)
						m_zones[a][b]->Add(entity);
		}
	}
}