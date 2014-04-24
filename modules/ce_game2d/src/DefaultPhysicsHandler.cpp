//- Standard Library -
#include <algorithm>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/Game2D/PhysicalObject.h>

using namespace std;

//- TODO: Implement collision callback -

namespace ce
{
	namespace game2d
	{
		vector<DefaultPhysicsHandler::ObjectHandle *> DefaultPhysicsHandler::ObjectHandle::ms_cacheVectors[8];
		void DefaultPhysicsHandler::ObjectHandle::ClearCache(unsigned int idx)
		{
			std::vector<ObjectHandle *> &cacheVector = ms_cacheVectors[idx];
			for(vector<ObjectHandle *>::iterator it = cacheVector.begin(); it != cacheVector.end(); it++)
				(*it)->m_cache[idx] = false;
			cacheVector.clear();
		}
		DefaultPhysicsHandler::ObjectHandle::ObjectHandle(PhysicsHandler *handler, PhysicalObject *object) : PhysicsHandler::ObjectHandle(handler, object)
		{
			m_startedPhysics = m_finishedPhysics = false;
			m_movePadding = Vector2<float>(1.f, 1.f);
			m_canMove[0] = m_canMove[1] = true;

			for(int a = 0; a < CE_OBJECTHANDLE_CACHESIZE; a++)
				m_cache[a] = false;
		}
		DefaultPhysicsHandler::ObjectHandle::~ObjectHandle()
		{
			for(vector<Zone *>::iterator it = m_zones.begin(); it != m_zones.end(); it++)
				(*it)->Remove(this);
		}
		void DefaultPhysicsHandler::ObjectHandle::AddZone(Zone *zone)
		{
			if(!ContainsZone(zone))
				m_zones.push_back(zone);
		}
		bool DefaultPhysicsHandler::ObjectHandle::ContainsZone(Zone *zone) const
		{
			return find(m_zones.begin(), m_zones.end(), zone) != m_zones.end();
		}
		void DefaultPhysicsHandler::ObjectHandle::RemoveZone(Zone *zone)
		{
			vector<Zone *>::iterator it = find(m_zones.begin(), m_zones.end(), zone);
			if(it != m_zones.end())
				m_zones.erase(it);
		}
		void DefaultPhysicsHandler::ObjectHandle::RemoveFromZones()
		{
			while(m_zones.size())
				m_zones.back()->Remove(this);
		}
		Vector2<float> DefaultPhysicsHandler::ObjectHandle::GetExtent() const
		{
			return m_object->GetExtent();
		}
		Vector2<float> DefaultPhysicsHandler::ObjectHandle::GetPosition() const
		{
			return m_object->GetPosition();
		}
		Vector2<float> DefaultPhysicsHandler::ObjectHandle::GetVelocity() const
		{
			return m_object->GetVelocity();
		}
		void DefaultPhysicsHandler::ObjectHandle::SetExtent(Vector2<float> extent)
		{
			m_object->SetExtent(extent);
		}
		void DefaultPhysicsHandler::ObjectHandle::SetPosition(Vector2<float> position)
		{
			m_object->SetPosition(position);
		}
		void DefaultPhysicsHandler::ObjectHandle::SetVelocity(Vector2<float> velocity)
		{
			m_object->SetVelocity(velocity);
		}
		vector<DefaultPhysicsHandler::Zone *> &DefaultPhysicsHandler::ObjectHandle::GetZones()
		{
			return m_zones;
		}
		void DefaultPhysicsHandler::ObjectHandle::Move(Vector2<float> movement)
		{
			if(m_zones.size())
				m_zones[0]->MoveEntity(this, movement);
			else
				SetPosition(GetPosition() + movement);
		}
		bool DefaultPhysicsHandler::ObjectHandle::CollidesWith(ObjectHandle *entity, Vector2<float> offsetA, Vector2<float> offsetB)
		{
			Vector2<float> aMin, aMax, bMin, bMax;
			Vector2<float> extent = GetExtent();
			Vector2<float> halfExtent = extent / 2.f;
			aMin = GetPosition() + offsetA - halfExtent;
			aMax = aMin + extent;
			Vector2<float> entExtent = entity->GetExtent();
			Vector2<float> entHalfExtent = entExtent / 2.f;
			bMin = entity->GetPosition() + offsetB - entHalfExtent;
			bMax = bMin + entExtent;

			return aMin[0] < bMax[0] && aMin[1] < bMax[1] && bMin[0] < aMax[0] && bMin[1] < aMax[1];
		}
		bool DefaultPhysicsHandler::ObjectHandle::CollidesWith(Vector2<float> boxMin, Vector2<float> boxMax, Vector2<float> offset)
		{
			Vector2<float> min, max;
			Vector2<float> extent = GetExtent();
			Vector2<float> halfExtent = extent / 2.f;
			min = GetPosition() + offset - halfExtent;
			max = min + extent;

			return min[0] < boxMax[0] && min[1] < boxMax[1] && boxMin[0] < max[0] && boxMin[1] < max[1];
		}
		unsigned int DefaultPhysicsHandler::ObjectHandle::GetCollisionMask() const
		{
			return m_object->GetCollisionMask();
		}
		bool DefaultPhysicsHandler::ObjectHandle::Cache(unsigned int idx)
		{
			if(m_cache[idx])
				return false;

			m_cache[idx] = true;
			ms_cacheVectors[idx].push_back(this);
			return true;
		}
		
