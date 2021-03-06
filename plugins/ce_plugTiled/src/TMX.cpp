//- Standard Library -
#include <algorithm>
#include <sstream>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- PugiXML -
#include <pugixml.hpp>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Plugin/Tiled/TMX.h>
#include <CE/Game2D/Prop.h>
#include <CE/Game2D/Wall.h>

using namespace std;
using namespace pugi;

//TODO: Support multiple object layers (saving/loading)

namespace ce
{
	namespace plugin
	{
		namespace tiled
		{
			TMX::TileSetDef *resolveTileset(vector<TMX::TileSetDef *> &ts, unsigned short gid)
			{
				if(ts.empty() == false)
				{
					TMX::TileSetDef **markTileSets = &ts.back();
					TMX::TileSetDef **endTileSets = markTileSets - ts.size();
					while(markTileSets != endTileSets)
					{
						TMX::TileSetDef *tileSet = *markTileSets--;
						if(tileSet->firstGID <= gid)
							return tileSet;
					}
				}
				return NULL;
			}
			Vector2<unsigned char> gidToCoord(unsigned short gid, TMX::TileSetDef *ts)
			{
				Vector2<unsigned char> size = ts->tileSet->GetSize();

				gid -= ts->firstGID;

				int x = 0; int y = 0;
				x = gid % size[0];
				y = (gid - x + 1) / size[0];

				return Vector2<unsigned char>(x, y);
			}
			unsigned short coordToGid(Vector2<unsigned char> coord, TMX::TileSetDef *ts)
			{
				return coord[0] + coord[1] * ts->tileSet->GetSize()[0] + ts->firstGID;
			}

