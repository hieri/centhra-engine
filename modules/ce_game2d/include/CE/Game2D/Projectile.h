#ifndef _CE_GAME2D_PROJECTILE_H_
#define _CE_GAME2D_PROJECTILE_H_

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Projectile;

		class ProjectileDef
		{
		protected:
			static std::map<unsigned short, ProjectileDef *> ms_projectileDefs;
			static unsigned short ms_nextID;

			unsigned short m_id;
			std::string m_name, m_sourceFile, m_spriteFile;
			Image *m_image;
			Sprite *m_sprite;
			bool m_isAnimated, m_isAxisOriented;
			Vector2<float> m_extent, m_renderScale;
			float m_speed, m_maxAnimTime;
			unsigned char m_damage;
			unsigned short m_lifeTime;

			friend class Projectile;

		public:
			static std::map<unsigned short, ProjectileDef *> *GetDefTable();
			static ProjectileDef *GetDefByID(unsigned short projectileID);
			static ProjectileDef *GetDefByName(std::string name);
			static void LoadFromFile(const char *file);
			static void Cleanup();

			ProjectileDef();
			~ProjectileDef();

			unsigned short GetID() const;
			std::string GetName() const;

			Projectile *Spawn(Vector2<float> position, Vector2<float> direction, PhysicalObject *source, float rotation = 0.f);
		};

		class Projectile : public PhysicalObject
		{
		protected:
			friend class ProjectileDef;

			ProjectileDef *m_projectileDef;
			PhysicalObject *m_source;
			unsigned long long m_timeout, m_lastBoost;
			float m_animTime;

			Projectile(Vector2<float> position, ProjectileDef *definition, Vector2<float> direction, float rotation, PhysicalObject *source);

			virtual void DoRender(RenderContext &context);
			virtual void OnProcess(float dt);

		public:
			virtual void OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);
		};
	}
}

#endif
