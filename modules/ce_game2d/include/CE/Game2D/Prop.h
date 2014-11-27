#ifndef _CE_GAME2D_PROP_H_
#define _CE_GAME2D_PROP_H_

//- Standard Library -
#include <map>

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/Sprite.h>

namespace ce
{
	namespace game2d
	{
		class Prop;

		class PropDef
		{
			static std::map<unsigned short, PropDef *> ms_propDefs;
			static unsigned short ms_nextPropID;

			unsigned short m_propID;
			Image *m_image;
			Sprite *m_sprite;
			std::string m_typeName, m_sourceFile;
			Vector2<float> m_extent;
			bool m_isStatic, m_hasFixedRotation, m_isCollidable, m_isAnimated;

			friend Prop;

		public:
			static std::map<unsigned short, PropDef *> *GetPropDefTable();
			static PropDef *GetPropDefByID(unsigned short propID);
			static void LoadFromFile(const char *file);
			static void Cleanup();

			PropDef();
			~PropDef();

			unsigned short GetPropID() const;
			Vector2<float> GetExtent() const;

			Prop *Spawn(Vector2<float> position);
			void UIRender();
		};
		
		class Prop : public game2d::PhysicalObject
		{
		protected:
			PropDef *m_propDef;
			int m_currentAnimation;
			float m_animationTime;

			Prop(Vector2<float> position, Vector2<float> extent, PropDef *propDef);

			virtual void DoRender();

			friend PropDef;

		public:
			virtual void OnProcess(float dt);
		};
	}
}

#endif
