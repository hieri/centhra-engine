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
#include <CE/Game2D/Sprite.h>
#include <CE/Game2D/Projectile.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		vector<Image *> g_projectileImageTable;
		typedef struct projectileDefinition
		{
			bool hasSprite;
			string spriteFile, spriteDefinitionFile; Image *image; game2d::Sprite *sprite; Vector2<float> extent, renderScale; float speed; unsigned char damage; bool axisOriented; unsigned short lifeTime; float maxAnimTime;
		} projectileDefinition;
		vector<projectileDefinition> g_projectileDefinitionTable;

		void ProjectileDef::LoadFromFile(const char *file)
		{
	/*		ifstream inFile;
			inFile.open(file);

			string in;
			while(getline(inFile, in))
			{
				stringstream lineStream(in);
				projectileDefinition def;
				getline(lineStream, def.spriteFile, '\t');
				if(!def.spriteFile.compare("//"))
					continue;
				getline(lineStream, def.spriteDefinitionFile, '\t');
				unsigned short damage;
				lineStream >> def.extent[0] >> def.extent[1] >> def.renderScale[0] >> def.renderScale[1] >> def.speed >> damage >> def.axisOriented >> def.lifeTime >> def.maxAnimTime;
				def.damage = (unsigned char)damage;
				def.hasSprite = def.spriteDefinitionFile.compare("()") != 0;
				g_projectileDefinitionTable.push_back(def);
			}

			inFile.close();
			*/
		}

		void ProjectileDef::Cleanup()
		{
/*			if(g_projectileDefinitionTable.empty() == false)
			{
				projectileDefinition *markProjDefs = &g_projectileDefinitionTable.front();
				projectileDefinition *endProjDefs = markProjDefs + g_projectileDefinitionTable.size();
				while(markProjDefs != endProjDefs)
				{
					delete markProjDefs->image;
					if(markProjDefs->hasSprite)
						delete markProjDefs->sprite;
					markProjDefs++;
				}
			}
*/		}

/*		void Projectile::InitAssets()
		{
			if(g_projectileDefinitionTable.empty() == false)
			{
				projectileDefinition *markProjDefs = &g_projectileDefinitionTable.front();
				projectileDefinition *endProjDefs = markProjDefs + g_projectileDefinitionTable.size();
				while(markProjDefs != endProjDefs)
				{
					markProjDefs->image = Image::CreateFromFile(markProjDefs->spriteFile.c_str());
					if(markProjDefs->hasSprite)
						markProjDefs->sprite = LoadSpriteFromFile(markProjDefs->spriteDefinitionFile.c_str(), markProjDefs->image);
					markProjDefs++;
				}
			}
		}
*/
//		unsigned short g_projectileColMask = Object::Mask_Body | Object::Mask_Wall;
		Projectile::Projectile(Vector2<float> position, unsigned char type, Vector2<float> direction, float rotation, PhysicalObject *source) : PhysicalObject(position, g_projectileDefinitionTable[type].extent, true), m_projectileType(type), m_source(source), m_lastBoost(0), m_timeout(0)
		{
			SetTypeMask(Mask_Projectile);
			SetCollisionMask(GetCollisionMask() & ~Mask_Projectile);

			SetVelocity(direction * g_projectileDefinitionTable[type].speed);
			if(!g_projectileDefinitionTable[type].axisOriented)
				SetRotation(rotation);
			m_timeout = getRunTimeMS() + g_projectileDefinitionTable[type].lifeTime;
		}
		Projectile::~Projectile()
		{
		}
		void Projectile::OnCollision(ce::game2d::PhysicalObject *collider, ce::Vector2<float> pointOfContact)
		{
/*			Object *object = (Object *)collider;
			AppLD30 *app = (AppLD30 *)App::GetCurrent();
			if(app->m_isServer || app->m_isLocal)
				if(object->GetTypeMask() & Object::Mask_Body)
					((Body *)object)->Damage(g_projectileDefinitionTable[m_projectileType].damage, Body::Generic, m_source);*/
			Delete();
		}
		void Projectile::OnProcess(float dt)
		{
			m_animTime += dt;
			if(m_animTime > g_projectileDefinitionTable[m_projectileType].maxAnimTime)
				m_animTime -= g_projectileDefinitionTable[m_projectileType].maxAnimTime;

			unsigned long long t = getRunTimeMS();
			if(t > m_timeout)
				Delete();
			if((t - m_lastBoost) > 500)
				SetVelocity((GetVelocity() / GetVelocity().GetLength()) * g_projectileDefinitionTable[m_projectileType].speed);
		}
		void Projectile::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTranslatef(m_position[0], m_position[1], 0.f);
			glRotatef(m_rotation, 0.f, 0.f, 1.f);
			glScalef(m_extent[0], m_extent[1], 1.f);
			glTranslatef(-0.5f, -0.5f, 0.f);

			//		g_projectileImageTable[m_projectileType]->Bind();
			glScalef(g_projectileDefinitionTable[m_projectileType].renderScale[0], g_projectileDefinitionTable[m_projectileType].renderScale[1], 1.f);
			glEnable(GL_TEXTURE_2D);
			if(g_projectileDefinitionTable[m_projectileType].hasSprite)
				g_projectileDefinitionTable[m_projectileType].sprite->Draw(0, m_animTime);
			else
			{
				g_projectileDefinitionTable[m_projectileType].image->Bind();
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
		void Projectile::OnAdded(ce::Group *group)
		{
			SetRotation(GetRotation());
			//		((plugin::box2d::bPhysicsHandler::bObjectHandle *)GetObjectHandle())->SetFixedRotation(true);
		}

		//-------------------- EXPLOSIONS???? ----------------
		/*
		void Explosion::InitAssets()
		{
		}
		void Explosion::CleanupAssets()
		{
		}
		void Explosion::LoadDefinitionsFromFile(const char *file)
		{
		}
		Explosion::Explosion(ce::Vector2<float> position, unsigned long long timeout) : Object(position, Vector2<float>(1.f, 1.f), true), m_timeout(timeout)
		{
		}
		void Explosion::DoRender()
		{
		}
		void Explosion::OnProcess(float dt)
		{
			if(getRunTimeMS() > m_timeout)
				Kill();
		}
		*/
	}
}
