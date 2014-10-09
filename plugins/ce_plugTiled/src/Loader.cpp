//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Plugin/Tiled/Loader.h>

//- PugiXML -
#include <pugixml.hpp>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;
using namespace pugi;

namespace ce
{
	namespace plugin
	{
		namespace tiled
		{
			Loader::TileSet *resolveTileset(vector<Loader::TileSet *> &ts, unsigned short gid)
			{
				for(vector<Loader::TileSet *>::reverse_iterator it = ts.rbegin(); it != ts.rend(); it++)
				{
					Loader::TileSet *tileSet = *it;
					if(tileSet->firstGID <= gid)
						return tileSet;
				}
				return NULL;
			}
			Vector2<unsigned char> gidToCoord(unsigned short gid, Loader::TileSet *ts)
			{
				Vector2<unsigned short> size = ts->tileSet->GetSize();

				gid -= ts->firstGID;

				int x = 0; int y = 0;
				x = gid % size[0];
				y = (gid - x + 1) / size[0];

				return Vector2<unsigned char>(x, y);
			}

			Loader::Layer::Layer()
			{
				m_type = Unknown;
				m_width = m_height = 0;
				m_renderView = false;
			}
			void Loader::Layer::Render(ui::CameraView2DCtrl *viewCtrl)
			{
			}

			Loader::TileLayer::TileLayer() : Layer()
			{
				m_type = Tile;
				m_tileMap = 0;
			}
			Loader::TileLayer::~TileLayer()
			{
				delete m_tileMap;
			}
			void Loader::TileLayer::Render(ui::CameraView2DCtrl *viewCtrl)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glPushMatrix();
				game2d::Camera *camera = viewCtrl->GetCamera();
				if(camera)
				{
					game2d::PhysicalObject *focus = camera->GetFocus();

					if(focus)
					{
						Vector2<float> focusPosition = focus->GetPosition();
						Vector2<float> focusExtent = focus->GetExtent();

						Vector2<float> viewScale = viewCtrl->GetViewScale();
						Vector2<int_canvas> viewExtent = viewCtrl->GetExtent();

						glPushMatrix();
						Vector2<float> half((float)viewExtent[0] / 2.f, (float)viewExtent[1] / 2.f);
						Vector2<float> focusHalf = focusExtent / 2.f;
						glTranslatef(half[0] - viewScale[0] * focusPosition[0], half[1] - viewScale[1] * focusPosition[1], 0.f);
		//						glTranslatef(half[0] - m_viewScale[0] * (focusPosition[0] + focusHalf[0]), half[1] - m_viewScale[1] * (focusPosition[1] + focusHalf[1]), 0.f);
						glScalef(viewScale[0], viewScale[1], 1.f);

						float minX, minY, maxX, maxY;
						minX = ((focusPosition[0] + focusHalf[0]) - half[0] / viewScale[0]);
						minY = ((focusPosition[1] + focusHalf[1]) - half[1] / viewScale[1]);
						maxX = ((focusPosition[0] + focusHalf[0]) + half[0] / viewScale[0]);
						maxY = ((focusPosition[1] + focusHalf[1]) + half[1] / viewScale[1]);

						game2d::TileMap *tileMap = m_tileMap;
						float scale = 1.f/32.f;//1.f;
						Vector2<unsigned short> tileSize = tileMap->GetTileSize();
						int _minX, _minY, _maxX, _maxY;
						_minX = (int)((minX / tileSize[0]) / scale) - 1;
						_maxX = (int)((maxX / tileSize[0]) / scale) + 1;
						_minY = (int)((minY / tileSize[1]) / scale) - 1;
						_maxY = (int)((maxY / tileSize[1]) / scale) + 1;						

						if(_minX < 0)
							_minX = 0;
						if(_minY < 0)
							_minY = 0;
						if(_maxX < 0)
							_maxX = 0;
						if(_maxY < 0)
							_maxY = 0;

						glScalef(scale, scale, 1.f);
						tileMap->Render(_minX, _minY, _maxX, _maxY);

						glPopMatrix();
					}
				}
				glPopMatrix();

				glDisable(GL_BLEND);

				if(m_renderView)
					viewCtrl->Render();
			}

