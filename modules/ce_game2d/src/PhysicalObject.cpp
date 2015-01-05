//- Standard Library -
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine 
#include <CE/Base.h>
#include <CE/Renderer.h>
#include <CE/Quaternion.h>
#include <CE/Game2D/PhysicalObject.h>

using namespace std;

namespace ce
{
	void printMatrix(Matrix4x4<float> &mat)
	{
		for(int a = 0; a < 4; a++)
			print("%f\t%f\t%f\t%f\n", mat[a], mat[4 + a], mat[8 + a], mat[12 + a]);
		print("\n");
	}
	namespace game2d
	{
		unsigned short PhysicalObject::ms_lastID = 65535;
		unsigned short PhysicalObject::DEFAULT_ID = 65535;
		vector<PhysicalObject *> PhysicalObject::ms_objects = vector<PhysicalObject *>(1024);
		vector<PhysicalObject *> PhysicalObject::ms_netObjects = vector<PhysicalObject *>(1024);

		PhysicalObject *PhysicalObject::GetObjectByID(unsigned short id)
		{
			if(id >= ms_objects.size())
				return 0;
			return ms_objects[id];
		}
		PhysicalObject *PhysicalObject::GetNetObjectByID(unsigned short id)
		{
			if(id >= ms_netObjects.size())
				return 0;
			return ms_netObjects[id];
		}

