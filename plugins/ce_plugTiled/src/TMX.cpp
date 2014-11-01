//- PugiXML -
#include <pugixml.hpp>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Plugin/Tiled/TMX.h>

using namespace std;
using namespace pugi;

//TODO: Support multiple object layers (saving/loading)

namespace ce
{
	namespace plugin
	{
		namespace tiled
		{
			TMX::TileSet *resolveTileset(vector<TMX::TileSet *> &ts, unsigned short gid)
			{
				for(vector<TMX::TileSet *>::reverse_iterator it = ts.rbegin(); it != ts.rend(); it++)
				{
					TMX::TileSet *tileSet = *it;
					if(tileSet->firstGID <= gid)
						return tileSet;
				}
				return NULL;
			}
			Vector2<unsigned char> gidToCoord(unsigned short gid, TMX::TileSet *ts)
			{
				Vector2<unsigned short> size = ts->tileSet->GetSize();

				gid -= ts->firstGID;

				int x = 0; int y = 0;
				x = gid % size[0];
				y = (gid - x + 1) / size[0];

				return Vector2<unsigned char>(x, y);
			}
			unsigned short coordToGid(Vector2<unsigned char> coord, TMX::TileSet *ts)
			{
				return coord[0] + coord[1] * ts->tileSet->GetSize()[0] + ts->firstGID;
			}

			TMX::Layer::Layer()
			{
				m_type = Unknown;
				m_width = m_height = 0;
				m_renderView = false;
				m_idx = 0;
			}
			void TMX::Layer::Render(Canvas *canvas, ui::CameraView2DCtrl *viewCtrl)
			{
			}

			TMX::TileLayer::TileLayer() : Layer()
			{
				m_type = Layer_Tile;
				m_tileMap = 0;
			}
			TMX::TileLayer::~TileLayer()
			{
				delete m_tileMap;
			}
			void TMX::TileLayer::Render(Canvas *canvas, ui::CameraView2DCtrl *viewCtrl)
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
					viewCtrl->Render(canvas);
			}

			TMX::ObjectLayer::ObjectLayer() : Layer()
			{
				m_type = Layer_Object;
			}
			TMX::ObjectLayer::~ObjectLayer()
			{
				for(vector<ObjectDef *>::iterator it = m_objectDefVec.begin(); it != m_objectDefVec.end(); it++)
					delete *it;
			}
			void TMX::ObjectLayer::Render(Canvas *canvas, ui::CameraView2DCtrl *viewCtrl)
			{
				if(m_renderView)
					viewCtrl->Render(canvas);
			}

			TMX::ImageLayer::ImageLayer() : Layer()
			{
				m_type = Layer_Image;
			}
			TMX::ImageLayer::~ImageLayer()
			{
			}
			void TMX::ImageLayer::Render(Canvas *canvas, ui::CameraView2DCtrl *viewCtrl)
			{
				if(m_renderView)
					viewCtrl->Render(canvas);
			}

			TMX::ObjectDef::ObjectDef()
			{
				m_type = Unknown;
				m_width = m_height = m_gid = 0;
				m_x = m_y = 0;
			}

			TMX *TMX::CreateFromFile(const char *file, TMX *tmx)
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
					unsigned short idx = layerVec.size();
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
			 						tileMap->AddTile(idx, gid, x, mapHeight - y - 1, ts->tileSet, coord);
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
							objectDef->m_type = ObjectDef::Object_Unknown;
							objectDef->m_x = (short)object.attribute("x").as_int();
							objectDef->m_y = (short)object.attribute("y").as_int();

							xml_attribute type = object.attribute("type");
							objectDef->m_typeStr = type.as_string();

							xml_attribute attrGID = object.attribute("gid");
							if(attrGID)
							{
								objectDef->m_type = ObjectDef::Object_Tile;
								objectDef->m_gid = attrGID.as_uint();
							}

							xml_node polyline = object.child("polyline");
							if(polyline)
							{
								objectDef->m_type = ObjectDef::Object_Polyline;
								//TODO: Give this property a legitimate member in the class
								objectDef->m_propertyMap["points"] = polyline.attribute("points").as_string();
							}

							xml_node polygon = object.child("polygon");
							if(polygon)
							{
								objectDef->m_type = ObjectDef::Object_Polygon;
							}

