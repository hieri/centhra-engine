#ifndef _CE_GAME2D_WORLD_H_
#define _CE_GAME2D_WORLD_H_

//- Standard Library -
#include <string>
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/TileMap.h>

namespace ce
{
	namespace game2d
	{
		class WallGrid;

		class World : public PhysicalGroup
		{
		public:
			World();
			~World();

			void Render(float minX, float minY, float maxX, float maxY, RenderContext &context);

			//- Layers -
		public:
			typedef enum LayerType
			{
				Layer_Unknown,
				Layer_Object,
				Layer_Tile,
				Layer_Wall
			} LayerType;

			class Layer
			{
			protected:
				unsigned char m_type, m_index;
				std::string m_name;
				World *m_world;

			public:
				Layer();
				virtual ~Layer();

				unsigned char GetIndex() const;
				unsigned char GetType() const;
				std::string GetName() const;
				void SetName(std::string name);

				//- User Data -
			protected:
				void *m_userData;
			public:
				void *GetUserData() const;
				void SetUserData(void *userData);

				friend class World;
			};
		protected:
			std::vector<Layer *> m_layers;
			
			//- ObjectLayer: Renders the Physical Group -
		public:
			class ObjectLayer : public Layer
			{
			public:
				ObjectLayer();

				//- Render All -
			protected:
				bool m_renderAll;
			public:
				bool IsRenderingAll() const;
				void SetRenderAll(bool renderAll);

				//- Render Mask -
			protected:
				unsigned short m_renderMask;
			public:
				unsigned short GetRenderMask() const;
				void SetRenderMask(unsigned short renderMask);

				friend class World;
			};

			//- WallLayer: Renders the Walls -
		public:
			class WallLayer : public Layer
			{
				WallGrid *m_wallGrid;

			public:
				WallLayer();
				~WallLayer();
				
				WallGrid *GetWallGrid() const;
				void SetWallGrid(WallGrid *wallGrid);

				friend class World;

				//- Scale -
			protected:
				float m_scale;
			public:
				float GetScale() const;
				void SetScale(float scale);
			};

			//- TileLayer: Renders TileMap -
		public:
			class TileLayer : public Layer, public TileMap
			{
			public:
				TileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);

				//- Scale -
			protected:
				float m_scale;
			public:
				float GetScale() const;
				void SetScale(float scale);

				//- Parallax -
			protected:
				Vector2<float> m_parallax;
			public:
				Vector2<float> GetParallax() const;
				void SetParallax(Vector2<float> parallax);
			};

			ObjectLayer *AddObjectLayer();
			WallLayer *AddWallLayer();
			TileLayer *AddTileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);
			Layer *GetLayer(unsigned char layerIdx) const;
			void RemoveLayer(unsigned char layerIdx);
			std::vector<Layer *> *GetLayers();

			//- Layer Movement -
			void MoveLayerUp(Layer *layer);
			void MoveLayerDown(Layer *layer);
			void MoveLayerToFront(Layer *layer);
			void MoveLayerToBack(Layer *layer);
			void SwapLayers(Layer *layerA, Layer *layerB);

			//- Tile Set Association -
		protected:
			std::vector<TileSet *> m_associatedTileSets;
		public:
			void AssociateTileSet(TileSet *tileSet);
			void DisassociateTileSet(TileSet *tileSet);
			bool IsTileSetAssociated(TileSet *tileSet);
			std::vector<TileSet *> *GetAssociatedTileSets();
		};
	}
}

#endif