		PhysicalObject::PhysicalObject(Vector2<float> position, Vector2<float> extent, unsigned short id, unsigned short netID, bool noID) : m_isCollidable(true), m_hasFixedRotation(false), m_renderLayer(0),
			m_mvChanged(true)
		{
			m_rotation = m_angularVelocity = 0.f;
			m_position = position;
			m_extent = extent;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
			m_velocity = Vector2<float>(0.f, 0.f);
			m_typeMask = Mask_Object;
			m_collisionMask = m_actualCollisionMask = 65535;

			m_objectHandle = 0;

			if(!noID)
			{
				if(id == DEFAULT_ID)
				{
					ms_lastID++;
					m_id = ms_lastID;
				}
				else
					m_id = id;

				if(m_id >= ms_objects.size())
					ms_objects.resize(ms_objects.size() * 2);
				ms_objects[m_id] = this;

				m_netID = netID;
				if(netID != DEFAULT_ID)
					ms_netObjects[m_netID] = this;
			}

			m_parentGroup = 0;
			m_isTrigger = m_isStatic = false;
		}
		PhysicalObject::~PhysicalObject()
		{
			if(m_parentGroup)
				m_parentGroup->Remove(this);
		}
		void PhysicalObject::Render(RenderContext &context)
		{
			DoRender(context);
		}
		void PhysicalObject::RenderAABB(RenderContext &context)
		{
			glPushMatrix();
				glColor4ub(0, 255, 0, 255);
				glBegin(GL_LINE_LOOP);
					glVertex2f(m_aabb[0], m_aabb[1]);
					glVertex2f(m_aabb[2], m_aabb[1]);
					glVertex2f(m_aabb[2], m_aabb[3]);
					glVertex2f(m_aabb[0], m_aabb[3]);
				glEnd();
				glColor3ub(255, 255, 255);
			glPopMatrix();
		}
		void PhysicalObject::DoRender(RenderContext &context)
		{
			if(m_mvChanged)
				CalculateModelViewMatrix();

			ConfigGeneric();
			ShaderProgram::GenericProgram *program = 0;
			if(context.useShaders)
				program = UseGenericProgram();
			if(program != 0)
			{
				//-------------------------- OpenGL 2.1 w/ GLSL 1.2 --------------------------
				//TODO: Switch to float based coloring??
				Matrix4x4<float> mvpMatrix = context.mvpMatrix * m_modelViewMatrix;
				glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &mvpMatrix[0]);
				glUniform4f(program->color, (float)m_color[0] / 255.f, (float)m_color[1] / 255.f, (float)m_color[2] / 255.f, (float)m_color[3] / 255.f);
			}
			else
			{
				//-------------------------- OpenGL 1.0 --------------------------
				Matrix4x4<float> mvMatrix = context.modelViewMatrix * m_modelViewMatrix;
				glLoadMatrixf(&mvMatrix[0]);
				glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);
			}
			RenderSquare(context);
		}
		Vector2<float> PhysicalObject::GetExtent() const
		{
			return m_extent;
		}
		Vector2<float> PhysicalObject::GetPosition() const
		{
			return m_position;
		}
		Vector2<float> PhysicalObject::GetVelocity() const
		{
			return m_velocity;
		}
		float PhysicalObject::GetRotation() const
		{
			return m_rotation;
		}
		float PhysicalObject::GetAngularVelocity() const
		{
			return m_angularVelocity;
		}
		void PhysicalObject::SetExtent(Vector2<float> extent, bool updateHandle)
		{
			m_extent = extent;
			OnSetExtent();
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetExtent();
			m_mvChanged = true;
		}
		void PhysicalObject::SetPosition(Vector2<float> position, bool updateHandle)
		{
			m_position = position;
			OnSetPosition();
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetPosition();
			m_mvChanged = true;
		}
		void PhysicalObject::SetVelocity(Vector2<float> velocity, bool updateHandle)
		{
			m_velocity = velocity;
			OnSetVelocity();
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetVelocity();
		}
		void PhysicalObject::SetRotation(float rotation, bool updateHandle)
		{
			m_rotation = rotation;
			OnSetRotation();
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetRotation();
			m_mvChanged = true;
		}
		void PhysicalObject::SetAngularVelocity(float angularVelocity, bool updateHandle)
		{
			m_angularVelocity = angularVelocity;
			OnSetAngularVelocity();
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetAngularVelocity();
		}
		unsigned short PhysicalObject::GetID() const
		{
			return m_id;
		}
		unsigned short PhysicalObject::GetNetID() const
		{
			return m_netID;
		}
		void PhysicalObject::SetNetID(unsigned short id)
		{
			if(m_netID != DEFAULT_ID)
				ms_netObjects[m_netID] = 0;
			if(id >= ms_netObjects.size())
				ms_netObjects.reserve(id + 1);
			ms_netObjects[id] = this;
			m_netID = id;
		}
		PhysicsHandler::ObjectHandle *PhysicalObject::GetObjectHandle() const
		{
			return m_objectHandle;
		}
		bool PhysicalObject::IsStatic() const
		{
			return m_isStatic;
		}
		bool PhysicalObject::HasFixedRotation() const
		{
			return m_hasFixedRotation;
		}
		unsigned int PhysicalObject::GetTypeMask() const
		{
			return m_typeMask;
		}
		void PhysicalObject::SetTypeMask(unsigned int mask)
		{
			m_typeMask = mask;
			OnSetTypeMask();

			if(m_objectHandle)
				m_objectHandle->OnSetTypeMask();
		}
		unsigned int PhysicalObject::GetCollisionMask() const
		{
			return m_collisionMask;
		}
		void PhysicalObject::SetCollisionMask(unsigned int mask)
		{
			m_actualCollisionMask = m_collisionMask = mask;
			OnSetCollisionMask();

			//TODO:: Determine if m_collisionMask sould be set here
			if(m_isCollidable)
				if(m_objectHandle)
					m_objectHandle->OnSetCollisionMask();
		}
		bool PhysicalObject::IsTrigger() const
		{
			return m_isTrigger;
		}
		void PhysicalObject::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
		}
		bool PhysicalObject::OnCollisionTest(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
			return true;
		}
		void PhysicalObject::SetStatic(bool isStatic)
		{
			m_isStatic = isStatic;
			OnSetStatic();

			if(m_objectHandle)
				m_objectHandle->OnSetStatic();
		}
		void PhysicalObject::SetFixedRotation(bool fixedRotation)
		{
			m_hasFixedRotation = fixedRotation;
			OnSetFixedRotation();

			if(m_objectHandle)
				m_objectHandle->OnSetFixedRotation();
		}
		bool PhysicalObject::CollidesWith(Vector2<float> pt)
		{
			if(m_objectHandle)
				return m_objectHandle->CollidesWith(pt);
			return false;
		}
		void PhysicalObject::OnSetTypeMask()
		{
		}
		void PhysicalObject::OnSetCollisionMask()
		{
		}
		void PhysicalObject::OnSetStatic()
		{
		}
		void PhysicalObject::OnSetPosition()
		{
		}
		void PhysicalObject::OnSetExtent()
		{
		}
		void PhysicalObject::OnSetVelocity()
		{
		}
		void PhysicalObject::OnSetRotation()
		{
		}
		void PhysicalObject::OnSetAngularVelocity()
		{
		}
		void PhysicalObject::OnSetFixedRotation()
		{
		}

		//- Toggle Collision -
		void PhysicalObject::SetCollidable(bool collidable)
		{
			if(collidable == m_isCollidable)
				return;
			if(collidable)
			{
				m_collisionMask = m_actualCollisionMask;
				if(m_objectHandle)
					m_objectHandle->OnSetCollisionMask();
			}
			else
			{
				m_collisionMask = 0;
				if(m_objectHandle)
					m_objectHandle->OnSetCollisionMask();
			}
			m_isCollidable = collidable;
		}
		bool PhysicalObject::IsCollidable() const
		{
			return m_isCollidable;
		}

		//- Layered Rendering -
		World::ObjectLayer *PhysicalObject::GetRenderLayer() const
		{
			return m_renderLayer;
		}
		void PhysicalObject::SetRenderLayer(World::ObjectLayer *layer)
		{
			m_renderLayer = layer;
		}

		//- Axis-Aligned Bounding Box -
		Rect<float> PhysicalObject::GetAABB() const
		{
			return m_aabb;
		}

		//- Model View Matrix -
		void PhysicalObject::CalculateModelViewMatrix()
		{
			Quaternion<float> rotation(0.f, 0.f, m_rotation, true);
			m_modelViewMatrix = Matrix4x4<float>::BuildFromTranslation(Vector3<float>(m_position[0], m_position[1], 0.f));
			m_modelViewMatrix *= rotation.GetMatrix4x4();
			m_modelViewMatrix *= Matrix4x4<float>::BuildFromScale(Vector3<float>(m_extent[0], m_extent[1], 1.f));
			m_modelViewMatrix *= Matrix4x4<float>::BuildFromTranslation(Vector3<float>(-0.5f, -0.5f, 0.f));
			m_mvChanged = false;
		}
	}
}
