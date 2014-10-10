//- Box2D -
#include <Box2D/Box2D.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/Trigger.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>

using namespace std;

const float radToDeg = 180.f / 3.14159f;
const float degToRad = 3.14159f / 180.f;

namespace ce
{
	namespace plugin
	{
		namespace box2d
		{
			//TODO: Have recurrent calls to the collision callback during constant collision
			class B2D_ContactListener : public b2ContactListener
			{
			public:
				/// Called when two fixtures begin to touch.
				virtual void BeginContact(b2Contact* contact)
				{
					game2d::PhysicalObject *objA = ((bPhysicsHandler::bObjectHandle *)contact->GetFixtureA()->GetBody()->GetUserData())->GetReferenceObject();
					game2d::PhysicalObject *objB = ((bPhysicsHandler::bObjectHandle *)contact->GetFixtureB()->GetBody()->GetUserData())->GetReferenceObject();

					if(!objA->IsTrigger() && !objB->IsTrigger())
					{
						//TODO: Average worldManifold points if necessary
						b2WorldManifold worldManifold;
						contact->GetWorldManifold(&worldManifold);

						Vector2<float> pointOfContact(worldManifold.points[0].x, worldManifold.points[0].y);


						//TODO: Move collision test to PreSolve if possible
						if(!objA->OnCollisionTest(objB, pointOfContact) || !objB->OnCollisionTest(objA, pointOfContact))
							contact->SetEnabled(false);
						else
						{
							objA->OnCollision(objB, pointOfContact);
							objB->OnCollision(objA, pointOfContact);
						}
					}
					else if(objA->IsTrigger())
					{
						game2d::Trigger *trigger = (game2d::Trigger *)objA;
						if(!trigger->IsDeleted())
							trigger->OnObjectEnter(objB);
					}
					else
					{
						game2d::Trigger *trigger = (game2d::Trigger *)objB;
						if(!trigger->IsDeleted())
							trigger->OnObjectEnter(objB);
					}
				}

				/// Called when two fixtures cease to touch.
				virtual void EndContact(b2Contact* contact)
				{
					game2d::PhysicalObject *objA = ((bPhysicsHandler::bObjectHandle *)contact->GetFixtureA()->GetBody()->GetUserData())->GetReferenceObject();
					game2d::PhysicalObject *objB = ((bPhysicsHandler::bObjectHandle *)contact->GetFixtureB()->GetBody()->GetUserData())->GetReferenceObject();

					if(!objA->IsTrigger() && !objB->IsTrigger())
					{
					}
					else if(objA->IsTrigger())
						((game2d::Trigger *)objA)->OnObjectLeave(objB);
					else
						((game2d::Trigger *)objB)->OnObjectLeave(objA);
				}

				/// This is called after a contact is updated. This allows you to inspect a
				/// contact before it goes to the solver. If you are careful, you can modify the
				/// contact manifold (e.g. disable contact).
				/// A copy of the old manifold is provided so that you can detect changes.
				/// Note: this is called only for awake bodies.
				/// Note: this is called even when the number of contact points is zero.
				/// Note: this is not called for sensors.
				/// Note: if you set the number of contact points to zero, you will not
				/// get an EndContact callback. However, you may get a BeginContact callback
				/// the next step.
				virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
				{
					B2_NOT_USED(contact);
					B2_NOT_USED(oldManifold);
				}

				/// This lets you inspect a contact after the solver is finished. This is useful
				/// for inspecting impulses.
				/// Note: the contact manifold does not include time of impact impulses, which can be
				/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
				/// in a separate data structure.
				/// Note: this is only called for contacts that are touching, solid, and awake.
				virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
				{
					B2_NOT_USED(contact);
					B2_NOT_USED(impulse);
				}
			};

			class B2D_BoxSearchCallback : public b2QueryCallback
			{
				vector<game2d::PhysicalObject *> m_currentBoxSearch;

			public:
				unsigned int mask;

				/// Called for each fixture found in the query AABB.
				/// @return false to terminate the query.
				virtual bool ReportFixture(b2Fixture *fixture)
				{
					if(!(fixture->GetFilterData().categoryBits & mask))
						return true;
					m_currentBoxSearch.push_back(((bPhysicsHandler::bObjectHandle *)fixture->GetBody()->GetUserData())->GetReferenceObject());
					return true;
				}
				vector<game2d::PhysicalObject *> GetResults()
				{
					return m_currentBoxSearch;
				}
			};
			class B2D_SegmentSearchCallback : public b2RayCastCallback
			{
				vector<game2d::PhysicalObject *> m_currentSegmentSearch;

