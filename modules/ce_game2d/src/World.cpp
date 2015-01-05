//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/World.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/Wall.h>
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
			if(m_layers.empty() == false)
			{
				Layer **mark = &m_layers.front();
				Layer **end = mark + m_layers.size();
				while(mark != end)
					delete *mark++;
			}
		}
		void World::Render(float minX, float minY, float maxX, float maxY, RenderContext &context)
		{
			Vector2<float> center((minX + maxX) / 2.f, (minY + maxY) / 2.f);

			if(m_layers.empty() == false)
			{
				Layer **markLayers = &m_layers.front();
				Layer **endLayers = markLayers + m_layers.size();
				while(markLayers != endLayers)
				{
					Layer *layer = *markLayers++;

					switch(layer->m_type)
					{
					case Layer_Object:
						{
							ObjectLayer *objectLayer = (ObjectLayer *)layer;
							unsigned short renderMask = objectLayer->GetRenderMask();

							if(m_members.empty() == false)
							{
								Group::Member **markObjects = &m_members.front();
								Group::Member **endObjects = markObjects + m_members.size();
								while(markObjects != endObjects)
								{
									PhysicalObject *object = (PhysicalObject *)*markObjects++;
									if(!objectLayer->m_renderAll)
										if(object->GetRenderLayer() != objectLayer)
											continue;
									if(!(renderMask & object->GetTypeMask()))
										continue;
									Rect<float> aabb = object->GetAABB();
									if(aabb[0] > maxX || aabb[1] > maxY || aabb[2] < minX || aabb[3] < minY)
										continue;
									object->Render(context);
									if(m_renderDebug)
										object->RenderAABB(context);
								}
							}

							if(m_renderDebug)
								if(m_physicsHandler)
									m_physicsHandler->Render(minX, minY, maxX, maxY);

						}
						break;
					case Layer_Tile:
						{
							//TODO: Toggle for shaders
//							if(context.useShaders)
								glLoadMatrixf(&context.modelViewMatrix[0]);
							TileLayer *tileLayer = (TileLayer *)layer;
							tileLayer->Render(minX, minY, maxX, maxY, tileLayer->GetScale());
						}
						break;
					case Layer_Wall:
						{
							WallLayer *wallLayer = (WallLayer *)layer;

							if(m_members.empty() == false)
							{
								Group::Member **markObjects = &m_members.front();
								Group::Member **endObjects = markObjects + m_members.size();
								while(markObjects != endObjects)
								{
									PhysicalObject *object = (PhysicalObject *)*markObjects++;
									if(!(Mask_Wall & object->GetTypeMask()))
										continue;
									Rect<float> aabb = object->GetAABB();
									if(aabb[0] > maxX || aabb[1] > maxY || aabb[2] < minX || aabb[3] < minY)
										continue;
									object->Render(context);
									if(m_renderDebug)
										object->RenderAABB(context);
								}
							}
						}
						break;
					}
				}
			}
		}

		World::ObjectLayer *World::AddObjectLayer()
		{
			ObjectLayer *objectLayer = new ObjectLayer();
			objectLayer->m_world = this;
			objectLayer->m_index = (unsigned char)m_layers.size();
			m_layers.push_back(objectLayer);
			return objectLayer;
		}
		World::WallLayer *World::AddWallLayer()
		{
			WallLayer *wallLayer = new WallLayer();
			wallLayer->m_world = this;
			wallLayer->m_index = (unsigned char)m_layers.size();
			m_layers.push_back(wallLayer);
			return wallLayer;
		}
		World::TileLayer *World::AddTileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize)
		{
			TileLayer *tileLayer = new TileLayer(size, tileSize);
			tileLayer->m_world = this;
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
		vector<World::Layer *> *World::GetLayers()
		{
			return &m_layers;
		}

		//- Layer Movement -
		void World::MoveLayerUp(Layer *layer)
		{
			if(layer->m_world != this)
				return;
			unsigned short target = layer->m_index + 1;
			if(target == m_layers.size())
				return;
			SwapLayers(layer, m_layers[target]);
		}
		void World::MoveLayerDown(Layer *layer)
		{
			if(layer->m_world != this)
				return;
			if(!layer->m_index)
				return;
			SwapLayers(layer, m_layers[layer->m_index - 1]);
		}
		void World::MoveLayerToFront(Layer *layer)
		{
			if(layer->m_world != this)
				return;
			if((layer->m_index + 1) == m_layers.size())
				return;
			m_layers.erase(m_layers.begin() + layer->m_index);
			m_layers.push_back(layer);
		}
		void World::MoveLayerToBack(Layer *layer)
		{
			if(layer->m_world != this)
				return;
			if(!layer->m_index)
				return;
			m_layers.erase(m_layers.begin() + layer->m_index);
			m_layers.insert(m_layers.begin(), layer);
		}
		void World::SwapLayers(Layer *layerA, Layer *layerB)
		{
			if(layerA->m_world != this || layerB->m_world != this)
				return;

			unsigned char idxA = layerA->m_index;
			unsigned char idxB = layerA->m_index;

			m_layers[idxB] = layerA;
			layerA->m_index = idxB;

			m_layers[idxA] = layerB;
			layerB->m_index = idxA;
		}

		//-------------------------------- Layer --------------------------------
		World::Layer::Layer() : m_index(255), m_type(Layer_Unknown), m_world(0), m_userData(0)
		{
		}
		World::Layer::~Layer()
		{

		}
		unsigned char World::Layer::GetType() const
		{
			return m_type;
		}
		unsigned char World::Layer::GetIndex() const
		{
			return m_index;
		}
		string World::Layer::GetName() const
		{
			return m_name;
		}
		void World::Layer::SetName(string name)
		{
			m_name = name;
		}

		//- User Data -
		void *World::Layer::GetUserData() const
		{
			return m_userData;
		}
		void World::Layer::SetUserData(void *userData)
		{
			m_userData = userData;
		}

		//-------------------------------- ObjectLayer --------------------------------
		World::ObjectLayer::ObjectLayer() : World::Layer(), m_renderAll(false),
			m_renderMask(65535 & ~Mask_Wall)
		{
			m_type = Layer_Object;
		}

		//- Render All -
		bool World::ObjectLayer::IsRenderingAll() const
		{
			return m_renderAll;
		}
		void World::ObjectLayer::SetRenderAll(bool renderAll)
		{
			m_renderAll = renderAll;
		}

		//- Render Mask -
		unsigned short World::ObjectLayer::GetRenderMask() const
		{
			return m_renderMask;
		}

		void World::ObjectLayer::SetRenderMask(unsigned short renderMask)
		{
			m_renderMask = renderMask;
		}

		//-------------------------------- WallLayer --------------------------------
		World::WallLayer::WallLayer() : World::Layer(), m_wallGrid(0), m_scale(1.f)
		{
			m_type = Layer_Wall;
		}
		World::WallLayer::~WallLayer()
		{
			if(m_wallGrid)
				delete m_wallGrid;
		}
		WallGrid *World::WallLayer::GetWallGrid() const
		{
			return m_wallGrid;
		}
		void World::WallLayer::SetWallGrid(WallGrid *wallGrid)
		{
			m_wallGrid = wallGrid;
		}

		//- Scale -
		float World::WallLayer::GetScale() const
		{
			return m_scale;
		}
		void World::WallLayer::SetScale(float scale)
		{
			m_scale = scale;
		}

		//-------------------------------- TileLayer --------------------------------
		World::TileLayer::TileLayer(Vector2<unsigned short> size, Vector2<unsigned short> tileSize)
			: World::Layer(), TileMap(size, tileSize), m_scale(1.f)
		{
			m_type = Layer_Tile;
		}

		//- Scale -
		float World::TileLayer::GetScale() const
		{
			return m_scale;
		}
		void World::TileLayer::SetScale(float scale)
		{
			m_scale = scale;
		}

		//- Parallax -
		Vector2<float> World::TileLayer::GetParallax() const
		{
			return m_parallax;
		}
		void World::TileLayer::SetParallax(Vector2<float> parallax)
		{
			m_parallax = parallax;
		}

		//-------------------------------- Tile Set Assocation --------------------------------
		void World::AssociateTileSet(TileSet *tileSet)
		{
			if(!IsTileSetAssociated(tileSet))
			{
				m_associatedTileSets.push_back(tileSet);
				//TODO: Sort tilesets by identifier
			}
		}
		void World::DisassociateTileSet(TileSet *tileSet)
		{
			vector<TileSet *>::iterator it = find(m_associatedTileSets.begin(), m_associatedTileSets.end(), tileSet);
			if(it != m_associatedTileSets.end())
				m_associatedTileSets.erase(it);
		}
		bool World::IsTileSetAssociated(TileSet *tileSet)
		{
			return find(m_associatedTileSets.begin(), m_associatedTileSets.end(), tileSet) != m_associatedTileSets.end();
		}
		vector<TileSet *> *World::GetAssociatedTileSets()
		{
			return &m_associatedTileSets;
		}
	}
}
