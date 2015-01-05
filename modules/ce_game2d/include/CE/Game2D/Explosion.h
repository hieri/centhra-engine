#ifndef _CE_GAME2D_EXPLOSION_H_
#define _CE_GAME2D_EXPLOSION_H_

//- Centhra Engine -
#include <CE/Sprite.h>
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Explosion;

		class ExplosionDef
		{
		protected:
			static std::map<unsigned short, ExplosionDef *> ms_explosionDefs;
			static unsigned short ms_nextID;

			unsigned short m_id;
			std::string m_name, m_sourceFile, m_spriteFile;
			Image *m_image;
			Sprite *m_sprite;
			bool m_isAnimated, m_isAxisOriented;
			Vector2<float> m_extent, m_renderScale;
			float m_maxAnimTime;
			unsigned char m_damage;
			unsigned short m_lifeTime;

			friend class Explosion;

		public:
			static std::map<unsigned short, ExplosionDef *> *GetDefTable();
			static ExplosionDef *GetDefByID(unsigned short propID);
			static ExplosionDef *GetDefByName(std::string name);
			static void LoadFromFile(const char *file);
			static void Cleanup();

			ExplosionDef();
			~ExplosionDef();

			unsigned short GetID() const;
			Vector2<float> GetExtent() const;
			std::string GetName() const;

			Explosion *Spawn(Vector2<float> position);
		};

		class Explosion : public PhysicalObject
		{
		protected:
			friend class ExplosionDef;

			ExplosionDef *m_explosionDef;
			unsigned long long m_timeout;
			float m_animTime;

			virtual void DoRender(RenderContext &context);
			virtual void OnProcess(float dt);

		public:
			Explosion(Vector2<float> position, Vector2<float> extent, ExplosionDef *definition, float rotation = 0.f);

			virtual void OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);
		};
	}
}

#endif
