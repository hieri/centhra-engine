//- Standard Library -
#include <math.h>
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/Plane.h>
#include <CE/Base.h>

using namespace std;

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
			unsigned int _minX, _minY, _maxX, _maxY;
			_minX = (unsigned int)floor(minX / m_zoneSize);
			_minY = (unsigned int)floor(minY / m_zoneSize);
			_maxX = (unsigned int)floor(maxX / m_zoneSize);
			_maxY = (unsigned int)floor(maxY / m_zoneSize);

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

			vector<ZoneEntity *> entities;

			for(unsigned int a = _minX; a <= _maxX; a++)
				for(unsigned int b = _minY; b <= _maxY; b++)
					for(vector<ZoneEntity *>::iterator it = m_zones[a][b]->m_children.begin(); it != m_zones[a][b]->m_children.end(); it++)
						entities.push_back(*it);

			for(vector<ZoneEntity *>::iterator it = entities.begin(); it != entities.end(); it++)
				(*it)->Render();
			for(vector<ZoneEntity *>::iterator it = entities.begin(); it != entities.end(); it++)
				(*it)->FinishRender();
//			for(unsigned int a = _minX; a <= _maxX; a++)
//				for(unsigned int b = _minY; b <= _maxY; b++)
//					m_zones[a][b]->Render();
/*
			for(unsigned int a = _minX; a <= _maxX; a++)
				for(unsigned int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->Render();
			for(unsigned int a = _minX; a <= _maxX; a++)
				for(unsigned int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->FinishRender();
*/		}
		void Plane::Place(ZoneEntity *entity)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();
			unsigned int _minX, _minY, _maxX, _maxY;
			_minX = (unsigned int)floor(position[0] / m_zoneSize);
			_minY = (unsigned int)floor(position[1] / m_zoneSize);
			_maxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
			_maxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);

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

			for(unsigned int a = _minX; a <= _maxX; a++)
				for(unsigned int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->Add(entity);
		}
		void Plane::MoveEntity(ce::game2d::ZoneEntity *entity, Vector2<float> movement)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();

			unsigned int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;
			aMinX = (unsigned int)floor(position[0] / m_zoneSize);
			aMinY = (unsigned int)floor(position[1] / m_zoneSize);
			aMaxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
			aMaxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);

			position += movement;
			entity->SetPosition(position);

			bMinX = (unsigned int)floor(position[0] / m_zoneSize);
			bMinY = (unsigned int)floor(position[1] / m_zoneSize);
			bMaxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
			bMaxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);

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

			for(unsigned int a = aMinX; a <= aMaxX; a++)
				for(unsigned int b = aMinY; b <= aMaxY; b++)
					if(a < bMinX || b < bMinY || a > bMaxX || b > bMaxY)
						m_zones[a][b]->Remove(entity);

			for(unsigned int a = bMinX; a <= bMaxX; a++)
				for(unsigned int b = bMinY; b <= bMaxY; b++)
					if(a < aMinX || b < aMinY || a > aMaxX || b > aMaxY)
						m_zones[a][b]->Add(entity);
		}
		void Plane::ProcessPhysics(float dt)
		{
			vector<ZoneEntity *> entities;
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					for(vector<ZoneEntity *>::iterator it = m_zones[a][b]->m_children.begin(); it != m_zones[a][b]->m_children.end(); it++)
					{
						(*it)->m_startedPhysics = false;
						entities.push_back(*it);
					}
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->PhysicsPhase1(dt);

			for(vector<ZoneEntity *>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				ZoneEntity *entity = *it;
				if(entity->m_startedPhysics)
					continue;
				Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();
				unsigned int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;
				aMinX = (unsigned int)floor(position[0] / m_zoneSize);
				aMinY = (unsigned int)floor(position[1] / m_zoneSize);
				aMaxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
				aMaxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);

				bMinX = (unsigned int)floor(entity->m_moveBoxMin[0] / m_zoneSize);
				bMinY = (unsigned int)floor(entity->m_moveBoxMin[1] / m_zoneSize);
				bMaxX = (unsigned int)floor(entity->m_moveBoxMax[0] / m_zoneSize);
				bMaxY = (unsigned int)floor(entity->m_moveBoxMax[1] / m_zoneSize);

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

				for(unsigned int x = aMinX; x <= aMaxX; x++)
					for(unsigned int y = aMinY; y <= aMaxY; y++)
						if(x < bMinX || y < bMinY || x > bMaxX || y > bMaxY)
							m_zones[x][y]->Remove(entity);

				for(unsigned int x = bMinX; x <= bMaxX; x++)
					for(unsigned int y = bMinY; y <= bMaxY; y++)
						if(x < aMinX || y < aMinY || x > aMaxX || y > aMaxY)
							m_zones[x][y]->Add(entity);

				entity->m_startedPhysics = true;
			}

			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->PhysicsPhase2(dt);

			for(vector<ZoneEntity *>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				ZoneEntity *entity = *it;
				if(!entity->m_finishedPhysics)
				{
					entity->m_finishedPhysics = true;
					Vector2<float> velocity = entity->GetVelocity();
					unsigned int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;

					aMinX = (unsigned int)floor(entity->m_moveBoxMin[0] / m_zoneSize);
					aMinY = (unsigned int)floor(entity->m_moveBoxMin[1] / m_zoneSize);
					aMaxX = (unsigned int)floor(entity->m_moveBoxMax[0] / m_zoneSize);
					aMaxY = (unsigned int)floor(entity->m_moveBoxMax[1] / m_zoneSize);

/*					if(!entity->m_canMove[0])
					{
						if(entity->m_movePadding[0] >= 0.01f)
							entity->m_movePadding[0] /= 2.f;
						else
							entity->m_movement[0] = 0.f;
					}
					if(!entity->m_canMove[1])
					{
						if(entity->m_movePadding[1] >= 0.01f)
							entity->m_movePadding[1] /= 2.f;
						else
							entity->m_movement[1] = 1.f;
					}
*/
					if(entity->m_canMove[0] || entity->m_canMove[1])
					{
						if(!entity->m_canMove[0])
							entity->m_movement[0] = 0.f;
						if(!entity->m_canMove[1])
							entity->m_movement[1] = 0.f;

						entity->m_position += entity->m_movement;
//						entity->Move(entity->m_movement);

/*						if(entity->m_canMove[0])
							entity->m_movePadding[0] = 1.f;
						if(entity->m_canMove[1])
							entity->m_movePadding[1] = 1.f;
*/					}

					Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent();
					bMinX = (unsigned int)floor(position[0] / m_zoneSize);
					bMinY = (unsigned int)floor(position[1] / m_zoneSize);
					bMaxX = (unsigned int)floor((position[0] + extent[0]) / m_zoneSize);
					bMaxY = (unsigned int)floor((position[1] + extent[1]) / m_zoneSize);
					
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

					for(unsigned int x = aMinX; x <= aMaxX; x++)
						for(unsigned int y = aMinY; y <= aMaxY; y++)
							if(x < bMinX || y < bMinY || x > bMaxX || y > bMaxY)
								m_zones[x][y]->Remove(entity);

					for(unsigned int x = bMinX; x <= bMaxX; x++)
						for(unsigned int y = bMinY; y <= bMaxY; y++)
							if(x < aMinX || y < aMinY || x > aMaxX || y > aMaxY)
								m_zones[x][y]->Add(entity);
					
/*					if(!entity->m_canMove[0])
						entity->m_velocity[0] *= -1.f;
					if(!entity->m_canMove[1])
						entity->m_velocity[1] *= -1.f;
*/					entity->m_movement = Vector2<float>(0.f, 0.f);
				}
			}
		}
		void Plane::RemoveDead()
		{
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->RemoveDead();
		}
	}
}
