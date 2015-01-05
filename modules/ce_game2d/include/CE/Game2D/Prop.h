#ifndef _CE_GAME2D_PROP_H_
#define _CE_GAME2D_PROP_H_

//- Standard Library -
#include <map>
#include <string>

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Sprite.h>

namespace ce
{
	namespace game2d
	{
		class Prop;

		class PropDef
		{
		protected:
			static std::map<unsigned short, PropDef *> ms_propDefs;
			static unsigned short ms_nextID;

			unsigned short m_id;
			Image *m_image;
			Sprite *m_sprite;
			std::string m_name, m_sourceFile;
			Vector2<float> m_extent;
			bool m_isStatic, m_hasFixedRotation, m_isCollidable, m_isAnimated;

			friend class Prop;

		public:
			static std::map<unsigned short, PropDef *> *GetDefTable();
			static PropDef *GetDefByID(unsigned short propID);
			static PropDef *GetDefByName(std::string name);
			static void LoadFromFile(const char *file);
			static void Cleanup();

			PropDef();
			~PropDef();

			unsigned short GetID() const;
			Vector2<float> GetExtent() const;
			std::string GetName() const;

			Prop *Spawn(Vector2<float> position);
			void UIRender();
		};
		
		class Prop : public PhysicalObject
		{
		protected:
			PropDef *m_propDef;
			int m_currentAnimation;
			float m_animationTime;

			Prop(Vector2<float> position, Vector2<float> extent, PropDef *definition);

			virtual void DoRender(RenderContext &context);

			friend class PropDef;

		public:
			virtual void OnProcess(float dt);
			PropDef *GetPropDef() const;
		};
	}
}

#endif
