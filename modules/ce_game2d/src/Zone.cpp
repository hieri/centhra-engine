//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Game2D/Plane.h>
#include <CE/Game2D/Zone.h>
#include <CE/Base.h>

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		Zone::Zone(float minX, float minY, float maxX, float maxY)
		{
			m_minX = minX;
			m_minY = minY;
			m_maxX = maxX;
			m_maxY = maxY;
			m_plane = 0;
		}
		Zone::~Zone()
		{
		}
		void Zone::Render()
		{
		//	for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
		//		(*it)->Render();

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
				FinishRender();
		}
		void Zone::Add(ZoneEntity *entity)
		{
			if(!IsMember(entity))
			{
				m_children.push_back(entity);
				entity->AddZone(this);
			}
		}
		bool Zone::IsMember(ZoneEntity *entity) const
		{
			return find(m_children.begin(), m_children.end(), entity) != m_children.end();
		}
		void Zone::Remove(ZoneEntity *entity)
		{
			vector<ZoneEntity *>::iterator it = find(m_children.begin(), m_children.end(), entity);
			if(it != m_children.end())
			{
				(*it)->RemoveZone(this);
				m_children.erase(it);
			}
		}
		void Zone::FinishRender()
		{
			for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->FinishRender();
		}
		void Zone::SetPlane(Plane *plane)
		{
			m_plane = plane;
		}
		Plane *Zone::GetPlane() const
		{
			return m_plane;
		}
		void Zone::MoveEntity(ZoneEntity *entity, Vector2<float> movement)
		{
			if(m_plane)
				m_plane->MoveEntity(entity, movement);
			else
				entity->SetPosition(entity->GetPosition() + movement);
		}
		void Zone::ProcessPhysics(float dt)
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
		void Zone::PhysicsPhase1(float dt)
		{
			for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ZoneEntity *entity = *it;
				entity->m_movement = entity->m_velocity * entity->m_movePadding * dt;

				for(int a = 0; a < 2; a++)
				{
					entity->m_moveBoxMin[a] = entity->m_position[a];
					if(entity->m_movement[a] < 0.f)
						entity->m_moveBoxMin[a] += entity->m_movement[a];
					entity->m_moveBoxMax[a] = entity->m_position[a] + entity->m_extent[a];
					if(entity->m_movement[a] > 0.f)
						entity->m_moveBoxMax[a] += entity->m_movement[a];
					entity->m_canMove[a] = true;
				}
				entity->m_finishedPhysics = false;
			}
		}
		void Zone::PhysicsPhase2(float dt)
		{
			for(vector<ZoneEntity *>::iterator itA = m_children.begin(); itA != m_children.end(); itA++)
			{
				ZoneEntity *entityA = *itA;
				if(entityA->m_velocity[0] != 0.f || entityA->m_velocity[1] != 0.f)
				{
					Vector2<float> aMin, aMax, aOMin, aOMax;
					aMin = entityA->m_moveBoxMin;
					aMax = entityA->m_moveBoxMax;
					aOMin = entityA->m_position;
					aOMax = aOMin + entityA->m_extent;

					unsigned int colCount = 0;
					vector<ZoneEntity *> diagonals;

					for(vector<ZoneEntity *>::iterator itB = m_children.begin(); itB != m_children.end(); itB++)
						if(itA != itB)
						{
							ZoneEntity *entityB = *itB;
							if(!(entityA->m_collisionMask & entityB->m_collisionMask))
								continue;
							Vector2<float> bMin, bMax, bOMin, bOMax;
							bMin = entityB->m_moveBoxMin;
							bMax = entityB->m_moveBoxMax;
							bOMin = entityB->m_position;
							bOMax = bOMin + entityB->m_extent;
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
						//entityA->m_canMove[1] = false;
						entityA->m_canMove[0] = false;
/*						for(vector<ZoneEntity *>::iterator itB = diagonals.begin(); itB != diagonals.end(); itB++)
						{
							ZoneEntity *entityB = *itB;
							//entityB->m_canMove[1] = false;
							entityB->m_canMove[0] = false;
						}*/
					}
				}
			}
		}
		void Zone::PhysicsPhase3(float dt)
		{
			for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				ZoneEntity *entity = *it;
				if(!entity->m_finishedPhysics)
				{
					entity->m_finishedPhysics = true;
					Vector2<float> velocity = entity->GetVelocity();

/*					if(!entity->m_canMove[0])
					{
						if(entity->m_movePadding[0] > 0.1f)
							entity->m_movePadding[0] /= 2.f;
						else
							entity->m_movePadding[0] = 0.1f;
					}
					if(!entity->m_canMove[1])
					{
						if(entity->m_movePadding[1] > 0.1f)
							entity->m_movePadding[1] /= 2.f;
						else
							entity->m_movePadding[1] = 0.1f;
					}
*/
					if(entity->m_canMove[0] || entity->m_canMove[1])
					{
						if(!entity->m_canMove[0])
							entity->m_movement[0] = 0.f;
						if(!entity->m_canMove[1])
							entity->m_movement[1] = 0.f;

						entity->Move(entity->m_movement);

/*						if(entity->m_canMove[0])
							entity->m_movePadding[0] = 1.f;
						if(entity->m_canMove[1])
							entity->m_movePadding[1] = 1.f;
*/					}
	/*				if(!entity->m_canMove[0])
						entity->m_velocity[0] *= -1.f;
					if(!entity->m_canMove[1])
						entity->m_velocity[1] *= -1.f;
	*/				entity->m_movement = Vector2<float>(0.f, 0.f);
				}
			}
		}
	}
}
