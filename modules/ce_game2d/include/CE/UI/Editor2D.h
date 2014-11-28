#ifndef _CE_GAME2D_EDITOR2D_H_
#define _CE_GAME2D_EDITOR2D_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/UI/ColorCtrl.h>
#include <CE/UI/TextButtonCtrl.h>
#include <CE/UI/ScrollCtrl.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/Prop.h>

namespace ce
{
	namespace ui
	{
		class Editor2DCtrl : public ui::Control
		{
			//- Prop Selector -
		protected:
			class PropSelectorCtrl : public ui::ScrollCtrl
			{
			public:
				class PropSelectCtrl : public ui::ButtonCtrl, public ui::ColorCtrl
				{
				protected:
					virtual void DoRender();

				public:
					unsigned short m_propID;
					game2d::PropDef *m_propDef;

					PropSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, unsigned short propID, Font *font);
				};

				PropSelectorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Skin *skin);

				void OnSelect(PropSelectCtrl *btn);
				void GenerateButtons(Font *font);

			protected:
				PropSelectCtrl *m_lastSelection;
			};
			
			friend bool Editor_PropSelectBtnDown(ui::ButtonCtrl *button);

			PropSelectorCtrl *m_propSelectorCtrl;
			short m_propPlaceID;

			//- Standard -
		protected:
			game2d::PhysicalObject *m_hover;
			std::vector<game2d::PhysicalObject *> m_selection;
			std::vector<std::pair<game2d::PhysicalObject *, Vector2<float> > > m_dragSelection;

			typedef struct RotateDef
			{
				game2d::PhysicalObject *object;
				float angle, offsetAngle, offsetLength;
			} RotateDef;

			std::vector<RotateDef> m_rotateSelection;
			Vector2<float> m_rotateCenter;
			Vector2<int_canvas> m_dragStart, m_curMouse;
			float m_startRotation;
			bool m_isDragging, m_isSelecting, m_isRotating;
			ui::TextButtonCtrl *m_modeObjectBtn, *m_modePropBtn, *m_modeTileBtn, *m_modeWallBtn;

			Font *m_font;

			unsigned char m_mode;

		protected:
			virtual void DoRender();

			void StartDragging(int_canvas x, int_canvas y);
			void StopDragging();
			void StartRotating(int_canvas x, int_canvas y);
			void StopRotating();
			void CheckHover(Vector2<float> position);

		public:
			typedef enum EditorMode
			{
				Mode_Object,
				Mode_Prop,
				Mode_Tile,
				Mode_Wall
			} EditorMode;

			Editor2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, Skin *scrollSkin);

			virtual bool OnEvent(Event &event);
			void SetMode(unsigned char mode);
		};
	}
}

#endif