			public:
				unsigned int mask;

				/// Called for each fixture found in the query. You control how the ray cast
				/// proceeds by returning a float:
				/// return -1: ignore this fixture and continue
				/// return 0: terminate the ray cast
				/// return fraction: clip the ray to this point
				/// return 1: don't clip the ray and continue
				/// @param fixture the fixture hit by the ray
				/// @param point the point of initial intersection
				/// @param normal the normal vector at the point of intersection
				/// @return -1 to filter, 0 to terminate, fraction to clip the ray for
				/// closest hit, 1 to continue
				virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					if(!(fixture->GetFilterData().categoryBits & mask))
						return -1.f;
					m_currentSegmentSearch.push_back(((bPhysicsHandler::bObjectHandle *)fixture->GetBody()->GetUserData())->GetReferenceObject());
					return -1.f;
				}
				vector<game2d::PhysicalObject *> GetResults()
				{
					return m_currentSegmentSearch;
				}
			};
			class B2D_SpecialSegmentSearchCallback : public b2RayCastCallback
			{
				vector<pair<game2d::PhysicalObject *, Vector2<float> > > m_currentSegmentSearch;

			public:
				unsigned int mask;

				virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					if(!(fixture->GetFilterData().categoryBits & mask))
						return -1.f;
					m_currentSegmentSearch.push_back(pair<game2d::PhysicalObject *, Vector2<float> >(((bPhysicsHandler::bObjectHandle *)fixture->GetBody()->GetUserData())->GetReferenceObject(), Vector2<float>(point.x, point.y)));
					return -1.f;
				}
				vector<pair<game2d::PhysicalObject *, Vector2<float> > > GetResults()
				{
					return m_currentSegmentSearch;
				}
			};
			class B2D_RaycastSearchCallback : public b2RayCastCallback
			{
				pair<game2d::PhysicalObject *, Vector2<float> > m_currentRaycastSearch;

			public:
				unsigned int mask;
				B2D_RaycastSearchCallback()
				{
					mask = 0;
					m_currentRaycastSearch.first = 0;
				}

				virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					if(!(fixture->GetFilterData().categoryBits & mask))
						return -1.f;
					m_currentRaycastSearch = pair<game2d::PhysicalObject *, Vector2<float> >(((bPhysicsHandler::bObjectHandle *)fixture->GetBody()->GetUserData())->GetReferenceObject(), Vector2<float>(point.x, point.y));
					return 0;
				}
				pair<game2d::PhysicalObject *, Vector2<float> > GetResults()
				{
					return m_currentRaycastSearch;
				}
			};

			class Box2DSystem
			{
			public:
				b2World *m_b2d_world;
				B2D_ContactListener *m_contactListener;

				Box2DSystem(Vector2<float> gravity)
				{
					b2Vec2 b2d_gravity;
					b2d_gravity.Set(gravity[0], -gravity[1]);
					m_b2d_world = new b2World(b2d_gravity);
					m_b2d_world->SetContinuousPhysics(true);

					m_contactListener = new B2D_ContactListener();
					m_b2d_world->SetContactListener(m_contactListener);
				}
				~Box2DSystem()
				{
					delete m_b2d_world;
					delete m_contactListener;
				}
				void SetGravity(Vector2<float> gravity)
				{
					b2Vec2 grav(gravity[0], -gravity[1]);
					m_b2d_world->SetGravity(grav);
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
						float angularVelocity = body->GetAngularVelocity();
						game2d::PhysicalObject *pObj = objectHandle->GetReferenceObject();
						b2Vec2 vel = body->GetLinearVelocity();
						pObj->SetVelocity(Vector2<float>(vel.x, vel.y), false);
						pObj->SetPosition(Vector2<float>(pos.x, pos.y), false);
						pObj->SetRotation(radToDeg * rot, false);
						pObj->SetAngularVelocity(angularVelocity, false);
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
				fd.filter.categoryBits = object->GetTypeMask();
				fd.filter.maskBits = object->GetCollisionMask();

//				fd.filter.categoryBits = 0x0001;	// collision mask stuff?
//				fd.filter.maskBits = 0xFFFF;// & ~0x0002; <- meant that floor didn't collide with ropes

				b2BodyDef bd;
				if(object->IsStatic())
					bd.type = b2_staticBody;
				else
					bd.type = b2_dynamicBody;
				bd.position.Set(position[0], position[1]);
				bd.linearVelocity.Set(velocity[0], velocity[1]);
				bd.linearDamping = 0.5f;
				bd.awake = true;
				bd.active = true;
				bd.angularDamping = 0.7f;
				bd.userData = this;

				if(object->IsTrigger())
				{
					fd.isSensor = true;
					bd.gravityScale = 0.f;
				}
				else
				{
					fd.density = 1.0f;
					fd.friction = 0.3f;
				}

				b2Body *b2d_body = world->CreateBody(&bd);
				b2d_body->CreateFixture(&fd);
				m_b2d_body = b2d_body;
			}
			bPhysicsHandler::bObjectHandle::~bObjectHandle()
			{
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				if(b2d_body)
				{
					bPhysicsHandler *bHandler = (bPhysicsHandler *)GetHandler();
					Box2DSystem *system = (Box2DSystem *)bHandler->GetBox2DSystem();
					b2World *world = system->m_b2d_world;
					b2d_body->SetActive(false);
					world->DestroyBody(b2d_body);
				}
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
			void bPhysicsHandler::bObjectHandle::OnSetStatic()
			{
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				if(m_object->IsStatic())
					b2d_body->SetType(b2_staticBody);
				else
					b2d_body->SetType(b2_dynamicBody);
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
			void bPhysicsHandler::bObjectHandle::OnSetRotation()
			{
				float rotation = m_object->GetRotation();
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2d_body->SetTransform(b2d_body->GetPosition(), rotation * degToRad);
			}
			void bPhysicsHandler::bObjectHandle::OnSetVelocity()
			{
				Vector2<float> velocity = m_object->GetVelocity();
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2Vec2 vel(velocity[0], velocity[1]);
				b2d_body->SetLinearVelocity(vel);
			}
			void bPhysicsHandler::bObjectHandle::OnSetTypeMask()
			{
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2Fixture *fix = b2d_body->GetFixtureList();
				do
				{
					b2Filter filter = fix->GetFilterData();
					filter.categoryBits = m_object->GetTypeMask();
					fix->SetFilterData(filter);
					fix = fix->GetNext();
				} while(fix);
			}
			void bPhysicsHandler::bObjectHandle::OnSetCollisionMask()
			{
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2Fixture *fix = b2d_body->GetFixtureList();
				do
				{
					b2Filter filter = fix->GetFilterData();
					filter.maskBits = m_object->GetCollisionMask();
					fix->SetFilterData(filter);
					fix = fix->GetNext();
				}
				while(fix);
			}
			void bPhysicsHandler::bObjectHandle::OnSetAngularVelocity()
			{
				float angularVelocity = m_object->GetAngularVelocity();
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				b2d_body->SetAngularVelocity(angularVelocity);
			}
			void bPhysicsHandler::bObjectHandle::SetFixedRotation(bool fixedRotation)
			{
				b2Body *b2d_body = (b2Body *)m_b2d_body;
				if(b2d_body)
					b2d_body->SetFixedRotation(fixedRotation);
			}
			bool bPhysicsHandler::bObjectHandle::CollidesWith(Vector2<float> pt)
			{
				b2Body *body = (b2Body *)m_b2d_body;
				b2Fixture *fixture = body->GetFixtureList();
				b2Vec2 p(pt[0], pt[1]);
				return fixture->TestPoint(p);
			}

			bPhysicsHandler::bPhysicsHandler()
			{
				m_b2d_system = 0;
			}
			bPhysicsHandler::~bPhysicsHandler()
			{
				if(m_b2d_system)
					delete (Box2DSystem *)m_b2d_system;
			}
			void bPhysicsHandler::Initialize()
			{
/*
				m_destructionListener.test = this;
				m_world->SetDestructionListener(&m_destructionListener);
				m_world->SetContactListener(this);
				m_world->SetDebugDraw(&m_debugDraw);
*/
				m_b2d_system = new Box2DSystem(Vector2<float>(0, 0.f));

				vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
				for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
				{
					game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;
					SetupObject(object);
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

				if(m_b2d_system)
					((Box2DSystem *)m_b2d_system)->Render();
			}
			void bPhysicsHandler::Process(float dt)
			{
				((Box2DSystem *)m_b2d_system)->Process(dt);
			}
			void bPhysicsHandler::Cleanup()
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					vector<Group::Member *> members = GetReferenceGroup()->GetMembers();
					for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
					{
						game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;
						CleanupObject(object);
					}
				}

//				if(m_b2d_system)
//					delete (Box2DSystem *)m_b2d_system;

				PhysicsHandler::Cleanup();
			}
			void bPhysicsHandler::SetGravity(Vector2<float> gravity)
			{
				((Box2DSystem *)m_b2d_system)->SetGravity(gravity);
			}
			void *bPhysicsHandler::GetBox2DSystem() const
			{
				return m_b2d_system;
			}
			vector<game2d::PhysicalObject *> bPhysicsHandler::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, game2d::PhysicalObject *ignore)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
						b2AABB box;
						box.lowerBound.x = minX;
						box.lowerBound.y = minY;
						box.upperBound.x = maxX;
						box.upperBound.y = maxY;
						B2D_BoxSearchCallback callback;
						callback.mask = mask;
						world->QueryAABB(&callback, box);
						return callback.GetResults();
					}
				}
				return vector<game2d::PhysicalObject *>();
			}
			vector<game2d::PhysicalObject *> bPhysicsHandler::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, game2d::PhysicalObject *ignore)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
						b2Vec2 point1(startX, startY);
						b2Vec2 point2(endX, endY);
						B2D_SegmentSearchCallback callback;
						callback.mask = mask;
						world->RayCast(&callback, point1, point2);
						return callback.GetResults();
					}
				}
				return vector<game2d::PhysicalObject *>();
			}
			vector<pair<game2d::PhysicalObject *, Vector2<float> > > bPhysicsHandler::SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, game2d::PhysicalObject *ignore)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
						b2Vec2 point1(startX, startY);
						b2Vec2 point2(endX, endY);
						B2D_SpecialSegmentSearchCallback callback;
						callback.mask = mask;
						world->RayCast(&callback, point1, point2);
						return callback.GetResults();
					}
				}
				return vector<pair<game2d::PhysicalObject *, Vector2<float> > >();
			}
			pair<game2d::PhysicalObject *, Vector2<float> > bPhysicsHandler::RaycastSearch(float startX, float startY, float endX, float endY, unsigned int mask)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
						b2Vec2 point1(startX, startY);
						b2Vec2 point2(endX, endY);
						B2D_RaycastSearchCallback callback;
						callback.mask = mask;
						world->RayCast(&callback, point1, point2);
						return callback.GetResults();
					}
				}
				return pair<game2d::PhysicalObject *, Vector2<float> >();
			}

			void bPhysicsHandler::SetupObject(game2d::PhysicalObject *object)
			{
				bObjectHandle *handle = new bObjectHandle(this, object);
			}
			void bPhysicsHandler::CleanupObject(game2d::PhysicalObject *object)
			{
				bObjectHandle *handle = (bObjectHandle *)object->GetObjectHandle();
				if(handle)
					delete handle;
			}

			void bPhysicsHandler::AddJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB, Vector2<float> offsetA, Vector2<float> offsetB)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
