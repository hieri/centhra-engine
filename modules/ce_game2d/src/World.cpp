#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/World.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		World::World() : PhysicalGroup()
		{
		}
		World::~World()
		{
			for(vector<Layer *>::iterator it = m_layers.begin(); it != m_layers.end(); it++)
				delete *it;
		}
		void World::Render(float minX, float minY, float maxX, float maxY)
		{
			Vector2<float> center((minX + maxX) / 2.f, (minY + maxY) / 2.f);

			for(vector<Layer *>::iterator it = m_layers.begin(); it != m_layers.end(); it++)
			{
				Layer *layer = *it;

				switch(layer->m_type)
				{
				case Layer_Object:
					{
						ObjectLayer *objectLayer = (ObjectLayer *)layer;
						objectLayer->m_isRendering = true;
						PhysicalGroup::Render(minX, minY, maxX, maxY);
						objectLayer->m_isRendering = false;
					}
					break;
				case Layer_Tile:
					break;
				}
			}
		}
		World::ObjectLayer *World::AddObjectLayer()
		{
			ObjectLayer *objectLayer = new ObjectLayer();
			objectLayer->m_index = (unsigned char)m_layers.size();
			m_layers.push_back(objectLayer);
			return objectLayer;
		}
		World::TileLayer *World::AddTileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet)
		{
			TileLayer *tileLayer = new TileLayer(size, tileSize, tileSet);
			tileLayer->m_index = (unsigned char)m_layers.size();
			m_layers.push_back(tileLayer);
			return tileLayer;
		}
		World::Layer *World::GetLayer(unsigned char layerIdx) const
		{
			if(layerIdx < m_layers.size())
				return m_layers[layerIdx];
			return 0;
		}
		void World::RemoveLayer(unsigned char layerIdx)
		{
			if(layerIdx < m_layers.size())
			{
				delete m_layers[layerIdx];
				m_layers.erase(m_layers.begin() + layerIdx);

				if(layerIdx < m_layers.size())
					for(; layerIdx < m_layers.size(); layerIdx++)
						m_layers[layerIdx]->m_index = layerIdx;
			}
		}

		World::Layer::Layer() : m_index(255), m_type(Layer_Unknown)
		{
		}
		unsigned char World::Layer::GetIndex() const
		{
			return m_index;
		}

		World::ObjectLayer::ObjectLayer() : World::Layer(), m_isRendering(false)
		{
			m_type = Layer_Object;
		}
		bool World::ObjectLayer::IsRendering() const
		{
			return m_isRendering;
		}

		World::TileLayer::TileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet) : World::Layer(), TileMap(size, tileSize, tileSet)
		{
			m_type = Layer_Tile;
		}
		Vector2<float> World::TileLayer::GetParallax() const
		{
			return m_parallax;
		}
		void World::TileLayer::SetParallax(Vector2<float> parallax)
		{
			m_parallax = parallax;
		}
	}
}