							xml_attribute width = object.attribute("width");
							xml_attribute height = object.attribute("height");
							if(width)
							{
								objectDef->m_width = width.as_uint();
								objectDef->m_height = height.as_uint();

								xml_node ellipse = object.child("ellipse");
								if(ellipse)
									objectDef->m_type = ObjectDef::Object_Ellipse;
								else
									objectDef->m_type = ObjectDef::Object_Rectangle;
							}
							else if(objectDef->m_type == ObjectDef::Object_Unknown)
								objectDef->m_type = ObjectDef::Object_Point;

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
						layer->m_idx = idx;
						layerVec.push_back(layer);
					}
				}

				if(!tmx)
					tmx = new TMX();
				tmx->m_file = file;
				tmx->m_tileSetVec = tilesets;
				tmx->m_tileSize = tileSize;
				tmx->m_size = Vector2<unsigned short>(mapWidth, mapHeight);
				tmx->m_layerVec = layerVec;
				tmx->m_orientation = string(xMap.attribute("orientation").as_string());
				tmx->m_version = string(xMap.attribute("version").as_string());

				xml_node properties = xMap.child("properties");
				if(properties)
					for(xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
						tmx->m_propertyMap[property.attribute("name").as_string()] = property.attribute("value").as_string();

				for(vector<Layer *>::iterator it = layerVec.begin(); it != layerVec.end(); it++)
				{
					Layer *layer = *it;
					if(layer->m_type == Layer_Object)
					{
						ObjectLayer *objectLayer = (ObjectLayer *)layer;
						for(vector<ObjectDef *>::iterator itB = objectLayer->m_objectDefVec.begin(); itB != objectLayer->m_objectDefVec.end(); itB++)
							tmx->LoadObject(layer, *itB);
					}
				}

				return tmx;
			}

			TMX::TMX()
			{
			}
			TMX::~TMX()
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
			void TMX::Render(Canvas *canvas, ui::CameraView2DCtrl *viewCtrl)
			{
				for(vector<Layer *>::iterator it = m_layerVec.begin(); it != m_layerVec.end(); it++)
				{
					Layer *layer = *it;
					layer->Render(canvas, viewCtrl);
				}
			}
			void TMX::LoadObject(Layer *layer, ObjectDef *object)
			{
			}
			void TMX::SaveToFile(const char *file)
			{
				xml_document doc;
//				doc.append_attribute("encoding").set_value("UTF-8");
				
				xml_node xMap = doc.append_child("map");
				{
					xml_attribute xAttrVersion = xMap.append_attribute("version");
					xAttrVersion.set_value(m_version.c_str());
					xMap.append_attribute("orientation").set_value(m_orientation.c_str());
					xMap.append_attribute("width").set_value(m_size[0]);
					xMap.append_attribute("height").set_value(m_size[1]);
					xMap.append_attribute("tilewidth").set_value(m_tileSize[0]);
					xMap.append_attribute("tileheight").set_value(m_tileSize[1]);
				}

				//- Properties -
				if(m_propertyMap.size())
				{
					xml_node xProperties = xMap.append_child("properties");
					for(map<string, string>::iterator it = m_propertyMap.begin(); it != m_propertyMap.end(); it++)
					{
						xml_node xProperty = xProperties.append_child("property");
						xProperty.append_attribute("name").set_value(it->first.c_str());
						xProperty.append_attribute("value").set_value(it->second.c_str());
					}
				}

				//- Tilesets -
				{
					for(vector<TileSet *>::iterator it = m_tileSetVec.begin(); it != m_tileSetVec.end(); it++)
					{
						TileSet *tileset = *it;

						xml_node xTileset = xMap.append_child("tileset");
						xTileset.append_attribute("firstgid").set_value(tileset->firstGID);
						xTileset.append_attribute("name").set_value(tileset->name.c_str());
						xTileset.append_attribute("tilewidth").set_value(tileset->tileWidth);
						xTileset.append_attribute("tileheight").set_value(tileset->tileHeight);

						xml_node xImage = xTileset.append_child("image");
						xImage.append_attribute("source").set_value(tileset->file.c_str());
						xImage.append_attribute("width").set_value(tileset->imageWidth);
						xImage.append_attribute("height").set_value(tileset->imageHeight);
					}
				}

				//- Layers -
				{
					int layerIdx = 0;
					for(vector<Layer *>::iterator it = m_layerVec.begin(); it != m_layerVec.end(); it++)
					{
						Layer *layer = *it;

						switch(layer->m_type)
						{
						case Layer_Object:
							{
								xml_node xLayer = xMap.append_child("objectgroup");
								xLayer.append_attribute("name").set_value(layer->m_name.c_str());
								xLayer.append_attribute("width").set_value(layer->m_width);
								xLayer.append_attribute("height").set_value(layer->m_height);

								bool renderView = false;
								if(layer->m_propertyMap.size())
								{
									xml_node xProperties = xLayer.append_child("properties");
									for(map<string, string>::iterator itB = layer->m_propertyMap.begin(); itB != layer->m_propertyMap.end(); itB++)
									{
										xml_node xProperty = xProperties.append_child("property");
										xProperty.append_attribute("name").set_value(itB->first.c_str());
										xProperty.append_attribute("value").set_value(itB->second.c_str());
										if(!itB->first.compare("renderView"))
											renderView = true;
									}
								}

								//TODO: Externalize this, and pass reference to object group

								ObjectLayer *objectLayer = (ObjectLayer *)layer;
								for(vector<ObjectDef *>::iterator itB = objectLayer->m_objectDefVec.begin(); itB != objectLayer->m_objectDefVec.end(); itB++)
								{
									ObjectDef *objectDef = *itB;

									if(!objectDef->m_typeStr.compare("Prop"))// || !objectDef->m_typeStr.compare("Wall"))
										continue;

									xml_node xObject = xLayer.append_child("object");
									if(objectDef->m_name.size())
										xObject.append_attribute("name").set_value(objectDef->m_name.c_str());
									if(objectDef->m_typeStr.size())
										xObject.append_attribute("type").set_value(objectDef->m_typeStr.c_str());

									xObject.append_attribute("x").set_value(objectDef->m_x);
									xObject.append_attribute("y").set_value(objectDef->m_y);

									if(objectDef->m_type == ObjectDef::Object_Rectangle || objectDef->m_type == ObjectDef::Object_Ellipse)
									{
										xObject.append_attribute("width").set_value(objectDef->m_width);
										xObject.append_attribute("height").set_value(objectDef->m_height);
									}

									if(objectDef->m_propertyMap.size())
									{
										xml_node xProperties;
										for(map<string, string>::iterator itC = objectDef->m_propertyMap.begin(); itC != objectDef->m_propertyMap.end(); itC++)
										{
											if(objectDef->m_type == ObjectDef::Object_Polyline)
												if(!itC->first.compare("points"))
													continue;

											if(!xProperties)
												xProperties = xObject.append_child("properties");
											xml_node xProperty = xProperties.append_child("property");
											xProperty.append_attribute("name").set_value(itC->first.c_str());
											xProperty.append_attribute("value").set_value(itC->second.c_str());
										}
									}

									if(objectDef->m_type == ObjectDef::Object_Polyline)
									{
										xml_node xPolyline = xObject.append_child("polyline");
										xPolyline.append_attribute("points").set_value(objectDef->m_propertyMap["points"].c_str());
									}
								}

								if(renderView)
									SaveObjects((void *)&xLayer);
							}
							break;
						case Layer_Tile:
							{
								xml_node xLayer = xMap.append_child("layer");
								xLayer.append_attribute("name").set_value(layer->m_name.c_str());
								xLayer.append_attribute("width").set_value(layer->m_width);
								xLayer.append_attribute("height").set_value(layer->m_height);

								xml_node xData = xLayer.append_child("data");
								TileLayer *tileLayer = (TileLayer *)layer;
								for(unsigned short b = layer->m_height - 1; b < layer->m_height; b--)
									for(unsigned short a = 0; a < layer->m_width; a++)
									{
										vector<game2d::ComplexTileMap::Tile *> *tiles = tileLayer->m_tileMap->GetTiles(a, b);
										unsigned short gid = 0;
										for(vector<game2d::ComplexTileMap::Tile *>::iterator itB = tiles->begin(); itB != tiles->end(); itB++)
										{
											game2d::ComplexTileMap::Tile *tile = *itB;
											if(tile->m_layerIdx == layer->m_idx)
											{
												gid = tile->m_gid;
												break;
											}
										}
										xml_node xTile = xData.append_child("tile");
										xTile.append_attribute("gid").set_value(gid);
									}
							}
							break;
						}
						layerIdx++;
					}
				}

				doc.save_file(file, " ", 1U, pugi::encoding_utf8);
			}
			void TMX::SaveObjects(void *groupLayer)
			{
			}
		}
	}
}
