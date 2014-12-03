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

			public:
				Layer();

				unsigned char GetIndex() const;

				friend World;
			};
			
			//- ObjectLayer: Renders the Physical Group -
			class ObjectLayer : public Layer
			{
			protected:
				bool m_isRendering;
			public:
				ObjectLayer();
				bool IsRendering() const;

				friend World;
			};

			//- TileLayer: Renders TileMap -
			class TileLayer : public Layer, TileMap
			{
			public:
				TileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet);

				//- Parallax -
			protected:
				Vector2<float> m_parallax;
			public:
				Vector2<float> GetParallax() const;
				void SetParallax(Vector2<float> parallax);
			};

			ObjectLayer *AddObjectLayer();
			TileLayer *AddTileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet);
			Layer *GetLayer(unsigned char layerIdx) const;
			void RemoveLayer(unsigned char layerIdx);

		protected:
			std::vector<Layer *> m_layers;
		};
	}
}

#endif
