//- Standard Library -
#include <fstream>
#include <sstream>

#ifdef _WIN32
//- Windows -
#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/Explosion.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		map<unsigned short, ExplosionDef *> ExplosionDef::ms_explosionDefs = map<unsigned short, ExplosionDef *>();
		unsigned short ExplosionDef::ms_nextID = 0;
		map<unsigned short, ExplosionDef *> *ExplosionDef::GetDefTable()
		{
			return &ms_explosionDefs;
		}
		ExplosionDef *ExplosionDef::GetDefByID(unsigned short explosionID)
		{
			if(ms_explosionDefs.count(explosionID))
				return ms_explosionDefs[explosionID];
			return 0;
		}
		ExplosionDef *ExplosionDef::GetDefByName(string name)
		{
			for(map<unsigned short, ExplosionDef *>::iterator it = ms_explosionDefs.begin(); it != ms_explosionDefs.end(); it++)
			{
				ExplosionDef *explosionDef = it->second;
				if(!explosionDef->m_name.compare(name))
					return explosionDef;
			}
			return 0;
		}
		void ExplosionDef::LoadFromFile(const char *file)
		{
			ifstream inFile;
			inFile.open(file);

			string in, imageFile, spriteFile;
			unsigned short damage;
			while(getline(inFile, in))
			{

				//- Get name, single word only -
				stringstream lineStream(in);
				ExplosionDef *def = new ExplosionDef();
				lineStream >> def->m_name;

				//- Continue if line is a comment -
				if(!def->m_name.compare("//"))
					continue;

				//- Image File -
				getline(lineStream, imageFile, '\"'); //- Flush previous content -
				getline(lineStream, imageFile, '\"');

				//- Attributes -
				lineStream >> def->m_extent[0] >> def->m_extent[1] >> def->m_renderScale[0] >> def->m_renderScale[1] >> damage >> def->m_isAxisOriented >> def->m_lifeTime >> def->m_maxAnimTime;
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
				ms_explosionDefs[def->m_id] = def;
			}

			inFile.close();
		}
		void ExplosionDef::Cleanup()
		{
			for(map<unsigned short, ExplosionDef *>::iterator it = ms_explosionDefs.begin(); it != ms_explosionDefs.end(); it++)
				delete it->second;
			ms_explosionDefs.clear();
		}
		ExplosionDef::ExplosionDef() : m_id(0), m_image(0), m_sprite(0),
			m_isAnimated(false), m_isAxisOriented(false), m_maxAnimTime(0.f),
			m_damage(0), m_lifeTime(0)
		{
		}
		ExplosionDef::~ExplosionDef()
		{
			if(m_isAnimated)
				delete m_sprite;
			delete m_image;
		}
		unsigned short ExplosionDef::GetID() const
		{
			return m_id;
		}
		Vector2<float> ExplosionDef::GetExtent() const
		{
			return m_extent;
		}
		string ExplosionDef::GetName() const
		{
			return m_name;
		}
		Explosion *ExplosionDef::Spawn(Vector2<float> position)
		{
			return new Explosion(position, m_extent, this);
		}

		Explosion::Explosion(Vector2<float> position, Vector2<float> extent, ExplosionDef *definition, float rotation) : PhysicalObject(position, extent), m_explosionDef(definition)
		{
			SetTypeMask(Mask_Explosion);
			SetCollisionMask(GetCollisionMask() & ~(Mask_Explosion | Mask_Projectile));

			SetFixedRotation(true);
			if(!definition->m_isAxisOriented)
				SetRotation(rotation);
			m_timeout = getRunTimeMS() + definition->m_lifeTime;
			m_animTime = 0.f;
		}
		void Explosion::OnProcess(float dt)
		{
			m_animTime += dt;
//			print("Proc: %f\t%f\t%f\n", dt, m_animTime, m_explosionDef->m_maxAnimTime);
			if(m_animTime > m_explosionDef->m_maxAnimTime)
				m_animTime -= m_explosionDef->m_maxAnimTime;

			unsigned long long t = getRunTimeMS();
			if(t >= m_timeout)
				Delete();
		}
		void Explosion::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
			//TODO: Deal damage and/or apply impulse
			SetCollisionMask(0);
		}
		void Explosion::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTranslatef(m_position[0], m_position[1], 0.f);
			glRotatef(m_rotation, 0.f, 0.f, 1.f);
			glScalef(m_extent[0], m_extent[1], 1.f);
			glTranslatef(-0.5f, -0.5f, 0.f);

			glEnable(GL_TEXTURE_2D);
			if(m_explosionDef->m_isAnimated)
				m_explosionDef->m_sprite->Draw(0, m_animTime);
			else
			{
				m_explosionDef->m_image->Bind();
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

			glDisable(GL_BLEND);
		}
	}
}
