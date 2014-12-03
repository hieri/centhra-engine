#ifndef _CE_GAME2D_WORLD_H_
#define _CE_GAME2D_WORLD_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/TileMap.h>

namespace ce
{
	namespace game2d
	{
		class World : public PhysicalGroup
		{
		public:
			World();
			~World();

			void Render(float minX, float minY, float maxX, float maxY);

			//- Layers -
		public:
			typedef enum LayerType
			{
				Layer_Unknown,
				Layer_Object,
				Layer_Tile
			} LayerType;

			class Layer
			{
			protected:
				unsigned char m_type, m_index;
				World *m_world;

			public:
				Layer();

				unsigned char GetIndex() const;

				friend class World;
			};
			
			//- ObjectLayer: Renders the Physical Group -
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

			//- TileLayer: Renders TileMap -
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
			TileLayer *AddTileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);
			Layer *GetLayer(unsigned char layerIdx) const;
			void RemoveLayer(unsigned char layerIdx);

			//- Layer Movement -
			void MoveLayerUp(Layer *layer);
			void MoveLayerDown(Layer *layer);
			void MoveLayerToFront(Layer *layer);
			void MoveLayerToBack(Layer *layer);
			void SwapLayers(Layer *layerA, Layer *layerB);

		protected:
			std::vector<Layer *> m_layers;
		};
	}
}

#endif