		//-------------------------------------------------------------------------------------------------------------------------------------

		DefaultPhysicsHandler::Zone::Zone(float minX, float minY, float maxX, float maxY)
		{
			m_minX = minX;
			m_minY = minY;
			m_maxX = maxX;
			m_maxY = maxY;
			m_plane = 0;
		}
		DefaultPhysicsHandler::Zone::~Zone()
		{
			for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->RemoveZone(this);
		}
		void DefaultPhysicsHandler::Zone::DoRender()
		{
/*			for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ObjectHandle *entity = *it;
				if(entity->Cache(0))
					entity->Render();
			}
*/
			glPushMatrix();
				if(m_children.size())
					glColor3ub(255, 0, 0);
				glBegin(GL_LINE_LOOP);
					glVertex2f(m_minX + 0.5f, m_minY + 0.5f);
					glVertex2f(m_maxX - 0.5f, m_minY + 0.5f);
					glVertex2f(m_maxX - 0.5f, m_maxY - 0.5f);
					glVertex2f(m_minX + 0.5f, m_maxY - 0.5f);
					glVertex2f(m_minX + 0.5f, m_minY + 0.5f);
				glEnd();
				glColor3ub(255, 255, 255);
			glPopMatrix();

			if(!m_plane)
				ObjectHandle::ClearCache(0);
		}
		void DefaultPhysicsHandler::Zone::Add(ObjectHandle *entity)
		{
			if(!IsMember(entity))
			{
				m_children.push_back(entity);
				entity->AddZone(this);
			}
		}
		bool DefaultPhysicsHandler::Zone::IsMember(ObjectHandle *entity) const
		{
			return find(m_children.begin(), m_children.end(), entity) != m_children.end();
		}
		void DefaultPhysicsHandler::Zone::Remove(ObjectHandle *entity)
		{
			vector<ObjectHandle *>::iterator it = find(m_children.begin(), m_children.end(), entity);
			if(it != m_children.end())
			{
				(*it)->RemoveZone(this);
				m_children.erase(it);
			}
		}
		void DefaultPhysicsHandler::Zone::RemoveDead()
		{
			vector<ObjectHandle *>::iterator it = m_children.begin();
			while(it != m_children.end())
			{
				if((*it)->m_object->IsDead())
				{
					m_children.erase(it);
					it = m_children.begin();
				}
				else
					it++;
			}
		}
		void DefaultPhysicsHandler::Zone::SetPlane(Plane *plane)
		{
			m_plane = plane;
		}
		DefaultPhysicsHandler::Plane *DefaultPhysicsHandler::Zone::GetPlane() const
		{
			return m_plane;
		}
		void DefaultPhysicsHandler::Zone::MoveEntity(ObjectHandle *entity, Vector2<float> movement)
		{
			if(m_plane)
				m_plane->MoveEntity(entity, movement);
			else
				entity->SetPosition(entity->GetPosition() + movement);
		}
		void DefaultPhysicsHandler::Zone::ProcessPhysics(float dt)
		{
			if(m_plane)
				m_plane->ProcessPhysics(dt);
			else
			{
				PhysicsPhase1(dt);
				PhysicsPhase2(dt);
				PhysicsPhase3(dt);
			}
		}
		void DefaultPhysicsHandler::Zone::PhysicsPhase1(float dt)
		{
			for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ObjectHandle *entity = *it;
				entity->m_movement = entity->GetVelocity() * entity->m_movePadding * dt;
				Vector2<float> position = entity->GetPosition();
				Vector2<float> halfExtent = entity->GetExtent() / 2.f;

				for(int a = 0; a < 2; a++)
				{
					entity->m_moveBoxMin[a] = position[a] - halfExtent[a];
					if(entity->m_movement[a] < 0.f)
						entity->m_moveBoxMin[a] += entity->m_movement[a];
					entity->m_moveBoxMax[a] = position[a] + halfExtent[a];
					if(entity->m_movement[a] > 0.f)
						entity->m_moveBoxMax[a] += entity->m_movement[a];
					entity->m_canMove[a] = true;
				}
				entity->m_finishedPhysics = false;
			}
		}
		void DefaultPhysicsHandler::Zone::PhysicsPhase2(float dt)
		{
			for(vector<ObjectHandle *>::iterator itA = m_children.begin(); itA != m_children.end(); itA++)
			{
				ObjectHandle *entityA = *itA;
				if(entityA->GetVelocity()[0] != 0.f || entityA->GetVelocity()[1] != 0.f)
				{
					Vector2<float> aMin, aMax, aOMin, aOMax;
					aMin = entityA->m_moveBoxMin;
					aMax = entityA->m_moveBoxMax;
					Vector2<float> extentA = entityA->GetExtent();
					Vector2<float> halfExtentA = extentA / 2.f;
					aOMin = entityA->GetPosition() - halfExtentA;
					aOMax = aOMin + extentA;

					unsigned int colCount = 0;
					vector<ObjectHandle *> diagonals;

					for(vector<ObjectHandle *>::iterator itB = m_children.begin(); itB != m_children.end(); itB++)
						if(itA != itB)
						{
							ObjectHandle *entityB = *itB;
							if(!(entityA->GetCollisionMask() & entityB->GetCollisionMask()))
								continue;
							Vector2<float> bMin, bMax, bOMin, bOMax;
							bMin = entityB->m_moveBoxMin;
							bMax = entityB->m_moveBoxMax;
							Vector2<float> extentB = entityB->GetExtent();
							Vector2<float> halfExtentB = extentB / 2.f;
							bOMin = entityB->GetPosition() - halfExtentB;
							bOMax = bOMin + extentB;
							if(aMin[0] < bMax[0] && aMin[1] < bMax[1] && bMin[0] < aMax[0] && bMin[1] < aMax[1])
							{
								bool xCol = aMin[0] < bMax[0] && bMin[0] < aMax[0] && aOMin[1] < bOMax[1] && bOMin[1] < aOMax[1];
								bool yCol = aOMin[0] < bOMax[0] && bOMin[0] < aOMax[0] && aMin[1] < bMax[1] && bMin[1] < aMax[1];
								if(xCol)
								{
									bool axCol = aMin[0] < bOMax[0] && bOMin[0] < aMax[0];
									bool bxCol = aOMin[0] < bMax[0] && bMin[0] < aOMax[0];
									if(axCol || !bxCol)
										entityA->m_canMove[0] = false;
									if(bxCol || !axCol)
										entityB->m_canMove[0] = false;
								}
								if(yCol)
								{
									bool ayCol = aMin[1] < bOMax[1] && bOMin[1] < aMax[1];
									bool byCol = aOMin[1] < bMax[1] && bMin[1] < aOMax[1];
									if(ayCol || !byCol)
										entityA->m_canMove[1] = false;
									if(byCol || !ayCol)
										entityB->m_canMove[1] = false;
								}
								if(!xCol && !yCol)
									diagonals.push_back(entityB);
								colCount++;
							}
						}
					if((entityA->m_canMove[0] || entityA->m_canMove[1]) && diagonals.size())
					{
//						entityA->m_canMove[1] = false;
						entityA->m_canMove[0] = false;
/*						for(vector<ObjectHandle *>::iterator itB = diagonals.begin(); itB != diagonals.end(); itB++)
						{
							ObjectHandle *entityB = *itB;
							//entityB->m_canMove[1] = false;
							entityB->m_canMove[0] = false;
						}*/
					}
				}
			}
		}
		void DefaultPhysicsHandler::Zone::PhysicsPhase3(float dt)
		{
			for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ObjectHandle *entity = *it;
				if(!entity->m_finishedPhysics)
				{
					entity->m_finishedPhysics = true;
					Vector2<float> velocity = entity->GetVelocity();

					if(!entity->m_canMove[0])
					{
						if(entity->m_movePadding[0] > 0.05f)
							entity->m_movePadding[0] /= 2.f;
						else
							entity->m_movePadding[0] = 0.1f;
					}
					if(!entity->m_canMove[1])
					{
						if(entity->m_movePadding[1] > 0.05f)
							entity->m_movePadding[1] /= 2.f;
						else
							entity->m_movePadding[1] = 0.1f;
					}

					if(entity->m_canMove[0] || entity->m_canMove[1])
					{
						if(!entity->m_canMove[0])
							entity->m_movement[0] = 0.f;
						if(!entity->m_canMove[1])
							entity->m_movement[1] = 0.f;

						entity->Move(entity->m_movement);

						if(entity->m_canMove[0])
							entity->m_movePadding[0] = 1.f;
						if(entity->m_canMove[1])
							entity->m_movePadding[1] = 1.f;
					}
					if(!entity->m_canMove[0])
						entity->GetVelocity()[0] *= -1.f;
					if(!entity->m_canMove[1])
						entity->GetVelocity()[1] *= -1.f;
					entity->m_movement = Vector2<float>(0.f, 0.f);
				}
			}
		}
		vector<DefaultPhysicsHandler::ObjectHandle *> DefaultPhysicsHandler::Zone::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, ObjectHandle *ignore)
		{
			Vector2<float> _min(minX, minY), _max(maxX, maxY);
			vector<ObjectHandle *> found;
			for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ObjectHandle *entity = *it;
				if(entity->GetCollisionMask() & mask)
					if(entity != ignore)
						if(entity->CollidesWith(_min, _max))
							found.push_back(entity);
			}
			return found;
		}
		vector<DefaultPhysicsHandler::ObjectHandle *> DefaultPhysicsHandler::Zone::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, ObjectHandle *ignore)
		{
			Vector2<float> _min(min(startX, endX), min(startY, endY)), _max(max(startX, endX), max(startY, endY));
			vector<ObjectHandle *> found;

			//- handle the vertical line case -
			if(!(endX - startX))
			{
				for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				{
					ObjectHandle *entity = *it;
					if(entity->GetCollisionMask() & mask)
						if(entity != ignore)
							if(entity->CollidesWith(_min, _max))
							{
								Vector2<float> position = entity->GetPosition();
								Vector2<float> extent = entity->GetExtent();
								Vector2<float> halfExtent = extent / 2.f;

								float _startY = position[1] - halfExtent[1];
								float _endY = position[1] + halfExtent[1];

								Vector2<float> rMin(_min[0], _startY), rMax(_max[0], _endY);

								if(entity->CollidesWith(rMin, rMax))
									found.push_back(entity);
							}
				}
			}
			//- handle the horizontal line case -
			else if(!(endY - startY))
			{
				for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				{
					ObjectHandle *entity = *it;
					if(entity->GetCollisionMask() & mask)
						if(entity != ignore)
							if(entity->CollidesWith(_min, _max))
							{
								Vector2<float> position = entity->GetPosition();
								Vector2<float> extent = entity->GetExtent();
								Vector2<float> halfExtent = extent / 2.f;

								float _startX = position[0] - halfExtent[0];
								float _endX = position[0] + halfExtent[0];

								Vector2<float> rMin(_startX, _min[1]), rMax(_endX, _max[1]);

								if(entity->CollidesWith(rMin, rMax))
									found.push_back(entity);
							}
				}
			}
			//- handle the diagonal line case -
			else
			{
				if(startX > endX)
				{
					swap(startX, endX);
					swap(startY, endY);
				}

				float slope = (endY - startY) / (endX - startX);
				for(vector<ObjectHandle *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				{
					ObjectHandle *entity = *it;
					if(entity->GetCollisionMask() & mask)
						if(entity != ignore)
							if(entity->CollidesWith(_min, _max))
							{
								Vector2<float> position = entity->GetPosition();
								Vector2<float> extent = entity->GetExtent();
								Vector2<float> halfExtent = extent / 2.f;

								float _startX = position[0] - halfExtent[0];
								float _startY = startY + (_startX - startX) * slope;
								float _endX = position[0] + halfExtent[0];
								float _endY = startY + (_endX - startX) * slope;

								Vector2<float> rMin(min(_startX, _endX), min(_startY, _endY)), rMax(max(_startX, _endX), max(_startY, _endY));

								if(entity->CollidesWith(rMin, rMax))
									found.push_back(entity);
							}
				}
			}

			return found;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		
		DefaultPhysicsHandler::Plane::Plane(unsigned int width, unsigned int height, float zoneSize)
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
		DefaultPhysicsHandler::Plane::~Plane()
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
		DefaultPhysicsHandler::Zone *DefaultPhysicsHandler::Plane::GetZone(unsigned int x, unsigned int y) const
		{
			return m_zones[x][y];
		}
		void DefaultPhysicsHandler::Plane::Render(float minX, float minY, float maxX, float maxY)
		{
			int _minX, _minY, _maxX, _maxY;
			_minX = (int)floor(minX / m_zoneSize);
			_minY = (int)floor(minY / m_zoneSize);
			_maxX = (int)floor(maxX / m_zoneSize);
			_maxY = (int)floor(maxY / m_zoneSize);

			if(_minX < 0)
				_minX = 0;
			else if(_minX >= (int)m_width)
				_minX = m_width - 1;
			if(_minY < 0)
				_minY = 0;
			else if(_minY >= (int)m_height)
				_minY = m_height - 1;
			if(_maxX < 0)
				_maxX = 0;
			else if(_maxX >= (int)m_width)
				_maxX = m_width - 1;
			if(_maxY < 0)
				_maxY = 0;
			else if(_maxY >= (int)m_height)
				_maxY = m_height - 1;

			for(int a = _minX; a <= _maxX; a++)
				for(int b = _minY; b <= _maxY; b++)
					for(vector<ObjectHandle *>::iterator it = m_zones[a][b]->m_children.begin(); it != m_zones[a][b]->m_children.end(); it++)
					{
						ObjectHandle *entity = *it;
						if(entity->Cache(0))
							entity->m_object->Render();
					}
			ObjectHandle::ClearCache(0);

/*
			for(int a = _minX; a <= _maxX; a++)
				for(int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->Render();
*/		}
		void DefaultPhysicsHandler::Plane::Place(ObjectHandle *entity)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent(), halfExtent = extent / 2.f;
			int _minX, _minY, _maxX, _maxY;
			_minX = (int)floor((position[0] - halfExtent[0]) / m_zoneSize);
			_minY = (int)floor((position[1] - halfExtent[1]) / m_zoneSize);
			_maxX = (int)floor((position[0] + halfExtent[0]) / m_zoneSize);
			_maxY = (int)floor((position[1] + halfExtent[1]) / m_zoneSize);

			if(_minX < 0)
				_minX = 0;
			else if(_minX >= (int)m_width)
				_minX = m_width - 1;
			if(_minY < 0)
				_minY = 0;
			else if(_minY >= (int)m_height)
				_minY = m_height - 1;
			if(_maxX < 0)
				_maxX = 0;
			else if(_maxX >= (int)m_width)
				_maxX = m_width - 1;
			if(_maxY < 0)
				_maxY = 0;
			else if(_maxY >= (int)m_height)
				_maxY = m_height - 1;

			for(int a = _minX; a <= _maxX; a++)
				for(int b = _minY; b <= _maxY; b++)
					m_zones[a][b]->Add(entity);
		}
		void DefaultPhysicsHandler::Plane::MoveEntity(game2d::DefaultPhysicsHandler::ObjectHandle *entity, Vector2<float> movement)
		{
			Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent(), halfExtent = extent / 2.f;

			int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;
			aMinX = (int)floor((position[0] - halfExtent[0]) / m_zoneSize);
			aMinY = (int)floor((position[1] - halfExtent[1]) / m_zoneSize);
			aMaxX = (int)floor((position[0] + halfExtent[0]) / m_zoneSize);
			aMaxY = (int)floor((position[1] + halfExtent[1]) / m_zoneSize);

			position += movement;
			entity->SetPosition(position);
			
			bMinX = (int)floor((position[0] - halfExtent[0]) / m_zoneSize);
			bMinY = (int)floor((position[1] - halfExtent[1]) / m_zoneSize);
			bMaxX = (int)floor((position[0] + halfExtent[0]) / m_zoneSize);
			bMaxY = (int)floor((position[1] + halfExtent[1]) / m_zoneSize);

			if(aMinX < 0)
				aMinX = 0;
			else if(aMinX >= (int)m_width)
				aMinX = m_width - 1;
			if(aMinY < 0)
				aMinY = 0;
			else if(aMinY >= (int)m_height)
				aMinY = m_height - 1;
			if(aMaxX < 0)
				aMaxX = 0;
			else if(aMaxX >= (int)m_width)
				aMaxX = m_width - 1;
			if(aMaxY < 0)
				aMaxY = 0;
			else if(aMaxY >= (int)m_height)
				aMaxY = m_height - 1;

			if(bMinX < 0)
				bMinX = 0;
			else if(bMinX >= (int)m_width)
				bMinX = m_width - 1;
			if(bMinY < 0)
				bMinY = 0;
			else if(bMinY >= (int)m_height)
				bMinY = m_height - 1;
			if(bMaxX < 0)
				bMaxX = 0;
			else if(bMaxX >= (int)m_width)
				bMaxX = m_width - 1;
			if(bMaxY < 0)
				bMaxY = 0;
			else if(bMaxY >= (int)m_height)
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
		void DefaultPhysicsHandler::Plane::ProcessPhysics(float dt)
		{
			vector<ObjectHandle *> entities;
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					for(vector<ObjectHandle *>::iterator it = m_zones[a][b]->m_children.begin(); it != m_zones[a][b]->m_children.end(); it++)
					{
						(*it)->m_startedPhysics = false;
						entities.push_back(*it);
					}
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->PhysicsPhase1(dt);

			for(vector<ObjectHandle *>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				ObjectHandle *entity = *it;
				if(entity->m_startedPhysics)
					continue;
				Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent(), halfExtent = extent / 2.f;
				int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;
				aMinX = (int)floor((position[0] - halfExtent[0]) / m_zoneSize);
				aMinY = (int)floor((position[1] - halfExtent[1]) / m_zoneSize);
				aMaxX = (int)floor((position[0] + halfExtent[0]) / m_zoneSize);
				aMaxY = (int)floor((position[1] + halfExtent[1]) / m_zoneSize);

				bMinX = (int)floor(entity->m_moveBoxMin[0] / m_zoneSize);
				bMinY = (int)floor(entity->m_moveBoxMin[1] / m_zoneSize);
				bMaxX = (int)floor(entity->m_moveBoxMax[0] / m_zoneSize);
				bMaxY = (int)floor(entity->m_moveBoxMax[1] / m_zoneSize);

				if(aMinX < 0)
					aMinX = 0;
				else if(aMinX >= (int)m_width)
					aMinX = m_width - 1;
				if(aMinY < 0)
					aMinY = 0;
				else if(aMinY >= (int)m_height)
					aMinY = m_height - 1;
				if(aMaxX < 0)
					aMaxX = 0;
				else if(aMaxX >= (int)m_width)
					aMaxX = m_width - 1;
				if(aMaxY < 0)
					aMaxY = 0;
				else if(aMaxY >= (int)m_height)
					aMaxY = m_height - 1;

				if(bMinX < 0)
					bMinX = 0;
				else if(bMinX >= (int)m_width)
					bMinX = m_width - 1;
				if(bMinY < 0)
					bMinY = 0;
				else if(bMinY >= (int)m_height)
					bMinY = m_height - 1;
				if(bMaxX < 0)
					bMaxX = 0;
				else if(bMaxX >= (int)m_width)
					bMaxX = m_width - 1;
				if(bMaxY < 0)
					bMaxY = 0;
				else if(bMaxY >= (int)m_height)
					bMaxY = m_height - 1;

				for(int x = aMinX; x <= aMaxX; x++)
					for(int y = aMinY; y <= aMaxY; y++)
						if(x < bMinX || y < bMinY || x > bMaxX || y > bMaxY)
							m_zones[x][y]->Remove(entity);

				for(int x = bMinX; x <= bMaxX; x++)
					for(int y = bMinY; y <= bMaxY; y++)
						if(x < aMinX || y < aMinY || x > aMaxX || y > aMaxY)
							m_zones[x][y]->Add(entity);

				entity->m_startedPhysics = true;
			}

			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->PhysicsPhase2(dt);

			for(vector<ObjectHandle *>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				ObjectHandle *entity = *it;
				if(!entity->m_finishedPhysics)
				{
					entity->m_finishedPhysics = true;
					Vector2<float> velocity = entity->GetVelocity();
					int aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY;

					aMinX = (int)floor(entity->m_moveBoxMin[0] / m_zoneSize);
					aMinY = (int)floor(entity->m_moveBoxMin[1] / m_zoneSize);
					aMaxX = (int)floor(entity->m_moveBoxMax[0] / m_zoneSize);
					aMaxY = (int)floor(entity->m_moveBoxMax[1] / m_zoneSize);

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

						entity->SetPosition(entity->GetPosition() + entity->m_movement);
//						entity->Move(entity->m_movement);

/*						if(entity->m_canMove[0])
							entity->m_movePadding[0] = 1.f;
						if(entity->m_canMove[1])
							entity->m_movePadding[1] = 1.f;
*/					}

					Vector2<float> position = entity->GetPosition(), extent = entity->GetExtent(), halfExtent = extent / 2.f;
					bMinX = (int)floor((position[0] - halfExtent[0]) / m_zoneSize);
					bMinY = (int)floor((position[1] - halfExtent[1]) / m_zoneSize);
					bMaxX = (int)floor((position[0] + halfExtent[0]) / m_zoneSize);
					bMaxY = (int)floor((position[1] + halfExtent[1]) / m_zoneSize);
					
					 if(aMinX < 0)
						aMinX = 0;
					else if(aMinX >= (int)m_width)
						aMinX = m_width - 1;
					if(aMinY < 0)
						aMinY = 0;
					else if(aMinY >= (int)m_height)
						aMinY = m_height - 1;
					if(aMaxX < 0)
						aMaxX = 0;
					else if(aMaxX >= (int)m_width)
						aMaxX = m_width - 1;
					if(aMaxY < 0)
						aMaxY = 0;
					else if(aMaxY >= (int)m_height)
						aMaxY = m_height - 1;

					if(bMinX < 0)
						bMinX = 0;
					else if(bMinX >= (int)m_width)
						bMinX = m_width - 1;
					if(bMinY < 0)
						bMinY = 0;
					else if(bMinY >= (int)m_height)
						bMinY = m_height - 1;
					if(bMaxX < 0)
						bMaxX = 0;
					else if(bMaxX >= (int)m_width)
						bMaxX = m_width - 1;
					if(bMaxY < 0)
						bMaxY = 0;
					else if(bMaxY >= (int)m_height)
						bMaxY = m_height - 1;

					for(int x = aMinX; x <= aMaxX; x++)
						for(int y = aMinY; y <= aMaxY; y++)
							if(x < bMinX || y < bMinY || x > bMaxX || y > bMaxY)
								m_zones[x][y]->Remove(entity);

					for(int x = bMinX; x <= bMaxX; x++)
						for(int y = bMinY; y <= bMaxY; y++)
							if(x < aMinX || y < aMinY || x > aMaxX || y > aMaxY)
								m_zones[x][y]->Add(entity);
					
/*					if(!entity->m_canMove[0])
						entity->GetVelocity()[0] *= -1.f;
					if(!entity->m_canMove[1])
						entity->GetVelocity()[1] *= -1.f;
*/					entity->m_movement = Vector2<float>(0.f, 0.f);
				}
			}
		}
		void DefaultPhysicsHandler::Plane::RemoveDead()
		{
			for(unsigned int a = 0; a < m_width; a++)
				for(unsigned int b = 0; b < m_height; b++)
					m_zones[a][b]->RemoveDead();
		}
		vector<DefaultPhysicsHandler::ObjectHandle *> DefaultPhysicsHandler::Plane::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, ObjectHandle *ignore)
		{
			vector<ObjectHandle *> found;

			int MinX = (int)floor(minX / m_zoneSize);
			int MinY = (int)floor(minY / m_zoneSize);
			int MaxX = (int)floor(maxX / m_zoneSize);
			int MaxY = (int)floor(maxY / m_zoneSize);

			if(MinX < 0)
				MinX = 0;
			else if(MinX >= (int)m_width)
				MinX = m_width - 1;
			if(MinY < 0)
				MinY = 0;
			else if(MinY >= (int)m_height)
				MinY = m_height - 1;
			if(MaxX < 0)
				MaxX = 0;
			else if(MaxX >= (int)m_width)
				MaxX = m_width - 1;
			if(MaxY < 0)
				MaxY = 0;
			else if(MaxY >= (int)m_height)
				MaxY = m_height - 1;

			for(int x = MinX; x <= MaxX; x++)
				for(int y = MinY; y <= MaxY; y++)
				{
					vector<ObjectHandle *> localFound = m_zones[x][y]->BoxSearch(minX, minY, maxX, maxY, mask, ignore);
					for(vector<ObjectHandle *>::iterator it = localFound.begin(); it != localFound.end(); it++)
					{
						ObjectHandle *entity = *it;
						if(entity->Cache(1))
							found.push_back(entity);
					}
				}
			ObjectHandle::ClearCache(1);

			return found;
		}
		vector<DefaultPhysicsHandler::ObjectHandle *> DefaultPhysicsHandler::Plane::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, ObjectHandle *ignore)
		{
			vector<ObjectHandle *> found;

			//- determine which zones intersect the line segment -
			vector<Zone *> searchZones;

			//- handle the vertical line case -
			if(!(endX - startX))
			{
				int X = (int)floor(startX / m_zoneSize);
				int MinY = (int)floor(startY / m_zoneSize);
				int MaxY = (int)floor(endY / m_zoneSize);
				if(MinY > MaxY)
					swap(MinY, MaxY);
				if(MinY < 0)
					MinY = 0;
				else if(MinY >= (int)m_height)
					MinY = m_height - 1;
				if(MaxY < 0)
					MaxY = 0;
				else if(MaxY >= (int)m_height)
					MaxY = m_height - 1;
				for(int a = MinY; a <= MaxY; a++)
					searchZones.push_back(m_zones[X][a]);
			}
			//- handle the horizonal line case -
			else if(!(endY - startY))
			{
				int Y = (int)floor(startY / m_zoneSize);
				int MinX = (int)floor(startX / m_zoneSize);
				int MaxX = (int)floor(endX / m_zoneSize);
				if(MinX > MaxX)
					swap(MinX, MaxX);
				if(MinX < 0)
					MinX = 0;
				else if(MinX >= (int)m_width)
					MinX = m_width - 1;
				if(MaxX < 0)
					MaxX = 0;
				else if(MaxX >= (int)m_width)
					MaxX = m_width - 1;
				for(int a = MinX; a <= MaxX; a++)
					searchZones.push_back(m_zones[a][Y]);
			}
			//- handle the diagonal line case -
			else
			{
				float slope = (endY - startY) / (endX - startX);
				int MinX = (int)floor(startX / m_zoneSize);
				int MaxX = (int)floor(endX / m_zoneSize);
				if(MinX > MaxX)
					swap(MinX, MaxX);
				if(MinX < 0)
					MinX = 0;
				else if(MinX >= (int)m_width)
					MinX = m_width - 1;
				if(MaxX < 0)
					MaxX = 0;
				else if(MaxX >= (int)m_width)
					MaxX = m_width - 1;
				float originX = MinX * m_zoneSize;
				float originY = startY - (startX - originX) * slope;
				int MinY = (int)floor(startY / m_zoneSize);
				int MaxY = (int)floor(endY / m_zoneSize);
				if(MinY > MaxY)
					swap(MinY, MaxY);
				if(MinY < 0)
					MinY = 0;
				else if(MinY >= (int)m_height)
					MinY = m_height - 1;
				if(MaxY < 0)
					MaxY = 0;
				else if(MaxY >= (int)m_height)
					MaxY = m_height - 1;
				for(int a = MinX; a <= MaxX; a++)
				{
					int i = a - MinX;
					int _MinY = (int)floor((i * m_zoneSize * slope + originY) / m_zoneSize);
					int _MaxY = (int)floor(((i + 1) * m_zoneSize * slope + originY) / m_zoneSize);
					if(_MinY > _MaxY)
						swap(_MinY, _MaxY);
					if(_MinY < MinY)
						_MinY = MinY;
					else if(_MinY > MaxY)
						_MinY = MaxY;
					if(_MaxY < MinY)
						_MaxY = MinY;
					else if(_MaxY >= MaxY)
						_MaxY = MaxY;
					for(int b = _MinY; b <= _MaxY; b++)
						searchZones.push_back(m_zones[a][b]);
				}
			}

			//- go through the zones and find which entities intersect the line segment -
			for(vector<Zone *>::iterator itA = searchZones.begin(); itA != searchZones.end(); itA++)
			{
				vector<ObjectHandle *> localFound = (*itA)->SegmentSearch(startX, startY, endX, endY, mask, ignore);
				for(vector<ObjectHandle *>::iterator itB = localFound.begin(); itB != localFound.end(); itB++)
				{
					ObjectHandle *entity = *itB;
					if(entity->Cache(1))
						found.push_back(entity);
				}
			}
			ObjectHandle::ClearCache(1);
			return found;
		}

		void DefaultPhysicsHandler::Initialize()
		{
			m_plane = new Plane(16, 16, 64.f);
			
			vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
			for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
			{
				PhysicalObject *object = (PhysicalObject *)*it;

				ObjectHandle *handle = new ObjectHandle(this, object);

				m_plane->Place(handle);
			}
		}
		void DefaultPhysicsHandler::SetupObject(PhysicalObject *object)
		{
			ObjectHandle *handle = new ObjectHandle(this, object);
			m_plane->Place(handle);
		}
		void DefaultPhysicsHandler::CleanupObject(PhysicalObject *object)
		{
			ObjectHandle *handle = (ObjectHandle *)object->GetObjectHandle();
			handle->RemoveFromZones();
			delete handle;
		}
		void DefaultPhysicsHandler::Render(float minX, float minY, float maxX, float maxY)
		{
			m_plane->Render(minX, minY, maxX, maxY);
		}
		void DefaultPhysicsHandler::Process(float dt)
		{
			m_plane->RemoveDead();
			m_plane->ProcessPhysics(dt);
		}
		void DefaultPhysicsHandler::Cleanup()
		{
			vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
			for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
			{
				PhysicalObject *object = (PhysicalObject *)*it;

				ObjectHandle *handle = (ObjectHandle *)object->GetObjectHandle();
				if(handle)
					delete handle;
			}

			delete m_plane;
		}
		vector<PhysicalObject *> DefaultPhysicsHandler::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, PhysicalObject *ignore)
		{
			if(m_plane)
			{
				ObjectHandle *ignoreHandle = 0;
				if(ignore)
					ignoreHandle = (ObjectHandle *)ignore->GetObjectHandle();
				vector<PhysicalObject *> objects;
				vector<ObjectHandle *> handles = m_plane->BoxSearch(minX, minY, maxX, maxY, mask, ignoreHandle);
				for(vector<ObjectHandle *>::iterator it = handles.begin(); it != handles.end(); it++)
					objects.push_back((*it)->GetReferenceObject());
				return objects;
			}

			return vector<PhysicalObject *>();
		}
		vector<PhysicalObject *> DefaultPhysicsHandler::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, PhysicalObject *ignore)
		{
			if(m_plane)
			{
				ObjectHandle *ignoreHandle = 0;
				if(ignore)
					ignoreHandle = (ObjectHandle *)ignore->GetObjectHandle();
				vector<PhysicalObject *> objects;
				vector<ObjectHandle *> handles = m_plane->SegmentSearch(startX, startY, endX, endY, mask, ignoreHandle);
				for(vector<ObjectHandle *>::iterator it = handles.begin(); it != handles.end(); it++)
					objects.push_back((*it)->GetReferenceObject());
				return objects;
			}

			return vector<PhysicalObject *>();
		}
	}
}