//						b2DistanceJointDef jointDef;
//						b2WeldJointDef jointDef;
						b2RevoluteJointDef jointDef;
						jointDef.bodyA = (b2Body *)((bPhysicsHandler::bObjectHandle *)objA->GetObjectHandle())->m_b2d_body;
						jointDef.bodyB = (b2Body *)((bPhysicsHandler::bObjectHandle *)objB->GetObjectHandle())->m_b2d_body;
						jointDef.collideConnected = false;
						jointDef.localAnchorA.Set(offsetA[0], offsetA[1]);
						jointDef.localAnchorB.Set(offsetB[0], offsetB[1]);
//						jointDef.dampingRatio = 0;
//						jointDef.length = 0;
						world->CreateJoint(&jointDef);
					}
				}
			}
			void bPhysicsHandler::ChangeToCircle(game2d::PhysicalObject *obj)
			{
				b2World *world = 0;
				if(m_b2d_system)
				{
					world = ((Box2DSystem *)m_b2d_system)->m_b2d_world;
					if(world)
					{
						bPhysicsHandler::bObjectHandle *handle = (bPhysicsHandler::bObjectHandle *)obj->GetObjectHandle();
						b2Body *body = (b2Body *)handle->m_b2d_body;

						b2Fixture *fixture = body->GetFixtureList();

						b2CircleShape shape;
						shape.m_radius = obj->GetExtent()[0] / 2.f;

						b2FixtureDef fd;
						fd.shape = &shape;
						fd.filter = fixture->GetFilterData();

						if(obj->IsTrigger())
							fd.isSensor = true;
						else
						{
							fd.density = fixture->GetDensity();
							fd.friction = fixture->GetFriction();
						}

						body->DestroyFixture(fixture);
						body->CreateFixture(&fd);
					}
				}
			}
		}
	}
}