			TMX::Layer::Layer()
			{
				m_type = Layer_Unknown;
				m_width = m_height = 0;
				m_renderView = false;
				m_idx = 0;
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

			TMX::ObjectLayer::ObjectLayer() : Layer()
			{
				m_type = Layer_Object;
			}
			TMX::ObjectLayer::~ObjectLayer()
			{
				if(m_objectDefVec.empty() == false)
				{
					ObjectDef **markObjectDefs = &m_objectDefVec.front();
					ObjectDef **endObjectDefs = markObjectDefs + m_objectDefVec.size();
					while(markObjectDefs != endObjectDefs)
						delete *markObjectDefs++;
				}
			}

			TMX::ImageLayer::ImageLayer() : Layer()
			{
				m_type = Layer_Image;
			}
			TMX::ImageLayer::~ImageLayer()
			{
			}

			TMX::ObjectDef::ObjectDef()
			{
				m_type = Object_Unknown;
				m_width = m_height = m_gid = 0;
				m_x = m_y = 0;
			}

			TMX *TMX::CreateFromFile(const char *file, TMX *tmx)
			{
				xml_document doc;
				xml_parse_result res = doc.load_file(file);

				string prefix = fileBase(file);

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

				vector<TileSetDef *> tilesets;
				vector<Layer *> layerVec;
				for(xml_node xNode = xMap.first_child(); xNode; xNode = xNode.next_sibling())
				{
					Layer *layer = 0;
					string name(xNode.name());
					unsigned short idx = layerVec.size();
					if(!name.compare("tileset"))
					{
						TileSetDef *ts = new TileSetDef();
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
						ts->tileSet->SetName(ts->name);
						tilesets.push_back(ts);
					}
					else if(!name.compare("layer"))
					{
						game2d::TileMap *tileMap = new game2d::TileMap(Vector2<unsigned short>(mapWidth, mapHeight), Vector2<unsigned short>(tileWidth, tileHeight));

						unsigned short x = 0, y = 0;
						for(xml_node tile = xNode.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
						{
							unsigned short gid = tile.attribute("gid").as_uint();
							if(gid)
							{
								TileSetDef *ts = resolveTileset(tilesets, gid);
								if(ts)
								{
									//TODO: Add tilesets ahead of time and use their indices
									tileMap->AddTileSet(ts->tileSet);
									Vector2<unsigned char> coord = gidToCoord(gid, ts);
									tileMap->SetTile(x, mapHeight - y - 1, coord, tileMap->GetTileSetIndex(ts->tileSet));
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

				if(tilesets.empty() == false)
				{
					TileSetDef **markTileSetDefs = &tilesets.front();
					TileSetDef **endTileSetDefs = markTileSetDefs + tilesets.size();
					while(markTileSetDefs != endTileSetDefs)
					{
						TileSetDef *ts = *markTileSetDefs++;
						tmx->m_tileSetMap[ts->tileSet] = ts;
					}
				}

				xml_node properties = xMap.child("properties");
				if(properties)
					for(xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
						tmx->m_propertyMap[property.attribute("name").as_string()] = property.attribute("value").as_string();

/*				if(layerVec.empty() == false)
				{
					Layer **markLayers = &layerVec.front();
					Layer **endLayers = markLayers + layerVec.size();
					while(markLayers != endLayers)
					{
						Layer *layer = *markLayers++;
						if(layer->m_type == Layer_Object)
						{
							ObjectLayer *objectLayer = (ObjectLayer *)layer;
							if(objectLayer->m_objectDefVec.empty() == false)
							{
								ObjectDef **markObjectDefs = &objectLayer->m_objectDefVec.front();
								ObjectDef **endObjectDefs = markObjectDefs + objectLayer->m_objectDefVec.size();
								while(markObjectDefs != endObjectDefs)
									tmx->LoadObject(layer, *markObjectDefs++);
							}
						}
					}
				}*/

				return tmx;
			}

			TMX::TMX()
			{
			}
			TMX::~TMX()
			{
				if(m_layerVec.empty() == false)
				{
					Layer **markLayers = &m_layerVec.front();
					Layer **endLayers = markLayers + m_layerVec.size();
					while(markLayers != endLayers)
						delete *markLayers++;
				}
				if(m_tileSetVec.empty() == false)
				{
					TileSetDef **markTileSets = &m_tileSetVec.front();
					TileSetDef **endTileSets = markTileSets + m_tileSetVec.size();
					while(markTileSets != endTileSets)
					{
						TileSetDef *tileSet = *markTileSets++;
						delete tileSet->tileSet;
						delete tileSet->image;
						delete tileSet;
					}
				}
			}
			void TMX::PopulateWorld(game2d::World *world, Vector2<float> scale)
			{
				//- Associate Tile Sets -
				if(m_tileSetVec.empty() == false)
				{
					TileSetDef **markTileSets = &m_tileSetVec.front();
					TileSetDef **endTileSets = markTileSets + m_tileSetVec.size();
					while(markTileSets != endTileSets)
						world->AssociateTileSet((*markTileSets++)->tileSet);
				}

				//- Generate Layers -
				if(m_layerVec.empty() == false)
				{
					int layerIdx = 0;
					Layer **markLayers = &m_layerVec.front();
					Layer **endLayers = markLayers + m_layerVec.size();
					while(markLayers != endLayers)
					{
						Layer *layer = *markLayers++;
						switch(layer->m_type)
						{
						case Layer_Object:
							{
								//- Determine if this is a Wall layer -
								bool isWallLayer = false;
								if(layer->m_propertyMap.size())
									if(layer->m_propertyMap.count("IsWallLayer"))
									{
										string value = layer->m_propertyMap["IsWallLayer"];
										if(!value.compare("1") || !value.compare("True") || !value.compare("true"))
											isWallLayer = true;
									}

								if(isWallLayer)
								{
									ObjectLayer *objectLayer = (ObjectLayer *)layer;
									game2d::World::WallLayer *wWallLayer = world->AddWallLayer();
									wWallLayer->SetUserData(layer);
									wWallLayer->SetName(layer->m_name);
									wWallLayer->SetScale(1.f / 32.f * scale[1]);

									game2d::WallGrid *wallGrid = new game2d::WallGrid(m_size[0], m_size[1]);
									wWallLayer->SetWallGrid(wallGrid);

									unsigned short mapHeight = m_size[1] * 32;

									if(objectLayer->m_objectDefVec.empty() == false)
									{
										ObjectDef **markObjectDefs = &objectLayer->m_objectDefVec.front();
										ObjectDef **endObjectDefs = markObjectDefs + objectLayer->m_objectDefVec.size();
										while(markObjectDefs != endObjectDefs)
										{
											ObjectDef *object = *markObjectDefs++;

											if(object->m_type == ObjectDef::Object_Polyline)
											{
												if(!object->m_typeStr.compare("Wall"))
												{
													string points(object->m_propertyMap["points"]);
													replace(points.begin(), points.end(), ',', ' ');
													istringstream pointsStream(points);

													int _x, _y, c = 0;
													float x, y, lastX, lastY;

													//- Snap object position to grid -
													object->m_x = (int)(32.f * floor(((float)object->m_x) / 32.f + 0.5f));
													object->m_y = (int)(32.f * floor(((float)object->m_y) / 32.f + 0.5f));

													unsigned short minX, minY, maxX, maxY;

													while(pointsStream >> _x >> _y)
													{
														//- Snap points to grid -
														_x = (int)(32.f * floor(((float)_x) / 32.f + 0.5f));
														_y = (int)(32.f * floor(((float)_y) / 32.f + 0.5f));

														x = ((float)(object->m_x + _x)) / 32.f;
														y = ((float)(mapHeight - object->m_y - _y)) / 32.f;

														//- Populate wall grid -
														if(c)
														{
															if(lastX < x)
																minX = (unsigned short)lastX;
															else
																minX = (unsigned short)x;
															if(lastY < y)
																minY = (unsigned short)lastY;
															else
																minY = (unsigned short)y;
															if(lastX > x)
																maxX = (unsigned short)lastX;
															else
																maxX = (unsigned short)x;
															if(lastY > y)
																maxY = (unsigned short)lastY;
															else
																maxY = (unsigned short)y;

															if(minX == maxX)
																for(unsigned short b = minY; b < maxY; b++)
																	wallGrid->SetVertical(minX, b, true);
															if(minY == maxY)
																for(unsigned short a = minX; a < maxX; a++)
																	wallGrid->SetHorizontal(a, minY, true);

															if(minX == maxX && minY == maxY)
																wallGrid->SetPost(minX, minY, true);
														}

														c++;
														lastX = x;
														lastY = y;
													}

													if(c == 1)
														wallGrid->m_data[(unsigned short)x][(unsigned short)y] = true;
												}
											}
											else if(object->m_type == ObjectDef::Object_Rectangle)
											{
												if(!object->m_typeStr.compare("Wall"))
												{
													//- Snap object position to grid -
													object->m_x = (int)(32.f * floor(((float)object->m_x) / 32.f + 0.5f));
													object->m_y = (int)(32.f * floor(((float)object->m_y) / 32.f + 0.5f));

													object->m_width = (int)(32.f * floor(((float)object->m_width) / 32.f + 0.5f));
													object->m_height = (int)(32.f * floor(((float)object->m_height) / 32.f + 0.5f));

													short minX = object->m_x / 32, minY = (mapHeight - object->m_y - object->m_height) / 32;
													short maxX = minX + (short)(object->m_width / 32), maxY = minY + (short)(object->m_height / 32);

													if(minX == maxX)
													{
														if(minY == maxY)
															wallGrid->SetPost(minX, minY, true);
														else
															for(unsigned short b = minY; b < maxY; b++)
																wallGrid->SetVertical(minX, b, true);
													}
													else if(minY == maxY)
													{
														for(unsigned short a = minX; a < maxX; a++)
															wallGrid->SetHorizontal(a, minY, true);
													}
													else
													{
														for(short a = minX; a < maxX; a++)
															for(short b = minY; b < maxY; b++)
																wallGrid->SetFill(a, b, true);
													}
												}
											}
											else if(object->m_type == ObjectDef::Object_Point)
												if(!object->m_typeStr.compare("Wall"))
												{
													//- Snap object position to grid -
													object->m_x = (int)(32.f * floor(((float)object->m_x) / 32.f + 0.5f));
													object->m_y = (int)(32.f * floor(((float)object->m_y) / 32.f + 0.5f));

													short X = object->m_x / 32, Y = (mapHeight - object->m_y) / 32;
													wallGrid->SetPost(X, Y, true);
												}
										}
									}

									wallGrid->GenerateWalls(world, scale);
								}
								else
								{
									ObjectLayer *objectLayer = (ObjectLayer *)layer;
									game2d::World::ObjectLayer *wObjectLayer = world->AddObjectLayer();
									wObjectLayer->SetUserData(layer);
									wObjectLayer->SetName(layer->m_name);

									//- RenderAll: Attempts to render all objects in the World -
									bool renderAll = false;
									if(layer->m_propertyMap.size())
										if(layer->m_propertyMap.count("RenderAll"))
										{
											string value = layer->m_propertyMap["RenderAll"];
											if(!value.compare("1") || !value.compare("True") || !value.compare("true"))
												renderAll = true;
										}

									wObjectLayer->SetRenderAll(renderAll);

									//TODO: Account for render mask

									if(objectLayer->m_objectDefVec.empty() == false)
									{
										ObjectDef **markObjectDefs = &objectLayer->m_objectDefVec.front();
										ObjectDef **endObjectDefs = markObjectDefs + objectLayer->m_objectDefVec.size();
										while(markObjectDefs != endObjectDefs)
										{
											game2d::PhysicalObject *obj = LoadObject(*markObjectDefs++);
											if(!obj)
												continue;
											obj->SetRenderLayer(wObjectLayer);
											world->Add(obj);
										}
									}
								}
							}
							break;
						case Layer_Tile:
							{
								TileLayer *tileLayer = (TileLayer *)layer;
								game2d::World::TileLayer *wTileLayer = world->AddTileLayer(tileLayer->m_tileMap->GetSize(), tileLayer->m_tileMap->GetTileSize());
								wTileLayer->SetUserData(layer);
								wTileLayer->SetName(layer->m_name);
								wTileLayer->CopyFrom(tileLayer->m_tileMap);
								//TODO: Account for horizontal scale
								wTileLayer->SetScale(1.f / 32.f * scale[1]);
							}
							break;
						}
						layerIdx++;
					}
				}
			}
			game2d::PhysicalObject *TMX::LoadObject(ObjectDef *object)
			{
//				unsigned short mapWidth = m_size[0] * 32;
				unsigned short mapHeight = m_size[1] * 32;

				if(object->m_type == ObjectDef::Object_Point)
					if(!object->m_typeStr.compare("Prop"))
					{
						game2d::PropDef *propDef = game2d::PropDef::GetDefByName(object->m_propertyMap["PropName"]);
						if(!propDef)
							return 0;
						game2d::Prop *prop = propDef->Spawn(Vector2<float>((float)(object->m_x), (float)(mapHeight - object->m_y)));
//						game2d::Prop *prop = propDef->Spawn(Vector2<float>((float)(object->m_x) / 32.f, (float)(mapHeight - object->m_y) / 32.f));
						if(!prop)
							return 0;
						float rot = 0.f;
						if(object->m_propertyMap.count("Rotation"))
						{
							stringstream rotStr(object->m_propertyMap["Rotation"]);
							rotStr >> rot;
						}
						if(rot != 0.f)
							prop->SetRotation(rot);
						return prop;
					}

				return 0;
			}
			void TMX::ExportWorldToFile(game2d::World *world, const char *file)
			{
				print("Exporting World to <%s>...\n", file);

				unsigned short mapHeight = m_size[1] * 32;

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
				if(m_tileSetVec.empty() == false)
				{
					TileSetDef **markTileSets = &m_tileSetVec.front();
					TileSetDef **endTileSets = markTileSets + m_tileSetVec.size();
					while(markTileSets != endTileSets)
					{
						TileSetDef *tileset = *markTileSets++;

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
					vector<game2d::World::Layer *> *layers = world->GetLayers();
					if(layers->empty() == false)
					{
						game2d::World::Layer **markLayers = &layers->front();
						game2d::World::Layer **endLayers = markLayers + layers->size();
						while(markLayers != endLayers)
						{
							game2d::World::Layer *wlayer = *markLayers++;
							unsigned char type = wlayer->GetType();
							switch(type)
							{
							case game2d::World::Layer_Object:
								{
									//TODO: Determine saveable objects
									game2d::World::ObjectLayer *wObjectLayer = (game2d::World::ObjectLayer *)wlayer;

									xml_node xLayer = xMap.append_child("objectgroup");
									xLayer.append_attribute("name").set_value(wlayer->GetName().c_str());
									xLayer.append_attribute("width").set_value(m_size[0]);
									xLayer.append_attribute("height").set_value(m_size[1]);

									if(wObjectLayer->IsRenderingAll())
									{
										xml_node xProperties = xLayer.append_child("properties");
										xml_node xProperty = xProperties.append_child("property");
										xProperty.append_attribute("name").set_value("RenderAll");
										xProperty.append_attribute("value").set_value("1");
									}

									vector<Group::Member *> *objects = world->GetMembers();
									if(objects->empty() == false)
									{
										Group::Member **markObjects = &objects->front();
										Group::Member **endObjects = markObjects + objects->size();
										while(markObjects != endObjects)
										{
											game2d::PhysicalObject *object = (game2d::PhysicalObject *)*markObjects++;
											if(object->GetRenderLayer() == wObjectLayer)
											{
												unsigned int typeMask = object->GetTypeMask();
												Vector2<float> position = object->GetPosition();
												if(typeMask & game2d::Mask_Prop)
												{
													xml_node xObject = xLayer.append_child("object");
													xObject.append_attribute("type").set_value("Prop");
													xObject.append_attribute("x").set_value((short)position[0]);
													xObject.append_attribute("y").set_value(mapHeight - (short)position[1]);

													xml_node xProperties = xObject.append_child("properties");
													{
														xml_node xProperty = xProperties.append_child("property");
														xProperty.append_attribute("name").set_value("PropName");
														xProperty.append_attribute("value").set_value(((game2d::Prop *)object)->GetPropDef()->GetName().c_str());
													}
													float rotation = object->GetRotation();
													if(rotation != 0.f)
													{
														xml_node xProperty = xProperties.append_child("property");
														xProperty.append_attribute("name").set_value("Rotation");
														xProperty.append_attribute("value").set_value(rotation);
													}
												}
											}
										}
									}
								}
								break;
							case game2d::World::Layer_Wall:
								{
									game2d::World::WallLayer *wWallLayer = (game2d::World::WallLayer *)wlayer;
									Vector2<float> scale(32.f, 32.f);

									xml_node xLayer = xMap.append_child("objectgroup");
									xLayer.append_attribute("name").set_value(wlayer->GetName().c_str());
									xLayer.append_attribute("width").set_value(m_size[0]);
									xLayer.append_attribute("height").set_value(m_size[1]);

									{
										xml_node xProperties = xLayer.append_child("properties");
										xml_node xProperty = xProperties.append_child("property");
										xProperty.append_attribute("name").set_value("IsWallLayer");
										xProperty.append_attribute("value").set_value("1");
									}

									game2d::WallGrid *wallGrid = wWallLayer->GetWallGrid();
									unsigned short start = 0, length = 0;
									bool isPlacing = false;

									//- Fill -
									for(unsigned short b = 0; b <= wallGrid->m_height; b++)
										for(unsigned short a = 0; a <= wallGrid->m_width; a++)
										{
											if(wallGrid->m_data[a][b] & game2d::WallGrid::Fill && !(wallGrid->m_data[a][b] & game2d::WallGrid::Done))
											{
												unsigned short w = 0, h = 0, _w = 0, _h = 0;
												unsigned short limit = 0;
												bool valid = true, hasLimit = false;
												for(unsigned short _b = b; _b <= wallGrid->m_height; _b++)
												{
													_h = _b - b;
													for(unsigned short _a = a; _a <= wallGrid->m_width; _a++)
													{
														if(_b == b && _a == a)
															continue;
														_w = _a - a;
														if(hasLimit)
														{
															//- Don't go farther than the predetermined width -
															if(_w > w)
																break;
															//- Stop entirely if we can't go further -
															if(!(wallGrid->m_data[_a][_b] & game2d::WallGrid::Fill) || wallGrid->m_data[_a][_b] & game2d::WallGrid::Done)
															{
																valid = false;
																break;
															}
														}
														else
														{
															//- Extend as far right as possible -
															if(!(wallGrid->m_data[_a][_b] & game2d::WallGrid::Fill) || wallGrid->m_data[_a][_b] & game2d::WallGrid::Done)
															{
																w = _a - a - 1;
																hasLimit = true;
																break;
															}
															if(_a == wallGrid->m_width)
															{
																w = _a - a;
																hasLimit = true;
																break;
															}
														}
													}
//													if(!w)
//														break;
													if(!valid)
														break;
													if(_h > h)
														h = _h;
												}
												for(unsigned short _b = b; _b <= (b + h); _b++)
													for(unsigned short _a = a; _a <= (a + w); _a++)
														wallGrid->m_data[_a][_b] |= game2d::WallGrid::Done;
												w++;
												h++;

												xml_node xObject = xLayer.append_child("object");
												xObject.append_attribute("type").set_value("Wall");
												xObject.append_attribute("x").set_value((short)((float)a * scale[0]));
												xObject.append_attribute("y").set_value(mapHeight - (short)((float)(b + h) * scale[1]));

												xObject.append_attribute("width").set_value((short)((float)w * scale[0]));
												xObject.append_attribute("height").set_value((short)((float)h * scale[1]));
											}
										}
									for(unsigned short b = 0; b <= wallGrid->m_height; b++)
										for(unsigned short a = 0; a <= wallGrid->m_width; a++)
											if(wallGrid->m_data[a][b] & game2d::WallGrid::Done)
												wallGrid->m_data[a][b] &= ~game2d::WallGrid::Done;

									//- game2d::WallGrid::Vertical -
									for(unsigned short a = 0; a <= wallGrid->m_width; a++)
										for(unsigned short b = 0; b <= wallGrid->m_height; b++)
										{
											if(isPlacing)
											{
												if(!(wallGrid->m_data[a][b] & game2d::WallGrid::Vertical))
												{
													isPlacing = false;
													length = b - start;
													if(!length)
														continue;

													xml_node xObject = xLayer.append_child("object");
													xObject.append_attribute("type").set_value("Wall");
													xObject.append_attribute("x").set_value((short)((float)a * scale[0]));
													xObject.append_attribute("y").set_value(mapHeight - (short)((float)b * scale[1]));

													xml_node xPolyline = xObject.append_child("polyline");
													string points = formatString("0,0 0,%d", (short)((float)length * scale[1]));
													xPolyline.append_attribute("points").set_value(points.c_str());
												}
											}
											else
											{
												if(wallGrid->m_data[a][b] & game2d::WallGrid::Vertical)
												{
													start = b;
													isPlacing = true;
												}
											}
										}

									//- game2d::WallGrid::Horizontal -
									for(unsigned short b = 0; b <= wallGrid->m_height; b++)
										for(unsigned short a = 0; a <= wallGrid->m_width; a++)
										{
											if(isPlacing)
											{
												if(!(wallGrid->m_data[a][b] & game2d::WallGrid::Horizontal))
												{
													isPlacing = false;
													length = a - start;
													if(!length)
														continue;

													xml_node xObject = xLayer.append_child("object");
													xObject.append_attribute("type").set_value("Wall");
													xObject.append_attribute("x").set_value((short)((float)(a - length) * scale[0]));
													xObject.append_attribute("y").set_value(mapHeight - (short)((float)b * scale[1]));

													xml_node xPolyline = xObject.append_child("polyline");
													string points = formatString("0,0 %d,0", (short)((float)length * scale[0]));
													xPolyline.append_attribute("points").set_value(points.c_str());
												}
											}
											else
											{
												if(wallGrid->m_data[a][b] & game2d::WallGrid::Horizontal)
												{
													start = a;
													isPlacing = true;
												}
											}
										}

									//- Post -
									for(unsigned short b = 0; b <= wallGrid->m_height; b++)
										for(unsigned short a = 0; a <= wallGrid->m_width; a++)
											if(wallGrid->m_data[a][b] & game2d::WallGrid::Post)
											{
												xml_node xObject = xLayer.append_child("object");
												xObject.append_attribute("type").set_value("Wall");
												xObject.append_attribute("x").set_value((short)((float)a * scale[0]));
												xObject.append_attribute("y").set_value(mapHeight - (short)((float)b * scale[1]));
											}

								}
								break;
							case game2d::World::Layer_Tile:
								{
									game2d::World::TileLayer *wTileLayer = (game2d::World::TileLayer *)wlayer;
									Vector2<unsigned short> size = wTileLayer->GetSize();

									xml_node xLayer = xMap.append_child("layer");
									xLayer.append_attribute("name").set_value(wlayer->GetName().c_str());
									xLayer.append_attribute("width").set_value(m_size[0]);
									xLayer.append_attribute("height").set_value(m_size[1]);

									xml_node xData = xLayer.append_child("data");
									for(unsigned short b = size[1] - 1; b < size[1]; b--)
										for(unsigned short a = 0; a < size[0]; a++)
										{
											Vector3<unsigned char> tile = wTileLayer->GetTile(a, b);
											unsigned short gid;
											if(tile[0] == 255 || tile[1] == 255 || tile[2] == 255)
												gid = 0;
											else
											{
												TileSetDef *ts = m_tileSetMap[wTileLayer->GetTileSet(tile[0])];
												gid = coordToGid(Vector2<unsigned char>(tile[1], tile[2]), ts);
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
				}

				doc.save_file(file, " ", 1U, pugi::encoding_utf8);
			}
			void TMX::SaveObjects(void *groupLayer)
			{
			}
		}
	}
}
