#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
	#define glGetProcAddress wglGetProcAddress
#elif __linux__
	#include <GL/glx.h>
	#define glGetProcAddress glXGetProcAddress
#endif

//- Standard Library -
#include <vector>
#include <fstream>
#include <sstream>

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/Base.h>
#include <CE/Game2D/AppGame2D.h>
#include <CE/Game2D/Sprite.h>
#include <CE/Game2D/Projectile.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		map<unsigned short, ProjectileDef *> ProjectileDef::ms_projectileDefs = map<unsigned short, ProjectileDef *>();
		unsigned short ProjectileDef::ms_nextID = 0;

		map<unsigned short, ProjectileDef *> *ProjectileDef::GetDefTable()
		{
			return &ms_projectileDefs;
		}
		ProjectileDef *ProjectileDef::GetDefByID(unsigned short projectileID)
		{
			if(ms_projectileDefs.count(projectileID))
				return ms_projectileDefs[projectileID];
			return 0;
		}
		ProjectileDef *ProjectileDef::GetDefByName(string name)
		{
			for(map<unsigned short, ProjectileDef *>::iterator it = ms_projectileDefs.begin(); it != ms_projectileDefs.end(); it++)
			{
				ProjectileDef *projectileDef = it->second;
				if(!projectileDef->m_name.compare(name))
					return projectileDef;
			}
			return 0;
		}
		void ProjectileDef::LoadFromFile(const char *file)
		{
			ifstream inFile;
			inFile.open(file);

			string in, imageFile, spriteFile;
			unsigned short damage;

			while(getline(inFile, in))
			{
				//- Get name, single word only -
				stringstream lineStream(in);
				ProjectileDef *def = new ProjectileDef();
				lineStream >> def->m_name;

				//- Continue if line is a comment -
				if(!def->m_name.compare("//"))
					continue;

				//- Image File -
				getline(lineStream, imageFile, '\"'); //- Flush previous content -
				getline(lineStream, imageFile, '\"');

				//- Attributes -
				lineStream >> def->m_extent[0] >> def->m_extent[1] >> def->m_renderScale[0] >> def->m_renderScale[1] >> def->m_speed >> damage >> def->m_isAxisOriented >> def->m_lifeTime >> def->m_maxAnimTime;
				def->m_damage = (unsigned char)damage;

				//- Read the sprite file -
				getline(lineStream, spriteFile, '\"'); //- Flush previous content -
				getline(lineStream, spriteFile, '\"');

				//- Create assets -
				def->m_image = Image::CreateFromFile(imageFile.c_str());
				if(spriteFile.length())
				{
					def->m_isAnimated = true;
					def->m_sprite = Sprite::LoadSpriteFromFile(spriteFile.c_str(), def->m_image);
				}

				//- Assign id and store to definition map -
				def->m_id = ms_nextID++;
				ms_projectileDefs[def->m_id] = def;
			}

			inFile.close();
		}
		void ProjectileDef::Cleanup()
		{
			for(map<unsigned short, ProjectileDef *>::iterator it = ms_projectileDefs.begin(); it != ms_projectileDefs.end(); it++)
				delete it->second;
			ms_projectileDefs.clear();
		}

		ProjectileDef::ProjectileDef() : m_id(0), m_image(0), m_sprite(0),
			m_isAnimated(false), m_isAxisOriented(false), m_speed(0.f), m_maxAnimTime(0.f),
			m_damage(0), m_lifeTime(0)
		{
		}
		ProjectileDef::~ProjectileDef()
		{
			if(m_isAnimated)
				delete m_sprite;
			delete m_image;
		}
		unsigned short ProjectileDef::GetID() const
		{
			return m_id;
		}
		string ProjectileDef::GetName() const
		{
			return m_name;
		}
		Projectile *ProjectileDef::Spawn(Vector2<float> position, Vector2<float> direction, PhysicalObject *source, float rotation)
		{
			return new Projectile(position, this, direction, rotation, source);
		}

		Projectile::Projectile(Vector2<float> position, ProjectileDef *definition, Vector2<float> direction, float rotation, PhysicalObject *source)
			: PhysicalObject(position, definition->m_extent, true),
			m_projectileDef(definition), m_source(source), m_lastBoost(0), m_timeout(0), m_animTime(0.f)
		{
			SetTypeMask(Mask_Projectile);
			SetCollisionMask(GetCollisionMask() & ~(Mask_Explosion | Mask_Projectile));

			SetFixedRotation(true);
			SetVelocity(direction * definition->m_speed);
			if(!definition->m_isAxisOriented)
				SetRotation(rotation);
			m_timeout = getRunTimeMS() + definition->m_lifeTime;
		}
		void Projectile::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
/*			Object *object = (Object *)collider;
			AppGame2D *app = (AppGame2D *)App::GetCurrent();
			if(app->m_isServer || app->m_isLocal)
				if(object->GetTypeMask() & Object::Mask_Body)
					((Body *)object)->Damage(m_projectileDef->m_damage, Body::Generic, m_source);*/
			Delete();
		}
		void Projectile::OnProcess(float dt)
		{
			m_animTime += dt;
			if(m_animTime > m_projectileDef->m_maxAnimTime)
				m_animTime -= m_projectileDef->m_maxAnimTime;

			unsigned long long t = getRunTimeMS();
			if(t > m_timeout)
				Delete();
			//TODO: Make velocity constant
			if((t - m_lastBoost) > 500)
				SetVelocity((GetVelocity() / GetVelocity().GetLength()) * m_projectileDef->m_speed);
		}
		void Projectile::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTranslatef(m_position[0], m_position[1], 0.f);
			glRotatef(m_rotation, 0.f, 0.f, 1.f);
			glScalef(m_extent[0], m_extent[1], 1.f);
			glTranslatef(-0.5f, -0.5f, 0.f);

			glScalef(m_projectileDef->m_renderScale[0], m_projectileDef->m_renderScale[1], 1.f);
			glEnable(GL_TEXTURE_2D);
			if(m_projectileDef->m_isAnimated)
				m_projectileDef->m_sprite->Draw(0, m_animTime);
			else
			{
				m_projectileDef->m_image->Bind();
				glBegin(GL_QUADS);
					glTexCoord2i(0, 1);
					glVertex2i(0, 0);

					glTexCoord2i(1, 1);
					glVertex2i(1, 0);

					glTexCoord2i(1, 0);
					glVertex2i(1, 1);

					glTexCoord2i(0, 0);
					glVertex2i(0, 1);
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);

			glColor3ub(255, 255, 255);

			glDisable(GL_BLEND);
		}
	}
}
