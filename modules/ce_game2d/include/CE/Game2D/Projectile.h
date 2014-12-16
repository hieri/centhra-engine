#ifndef _CE_GAME2D_PROJECTILE_H_
#define _CE_GAME2D_PROJECTILE_H_

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
/*		class Explosion : public PhysicalObject
		{
		protected:
			virtual void DoRender();
			virtual void OnProcess(float dt);

		public:
			typedef enum ExplosionType
			{
				Boom
			} ExplosionType;

			static void InitAssets();
			static void CleanupAssets();
			static void LoadDefinitionsFromFile(const char *file);

			unsigned long long m_timeout;
			float m_animTime;

			Explosion(ce::Vector2<float> position, unsigned long long timeout);
		};
		*/

		class ProjectileDef
		{
		public:
			static void LoadFromFile(const char *file);
			static void Cleanup();
		};

		class Projectile : public PhysicalObject
		{
		protected:
			virtual void DoRender();
			virtual void OnProcess(float dt);

			unsigned char m_projectileType;
			unsigned long long m_timeout, m_lastBoost;
			PhysicalObject *m_source;
			float m_animTime;

		public:
			typedef enum ProjectileType
			{
				Bullet,
				HoverBullet,
				EnergyBall,
				BlueBall
			} ProjectileType;

			Projectile(Vector2<float> position, unsigned char type, Vector2<float> direction, float rotation, PhysicalObject *source);
			virtual ~Projectile();

			virtual void OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);
			virtual void OnAdded(Group *group);
		};
	}
}

#endif
