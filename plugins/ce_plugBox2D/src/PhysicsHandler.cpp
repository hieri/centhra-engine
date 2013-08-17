//- Box2D -
#include <Box2D/Box2D.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>

using namespace std;

#define PI 3.14159

const float radToDeg = 180.f / 3.14159f;

namespace ce
{
	namespace plugin
	{
		namespace box2d
		{
			class Box2DSystem
			{
			public:
				b2World *m_b2d_world;

				Box2DSystem(Vector2<float> gravity)
				{
					b2Vec2 b2d_gravity;
					b2d_gravity.Set(gravity[0], gravity[1]);
					m_b2d_world = new b2World(b2d_gravity);
					m_b2d_world->SetContinuousPhysics(true);
				}
				~Box2DSystem()
				{
					delete m_b2d_world;
				}
				void Process(float dt)
				{
					// float32 time, velIterations, posIterations
					m_b2d_world->Step(dt, 10, 10);

					unsigned int count = m_b2d_world->GetBodyCount();
					b2Body *body = m_b2d_world->GetBodyList();
					for(unsigned int a = 0; a < count; a++)
					{
						bPhysicsHandler::bObjectHandle *objectHandle = (bPhysicsHandler::bObjectHandle *)body->GetUserData();
						b2Vec2 pos = body->GetPosition();
						float rot = body->GetAngle();
						game2d::PhysicalObject *pObj = objectHandle->GetReferenceObject();
						pObj->SetPosition(Vector2<float>(pos.x, pos.y), false);
						pObj->SetRotation(radToDeg * rot);
						body = body->GetNext();
					}
				}
				void Render()
				{
					m_b2d_world->DrawDebugData();
				}
			};
			
			bPhysicsHandler::bObjectHandle::bObjectHandle(PhysicsHandler *handler, game2d::PhysicalObject *object) : PhysicsHandler::ObjectHandle(handler, object)
			{
				bPhysicsHandler *bHandler = (bPhysicsHandler *)handler;
				Box2DSystem *system = (Box2DSystem *)bHandler->GetBox2DSystem();

				b2World *world = system->m_b2d_world;

				Vector2<float> position = object->GetPosition();
				Vector2<float> extent = object->GetExtent();
				Vector2<float> velocity = object->GetVelocity();

				b2PolygonShape shape;
				shape.SetAsBox(extent[0] / 2.f, extent[1] / 2.f);

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 1.0f;
				fd.friction = 0.3f;
//				fd.filter.categoryBits = 0x0001;	// collision mask stuff?
//				fd.filter.maskBits = 0xFFFF;// & ~0x0002; <- meant that floor didn't collide with ropes

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(position[0], position[1]);

//				fd.filter.categoryBits = 0x0002; <- was rope mask
				bd.angularDamping = 0.7f;

				b2Body *b2d_body = world->CreateBody(&bd);
				b2d_body->CreateFixture(&fd);
				m_b2d_body = b2d_body;
				b2d_body->SetUserData(this);
			}
			bPhysicsHandler::bObjectHandle::~bObjectHandle()
			{
			}
			void bPhysicsHandler::bObjectHandle::OnCreate()
			{
			}
			void bPhysicsHandler::bObjectHandle::OnRelocate(game2d::PhysicalGroup *oldGroup, game2d::PhysicalGroup *newGroup)
			{
			}
			void bPhysicsHandler::bObjectHandle::OnDestroy()
			{
			}
			void bPhysicsHandler::bObjectHandle::OnSetPosition()
			{
				Vector2<float> position = m_object->GetPosition();
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2Vec2 pos(position[0], position[1]);
				b2d_body->SetTransform(pos, 0.f);
			}
			void bPhysicsHandler::bObjectHandle::OnSetExtent()
			{
				
			}
			void bPhysicsHandler::bObjectHandle::OnSetVelocity()
			{
				Vector2<float> velocity = m_object->GetVelocity();
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2Vec2 vel(velocity[0], velocity[1]);
				b2d_body->SetLinearVelocity(vel);
			}

			bPhysicsHandler::bPhysicsHandler()
			{
				m_b2d_system = 0;
			}
			bPhysicsHandler::~bPhysicsHandler()
			{
				if(m_b2d_system)
					delete m_b2d_system;
			}
			void bPhysicsHandler::Initialize()
			{
/*
				m_destructionListener.test = this;
				m_world->SetDestructionListener(&m_destructionListener);
				m_world->SetContactListener(this);
				m_world->SetDebugDraw(&m_debugDraw);
*/
				m_b2d_system = new Box2DSystem(Vector2<float>(0, -10.f));

				vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
				for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
				{
					game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;

					bObjectHandle *handle = new bObjectHandle(this, object);
				}
			}
			void bPhysicsHandler::Render(float minX, float minY, float maxX, float maxY)
			{
				vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
				for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
				{
					game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;
					object->Render();
				}

				((Box2DSystem *)m_b2d_system)->Render();
			}
			void bPhysicsHandler::Process(float dt)
			{
				((Box2DSystem *)m_b2d_system)->Process(dt);
			}
			void bPhysicsHandler::Cleanup()
			{
				vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
				for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
				{
					game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;

					bObjectHandle *handle = (bObjectHandle *)object->GetObjectHandle();
					if(handle)
						delete handle;
				}
			}
			void *bPhysicsHandler::GetBox2DSystem() const
			{
				return m_b2d_system;
			}
		}
	}
}