			Loader::ObjectLayer::ObjectLayer() : Layer()
			{
				m_type = Object;
			}
			Loader::ObjectLayer::~ObjectLayer()
			{
				for(vector<ObjectDef *>::iterator it = m_objectDefVec.begin(); it != m_objectDefVec.end(); it++)
					delete *it;
			}
			void Loader::ObjectLayer::Render(ui::CameraView2DCtrl *viewCtrl)
			{
				if(m_renderView)
					viewCtrl->Render();
			}

			Loader::ImageLayer::ImageLayer() : Layer()
			{
				m_type = Image;
			}
			Loader::ImageLayer::~ImageLayer()
			{
			}
			void Loader::ImageLayer::Render(ui::CameraView2DCtrl *viewCtrl)
			{
				if(m_renderView)
					viewCtrl->Render();
			}

			Loader::ObjectDef::ObjectDef()
			{
				m_type = Unknown;
				m_width = m_height = m_gid = 0;
				m_x = m_y = 0;
			}

			Loader *Loader::CreateFromFile(const char *file, Loader *loader)
			{
				xml_document doc;
				xml_parse_result res = doc.load_file(file);

				string prefix = filePath(file);

				if(!res)
				{
					error("Error parsing Tiled xml!\n    %s\n", res.description());
					return 0;
				}

				xml_node xMap = doc.child("map");

				unsigned short mapWidth = xMap.attribute("width").as_uint();
				unsigned short mapHeight = xMap.attribute("height").as_uint();

				unsigned short tileWidth = xMap.attribute("tilewidth").as_uint();
				unsigned short tileHeight = xMap.attribute("tileheight").as_uint();
				Vector2<unsigned short> tileSize(tileWidth, tileHeight);

				vector<TileSet *> tilesets;
				vector<Layer *> layerVec;
				for(xml_node xNode = xMap.first_child(); xNode; xNode = xNode.next_sibling())
				{
					Layer *layer = 0;
					string name(xNode.name());
					if(!name.compare("tileset"))
					{
						TileSet *ts = new TileSet();
						ts->name = xNode.attribute("name").as_string();
						ts->file = xNode.child("image").attribute("source").as_string();
						ts->firstGID = xNode.attribute("firstgid").as_uint();
						ts->tileWidth = tileWidth;
						ts->tileHeight = tileHeight;
						ts->imageWidth = xNode.child("image").attribute("width").as_uint();
						ts->imageHeight = xNode.child("image").attribute("height").as_uint();
						string tilesetFile (prefix);
						tilesetFile.append(ts->file);
						ts->image = Image::CreateFromFile(tilesetFile.c_str());
						ts->tileSet = new game2d::TileSet(ts->image, tileSize);
						tilesets.push_back(ts);
					}
					else if(!name.compare("layer"))
					{
						game2d::ComplexTileMap *tileMap = new game2d::ComplexTileMap(Vector2<unsigned short>(mapWidth, mapHeight), Vector2<unsigned short>(tileWidth, tileHeight));

						unsigned short x = 0, y = 0;
						for(xml_node tile = xNode.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
						{
							unsigned short gid = tile.attribute("gid").as_uint();
							if(gid)
							{
								TileSet *ts = resolveTileset(tilesets, gid);
								if(ts)
								{
									Vector2<unsigned char> coord = gidToCoord(gid, ts);
			 						tileMap->AddTile(x, mapHeight - y - 1, ts->tileSet, coord);
								}
							}

							x++;
							if(x >= mapWidth)
							{
								x = 0;
								y++;
							}
						}

						TileLayer *tileLayer = new TileLayer();
						tileLayer->m_tileMap = tileMap;
						layer = tileLayer;
					}
					else if(!name.compare("objectgroup"))
					{
						ObjectLayer *objectLayer = new ObjectLayer();
						layer = objectLayer;

						for(xml_node object = xNode.child("object"); object; object = object.next_sibling("object"))
						{
							ObjectDef *objectDef = new ObjectDef;
							objectDef->m_type = ObjectDef::Unknown;
							objectDef->m_x = (short)object.attribute("x").as_int();
							objectDef->m_y = (short)object.attribute("y").as_int();

							xml_attribute type = object.attribute("type");
							objectDef->m_typeStr = type.as_string();

							xml_attribute attrGID = object.attribute("gid");
							if(attrGID)
							{
								objectDef->m_type = ObjectDef::Tile;
								objectDef->m_gid = attrGID.as_uint();
							}

							xml_node polyline = object.child("polyline");
							if(polyline)
							{
								objectDef->m_type = ObjectDef::Polyline;
								//TODO: Give this property a legitimate member in the class
								objectDef->m_propertyMap["points"] = polyline.attribute("points").as_string();
							}

							xml_node polygon = object.child("polygon");
							if(polygon)
							{
								objectDef->m_type = ObjectDef::Polygon;
							}

							xml_attribute width = object.attribute("width");
							xml_attribute height = object.attribute("height");
							if(width)
							{
								objectDef->m_width = width.as_uint();
								objectDef->m_height = height.as_uint();

								xml_node ellipse = object.child("ellipse");
								if(ellipse)
									objectDef->m_type = ObjectDef::Ellipse;
								else
									objectDef->m_type = ObjectDef::Rectangle;
							}
							else if(objectDef->m_type == ObjectDef::Unknown)
								objectDef->m_type = ObjectDef::Point;

							xml_attribute name = object.attribute("name");
							if(name)
								objectDef->m_name = name.as_string();

							xml_node properties = object.child("properties");
							if(properties)
								for(xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
									objectDef->m_propertyMap[property.attribute("name").as_string()] = property.attribute("value").as_string();

							objectLayer->m_objectDefVec.push_back(objectDef);
						}
					}
					else if(!name.compare("imagelayer"))
					{
						ImageLayer *imageLayer = new ImageLayer();
						layer = imageLayer;
						imageLayer->m_imageFile = xNode.child("image").attribute("source").as_string();
					}

					if(layer)
					{
						xml_node properties = xNode.child("properties");
						if(properties)
							for(xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
							{
								string propertyName(property.attribute("name").as_string());
								layer->m_propertyMap[propertyName] = property.attribute("value").as_string();
								if(!propertyName.compare("renderView"))
									layer->m_renderView = property.attribute("value").as_bool();
							}

						layer->m_name = xNode.attribute("name").as_string();
						layer->m_width = xNode.attribute("width").as_uint();
						layer->m_height = xNode.attribute("height").as_uint();
						layerVec.push_back(layer);
					}
				}

				if(!loader)
					loader = new Loader();
				loader->m_file = file;
				loader->m_tileSetVec = tilesets;
				loader->m_size = Vector2<unsigned short>(mapWidth, mapHeight);
				loader->m_layerVec = layerVec;

				xml_node properties = xMap.child("properties");
				if(properties)
				for(xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
					loader->m_propertyMap[property.attribute("name").as_string()] = property.attribute("value").as_string();

				for(vector<Layer *>::iterator it = layerVec.begin(); it != layerVec.end(); it++)
				{
					Layer *layer = *it;
					if(layer->m_type == Object)
					{
						ObjectLayer *objectLayer = (ObjectLayer *)layer;
						for(vector<ObjectDef *>::iterator itB = objectLayer->m_objectDefVec.begin(); itB != objectLayer->m_objectDefVec.end(); itB++)
							loader->LoadObject(layer, *itB);
					}
				}

				return loader;
			}

			Loader::Loader()
			{
			}
			Loader::~Loader()
			{
				for(vector<Layer *>::iterator it = m_layerVec.begin(); it != m_layerVec.end(); it++)
				{
					Layer *layer = *it;
					delete layer;
				}
				for(vector<TileSet *>::iterator it = m_tileSetVec.begin(); it != m_tileSetVec.end(); it++)
				{
					TileSet *tileSet = *it;
					delete tileSet->tileSet;
					delete tileSet->image;
					delete tileSet;
				}
			}
			void Loader::Render(ui::CameraView2DCtrl *viewCtrl)
			{
				for(vector<Layer *>::iterator it = m_layerVec.begin(); it != m_layerVec.end(); it++)
				{
					Layer *layer = *it;
					layer->Render(viewCtrl);
				}
			}
			void Loader::LoadObject(Layer *layer, ObjectDef *object)
			{
			}
		}
	}
}
