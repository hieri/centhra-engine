#ifndef _CE_GAME2D_EDITOR2D_H_
#define _CE_GAME2D_EDITOR2D_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/UI/ColorCtrl.h>
#include <CE/UI/TextButtonCtrl.h>
#include <CE/UI/ScrollCtrl.h>
#include <CE/UI/TextDropDownCtrl.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/Prop.h>

namespace ce
{
	namespace ui
	{
		class Editor2DCtrl : public ui::Control
		{
			//- Tile Selector -
		protected:
			class TileSelectorCtrl : public ui::ScrollCtrl
			{
			public:
				class TileSelectCtrl : public ui::ButtonCtrl, public ui::ColorCtrl
				{
				protected:
					virtual void DoRender();

				public:
					unsigned char m_tileX, m_tileY;

					TileSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, unsigned char tileX, unsigned char tileY, Font *font);
				};

				TileSelectorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Skin *skin);

				void OnSelect(TileSelectCtrl *btn);
				void GenerateButtons(Font *font, game2d::TileSet *tileSet);

			protected:
				TileSelectCtrl *m_lastSelection;
			};
			friend class TileSelectorCtrl;
			friend class TileSelectorCtrl::TileSelectCtrl;

			friend bool Editor_TileSelectBtnDown(ui::ButtonCtrl *button);

			TileSelectorCtrl *m_tileSelectorCtrl;
			ui::TextDropDownCtrl *m_tileSetSelection;
			friend bool Editor_TileSetSelection(ui::TextDropDownCtrl *ctrl, unsigned char id);
			game2d::TileSet *m_currentTileSet;

			typedef enum TileMode
			{
				TileMode_None,
				TileMode_Placing,
				TileMode_Deleting
			} TileMode;
			unsigned char m_tileMode;
			Vector2<float> m_tileHover;
			Vector2<unsigned char> m_currentTile;
		public:
			void SelectTileSet(unsigned char tileSetId);

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

			Font *m_font;

		protected:
			virtual void DoRender();

			void StartDragging(int_canvas x, int_canvas y);
			void StopDragging();
			void StartRotating(int_canvas x, int_canvas y);
			void StopRotating();
			void CheckHover(Vector2<float> position);

		public:
			Editor2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, Skin *scrollSkin);

			virtual void OnAdded(Control *parent);
			virtual bool OnEvent(Event &event);

			//- Layer Selection -
		protected:
			ui::TextDropDownCtrl *m_layerSelection;
			game2d::World::Layer *m_currentLayer;
			friend bool Editor_OnLayerSelect(ui::TextDropDownCtrl *ctrl, unsigned char id);
		public:
			void UpdateLayerSelection();
			void SelectLayer(unsigned char layerId);

			//- Mode Selection -
		protected:
			unsigned char m_mode;
			ui::TextDropDownCtrl *m_objectModeSelection;
			friend bool Editor_OnObjectModeSelect(ui::TextDropDownCtrl *ctrl, unsigned char id);
		public:
			typedef enum EditorMode
			{
				Mode_None,
				Mode_Object,
				Mode_Prop,
				Mode_Tile,
				Mode_Wall
			} EditorMode;
			void SetMode(unsigned char mode);

			//- Camera Control -
		protected:
			float m_zoom, m_minZoom, m_maxZoom;
			bool m_freeCam;
		public:
		};
	}
}

#endif
