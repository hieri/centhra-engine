//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/UI/Editor2D.h>
#include <CE/RenderPrimitives.h>
#include <CE/Game2D/AppGame2D.h>
#include <CE/Math.h>

using namespace std;

//TODO: Handle deletions on objects that are selected
//TODO: Auto sizing text buttons to wrap around the text
//TODO: Have active/inactive state changes for the editor, so we can handle cleanup/setup

namespace ce
{
	namespace ui
	{
		bool Editor_OnLayerSelect(ui::TextDropDownCtrl *ctrl, unsigned char id)
		{
			((Editor2DCtrl *)ctrl->GetParent())->SelectLayer(id);
			return false;
		}
		bool Editor_OnObjectModeSelect(ui::TextDropDownCtrl *ctrl, unsigned char id)
		{
			switch(id)
			{
			case 0:
				((Editor2DCtrl *)ctrl->GetParent())->SetMode(Editor2DCtrl::Mode_Object);
				break;
			case 1:
				((Editor2DCtrl *)ctrl->GetParent())->SetMode(Editor2DCtrl::Mode_Prop);
				break;
			}
			return false;
		}
		bool Editor_TileSetSelection(ui::TextDropDownCtrl *ctrl, unsigned char id)
		{
			((Editor2DCtrl *)ctrl->GetParent())->SelectTileSet(id);
			return false;
		}

		Editor2DCtrl::Editor2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, Skin *scrollSkin)
			: ui::Control(position, extent), m_isSelecting(false), m_isDragging(false), m_isRotating(false), m_tileMode(TileMode_None),
			m_mode(0), m_propPlaceID(-1), m_hover(0), m_font(font), m_currentLayer(0), m_currentTileSet(0), m_tilePlacementGroup(0),
			m_currentTile(255, 255)
		{
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseMotion | event::Mask_KeyDown | event::Mask_KeyUp;

			//- Menu Bar -
/*			m_modeObjectBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(32, 0), Vector2<int_canvas>(76, 22), m_font, "Object", Color(255, 255, 255), Color(63, 63, 63));
			m_modePropBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(124, 0), Vector2<int_canvas>(51, 22), m_font, "Prop", Color(255, 255, 255), Color(63, 63, 63));
			m_modeTileBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(191, 0), Vector2<int_canvas>(51, 22), m_font, "Tile", Color(255, 255, 255), Color(63, 63, 63));
			m_modeWallBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(258, 0), Vector2<int_canvas>(51, 22), m_font, "Wall", Color(255, 255, 255), Color(63, 63, 63));
*/
			//- Prop Selection -
			m_propSelectorCtrl = new PropSelectorCtrl(Vector2<int_canvas>(extent[0] - 246, 0), Vector2<int_canvas>(230, 120), scrollSkin);
			m_propSelectorCtrl->SetAnchor(ui::Control::Anchor_Right);
			m_propSelectorCtrl->GenerateButtons(m_font);
			Add(m_propSelectorCtrl);

			//- Tile Set & Tile Selection -
			m_tileSelectorCtrl = new TileSelectorCtrl(Vector2<int_canvas>(extent[0] - 246, 22), Vector2<int_canvas>(230, 120), scrollSkin);
			m_tileSelectorCtrl->SetAnchor(ui::Control::Anchor_Right);
			Add(m_tileSelectorCtrl);

			m_tileSetSelection = new ui::TextDropDownCtrl(Vector2<int_canvas>(extent[0] - 246, 0), Vector2<int_canvas>(230, 22), m_font, 200, "Select Tile Set");
			m_tileSetSelection->SetAnchor(ui::Control::Anchor_Right);
			m_tileSetSelection->SetOnSelection(Editor_TileSetSelection);
			Add(m_tileSetSelection);

			//- Mode Selection -
			m_objectModeSelection = new ui::TextDropDownCtrl(Vector2<int_canvas>(232, 0), Vector2<int_canvas>(100, 22), m_font, 100, "Mode");
			m_objectModeSelection->SetOnSelection(Editor_OnObjectModeSelect);
			m_objectModeSelection->AddSelection(0, "Object");
			m_objectModeSelection->AddSelection(1, "Prop");
			Add(m_objectModeSelection);

			//- Layer Selection -
			m_layerSelection = new ui::TextDropDownCtrl(Vector2<int_canvas>(16, 0), Vector2<int_canvas>(200, 22), m_font, 200, "Select Layer");
			m_layerSelection->SetOnSelection(Editor_OnLayerSelect);
			UpdateLayerSelection();
			Add(m_layerSelection);
			m_layerSelection->Select(0);

//			SetMode(Mode_None);
		}
		void Editor2DCtrl::OnAdded(Control *parent)
		{
			if(m_layerSelection)
				m_layerSelection->ReattachSelector();
		}
		bool Editor2DCtrl::OnEvent(Event &event)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			switch(event.type)
			{
			case event::MouseButtonDown:
				if(event.mouseButton.button == event::MouseButtonLeft)
				{
					if(m_isRotating)
						return true;
					if(m_isDragging)
						StopDragging();
					if(m_mode == Mode_Prop)
					{
						if(m_propPlaceID != -1)
						{
							game2d::PropDef *propDef = game2d::PropDef::GetPropDefByID(m_propPlaceID);
							if(propDef)
							{
								Vector2<float> pos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
								app->LockWorldMutex();
								game2d::Prop *prop = propDef->Spawn(pos);
								game2d::Camera::GetCurrent()->GetFocusGroup()->Add(prop);
								app->UnlockWorldMutex();
							}
						}
						else
						{
							if(m_hover)
							{
								StartDragging(event.mouseButton.x, event.mouseButton.y);
								if(m_isDragging)
									return true;
							}

							//TODO: Tidy this up
							m_dragStart = m_curMouse = Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y);
							m_isSelecting = true;
						}
					}
					else if(m_mode == Mode_Object)
					{
						if(m_hover)
						{
							StartDragging(event.mouseButton.x, event.mouseButton.y);
							if(m_isDragging)
								return true;
						}

						m_dragStart = m_curMouse = Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y);
						m_isSelecting = true;
					}
					else if(m_mode == Mode_Tile)
						if(m_currentLayer && m_tileMode == TileMode_None)
						{
							if(event.mouseButton.modifier == event::Mod_Shift)
							{
								if(m_tilePlacementGroup)
									break;

								Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
								game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
								Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
								Vector2<unsigned short> size = tileLayer->GetSize();
								float tileScale = tileLayer->GetScale();
								float tileWidth = tileScale * tileSize[0];
								float tileHeight = tileScale * tileSize[1];
								unsigned short x = (unsigned short)floor(curPos[0] / tileWidth);
								unsigned short y = (unsigned short)floor(curPos[1] / tileHeight);
								if(x < size[0] && y < size[1])
								{
									if(!tileLayer->HasTileSet(m_currentTileSet))
										tileLayer->AddTileSet(m_currentTileSet);
									tileLayer->FillTiles(x, y, Vector2<unsigned char>(m_currentTile[0], m_currentTile[1]), tileLayer->GetTileSetIndex(m_currentTileSet));
								}
							}
							else
							{
								if(m_currentTile[0] != 255 || m_currentTile[1] != 255)
								{
									Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
									game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
									Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
									Vector2<unsigned short> size = tileLayer->GetSize();
									float tileScale = tileLayer->GetScale();
									float tileWidth = tileScale * tileSize[0];
									float tileHeight = tileScale * tileSize[1];
									unsigned short x = (unsigned short)floor(curPos[0] / tileWidth);
									unsigned short y = (unsigned short)floor(curPos[1] / tileHeight);
									if(x < size[0] && y < size[1])
									{
										if(!tileLayer->HasTileSet(m_currentTileSet))
											tileLayer->AddTileSet(m_currentTileSet);
										tileLayer->SetTile(x, y, Vector2<unsigned char>(m_currentTile[0], m_currentTile[1]), tileLayer->GetTileSetIndex(m_currentTileSet));
									}
									m_tileMode = TileMode_Placing;
								}
								else if(m_tilePlacementGroup)
								{
									Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
									game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
									Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
									Vector2<unsigned short> size = tileLayer->GetSize();
									float tileScale = tileLayer->GetScale();
									float tileWidth = tileScale * tileSize[0];
									float tileHeight = tileScale * tileSize[1];
									unsigned short x = (unsigned short)floor(curPos[0] / tileWidth);
									unsigned short y = (unsigned short)floor(curPos[1] / tileHeight);

									short startX = (short)x - m_tilePlacementGroupSize[0] / 2;
									short startY = (short)y - m_tilePlacementGroupSize[1] / 2;
									short endX = startX + m_tilePlacementGroupSize[0] - 1;
									short endY = startY + m_tilePlacementGroupSize[1] - 1;

									//- Determine Real Start and End -
									if(startX > endX)
										swap(startX, endX);
									if(startY > endY)
										swap(startY, endY);

									short originX = startX;
									short originY = startY;

									//- Limit Selection -
									if(startX < 0)
										startX = 0;
									if(startX >= size[0])
										startX = size[0] - 1;
									if(endX < 0)
										endX = 0;
									if(endX >= size[0])
										endX = size[0] - 1;
									if(startY < 0)
										startY = 0;
									if(startY >= size[1])
										startY = size[1] - 1;
									if(endY < 0)
										endY = 0;
									if(endY >= size[1])
										endY = size[1] - 1;

									if((originX + m_tilePlacementGroupSize[0]) > 0 && (originY + m_tilePlacementGroupSize[1]) > 0 && originX < (short)size[0] && originY < (short)size[1])
									{
										//- Place Selection -
										for(short a = startX; a <= endX; a++)
											for(short b = startY; b <= endY; b++)
											{
												Vector3<unsigned char> &tile = m_tilePlacementGroup[a - originX][b - originY];
												if(tile[0] == 255 || tile[1] == 255 || tile[2] == 255)
													continue;
												tileLayer->SetTile(a, b, Vector2<unsigned char>(tile[1], tile[2]), tile[0]);
											}
									}
									m_tileMode = TileMode_Placing;
								}
								else
								{
									m_dragStart = m_curMouse = Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y);
									m_isSelecting = true;
								}
							}
						}
				}
				else if(event.mouseButton.button == event::MouseButtonRight)
				{
					if(m_mode == Mode_Object || m_mode == Mode_Prop)
					{
						if(m_selection.size())
						{
							if(m_isDragging)
								return true;
							StartRotating(event.mouseButton.x, event.mouseButton.y);
							if(m_isRotating)
								return true;
						}
					}
					else if(m_mode == Mode_Tile)
					{
						if(m_tileMode == TileMode_Placing && m_tilePlacementGroup)
						{
							ClearTilePlacementGroup();
							m_tileMode = TileMode_None;
						}
						if(m_tileMode == TileMode_None && m_currentLayer)
						{
							Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
							game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
							Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
							Vector2<unsigned short> size = tileLayer->GetSize();
							float tileScale = tileLayer->GetScale();
							float tileWidth = tileScale * tileSize[0];
							float tileHeight = tileScale * tileSize[1];
							unsigned short x = (unsigned short)floor(curPos[0] / tileWidth);
							unsigned short y = (unsigned short)floor(curPos[1] / tileHeight);
							if(event.mouseButton.modifier == event::Mod_Shift)
							{
								if(m_tilePlacementGroup)
									break;
								if(x < size[0] && y < size[1])
									tileLayer->FillTiles(x, y, Vector2<unsigned char>(255, 255), 255);
							}
							else if(m_tilePlacementGroup)
							{
								short startX = (short)x - m_tilePlacementGroupSize[0] / 2;
								short startY = (short)y - m_tilePlacementGroupSize[1] / 2;
								short endX = startX + m_tilePlacementGroupSize[0] - 1;
								short endY = startY + m_tilePlacementGroupSize[1] - 1;

								//- Determine Real Start and End -
								if(startX > endX)
									swap(startX, endX);
								if(startY > endY)
									swap(startY, endY);

								short originX = startX;
								short originY = startY;

								//- Limit Selection -
								if(startX < 0)
									startX = 0;
								if(startX >= size[0])
									startX = size[0] - 1;
								if(endX < 0)
									endX = 0;
								if(endX >= size[0])
									endX = size[0] - 1;
								if(startY < 0)
									startY = 0;
								if(startY >= size[1])
									startY = size[1] - 1;
								if(endY < 0)
									endY = 0;
								if(endY >= size[1])
									endY = size[1] - 1;

								if((originX + m_tilePlacementGroupSize[0]) > 0 && (originY + m_tilePlacementGroupSize[1]) > 0 && originX < (short)size[0] && originY < (short)size[1])
								{
									//- Delete Selection -
									for(short a = startX; a <= endX; a++)
										for(short b = startY; b <= endY; b++)
											tileLayer->SetTile(a, b, Vector2<unsigned char>(255, 255), 255);
								}
							}
							else if(x < size[0] && y < size[1])
								tileLayer->SetTile(x, y, Vector2<unsigned char>(255, 255), 255);
							m_tileMode = TileMode_Deleting;
						}
					}
				}
				else if(event.mouseButton.button == event::MouseButtonMiddle)
					if(m_mode == Mode_Tile)
						if(m_tileMode == TileMode_None)
						{
							//- Pick Tile -
							Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
							game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
							Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
							Vector2<unsigned short> size = tileLayer->GetSize();
							float tileScale = tileLayer->GetScale();
							float tileWidth = tileScale * tileSize[0];
							float tileHeight = tileScale * tileSize[1];
							unsigned short x = (unsigned short)floor(curPos[0] / tileWidth);
							unsigned short y = (unsigned short)floor(curPos[1] / tileHeight);

							ClearTilePlacementGroup();
							if(x < size[0] && y < size[1])
							{
								Vector3<unsigned char> tile = tileLayer->GetTile(x, y);
								if(tile[0] != 255 || tile[1] != 255 || tile[2] != 255)
								{
									game2d::TileSet *tileSet = tileLayer->GetTileSet(tile[0]);
									game2d::World *world = (game2d::World *)game2d::Camera::GetCurrent()->GetFocusGroup();
									vector<game2d::TileSet *> *tileSets = world->GetAssociatedTileSets();
									vector<game2d::TileSet *>::iterator it = find(tileSets->begin(), tileSets->end(), tileSet);
									m_tileSetSelection->Select(it - tileSets->begin());
									m_currentTile[0] = tile[1];
									m_currentTile[1] = tile[2];
								}
								else
									m_currentTile = Vector2<unsigned char>(255, 255);
							}
							else
								m_currentTile = Vector2<unsigned char>(255, 255);
						}
				break;
			case event::MouseButtonUp:
				if(m_mode == Mode_Object || m_mode == Mode_Prop)
				{
					if(event.mouseButton.button == event::MouseButtonLeft)
					{
						if(m_isSelecting)
						{
							float minX, minY, maxX, maxY;

							Vector2<float> startPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
							Vector2<float> endPos = app->GetWorldPositionFromCanvasPosition(m_dragStart[0], m_dragStart[1]);

							minX = min(startPos[0], endPos[0]);
							minY = min(startPos[1], endPos[1]);
							maxX = max(startPos[0], endPos[0]);
							maxY = max(startPos[1], endPos[1]);

							unsigned short ignoreMask = 0;// game2d::PhysicalObject::Mask_Wall;

							vector<game2d::PhysicalObject *> objects = game2d::Camera::GetCurrent()->GetFocusGroup()->BoxSearch(minX, minY, maxX, maxY, m_mode == Mode_Object ? (-1 & ~ignoreMask) : game2d::Mask_Prop);
							m_selection.clear();

							if((maxX - minX) < 0.1f && (maxY - minY) < 0.1f)
							{
								Vector2<float> pt = (startPos + endPos) / 2.f;
								if(objects.empty() == false)
								{
									game2d::PhysicalObject **markPhysicalObjects = &objects.front();
									game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + objects.size();
									while(markPhysicalObjects != endPhysicalObjects)
									{
										game2d::PhysicalObject *obj = *markPhysicalObjects++;
										if(obj->CollidesWith(pt))
										{
											m_selection.push_back(obj);
											break;
										}
									}
								}
							}
							else
							{
								m_selection.reserve(objects.size());
								if(objects.empty() == false)
								{
									game2d::PhysicalObject **markPhysicalObjects = &objects.front();
									game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + objects.size();
									while(markPhysicalObjects != endPhysicalObjects)
										m_selection.push_back(*markPhysicalObjects++);
								}
							}

							m_isSelecting = false;
						}
						else if(m_isDragging)
							StopDragging();
						if(m_selection.size())
						{
							Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
							CheckHover(curPos);
						}
					}
					else if(event.mouseButton.button == event::MouseButtonRight)
					{
						if(m_isRotating)
							StopRotating();
					}
				}
				else if(m_mode == Mode_Tile)
				{
					//- Select and Populate Placement Group -
					if(m_isSelecting)
					{
						ClearTilePlacementGroup();

						//- Get Selection Dimensions -
						Vector2<float> startPos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
						Vector2<float> endPos = app->GetWorldPositionFromCanvasPosition(m_dragStart[0], m_dragStart[1]);
						game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
						Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
						Vector2<unsigned short> size = tileLayer->GetSize();
						float tileScale = tileLayer->GetScale();
						float tileWidth = tileScale * tileSize[0];
						float tileHeight = tileScale * tileSize[1];
						short startX = (unsigned short)floor(startPos[0] / tileWidth);
						short startY = (unsigned short)floor(startPos[1] / tileHeight);
						short endX = (unsigned short)floor(endPos[0] / tileWidth);
						short endY = (unsigned short)floor(endPos[1] / tileHeight);

						//- Determine Real Start and End -
						if(startX > endX)
							swap(startX, endX);
						if(startY > endY)
							swap(startY, endY);

						//- Limit Selection -
						if(startX < 0)
							startX = 0;
						if(startX >= size[0])
							startX = size[0] - 1;
						if(endX < 0)
							endX = 0;
						if(endX >= size[0])
							endX = size[0] - 1;
						if(startY < 0)
							startY = 0;
						if(startY >= size[1])
							startY = size[1] - 1;
						if(endY < 0)
							endY = 0;
						if(endY >= size[1])
							endY = size[1] - 1;
						m_tilePlacementGroupSize = Vector2<unsigned short>((unsigned short)(endX - startX + 1), (unsigned short)(endY - startY + 1));

						if(m_tilePlacementGroupSize[0] && m_tilePlacementGroupSize[1])
						{
							//- Populate Selection -
							unsigned short validTiles = 0;
							m_tilePlacementGroup = new Vector3<unsigned char> *[m_tilePlacementGroupSize[0]];
							for(unsigned short a = 0; a < m_tilePlacementGroupSize[0]; a++)
							{
								m_tilePlacementGroup[a] = new Vector3<unsigned char>[m_tilePlacementGroupSize[1]];
								for(unsigned short b = 0; b < m_tilePlacementGroupSize[1]; b++)
								{
									m_tilePlacementGroup[a][b] = tileLayer->GetTile(startX + a, startY + b);
									if(m_tilePlacementGroup[a][b][1] != 255 && m_tilePlacementGroup[a][b][2] != 255)
										validTiles++;
								}
							}
							if(!validTiles)
								ClearTilePlacementGroup();
						}
						else
							m_tilePlacementGroupSize = Vector2<unsigned short>(0, 0);

						m_isSelecting = false;
					}

					if(event.mouseButton.button == event::MouseButtonLeft && m_tileMode == TileMode_Placing)
						m_tileMode = TileMode_None;
					else if(event.mouseButton.button == event::MouseButtonRight && m_tileMode == TileMode_Deleting)
						m_tileMode = TileMode_None;
				}
				break;
			case event::MouseMotion:
				if(m_isSelecting)
					m_curMouse = Vector2<int_canvas>(event.mouseMotion.x, event.mouseMotion.y);
				if(m_mode == Mode_Object || m_mode == Mode_Prop)
				{
					if(m_selection.size())
					{
						Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseMotion.x, event.mouseMotion.y);
						if(m_isDragging)
						{
							app->LockWorldMutex();
							if(m_dragSelection.empty() == false)
							{
								pair<game2d::PhysicalObject *, Vector2<float> > *markPhysicalObjects = &m_dragSelection.front();
								pair<game2d::PhysicalObject *, Vector2<float> > *endPhysicalObjects = markPhysicalObjects + m_dragSelection.size();
								while(markPhysicalObjects != endPhysicalObjects)
								{
									markPhysicalObjects->first->SetPosition(curPos + markPhysicalObjects->second);
									markPhysicalObjects++;
								}
							}
							app->UnlockWorldMutex();
						}
						else if(m_isRotating)
						{
							Vector2<float> dif = curPos - m_rotateCenter;
							float angleOffset = BaseAngleRadians(atan2f(dif[1], dif[0]) - m_startRotation);

							app->LockWorldMutex();
							if(m_rotateSelection.empty() == false)
							{
								RotateDef *markPhysicalObjects = &m_rotateSelection.front();
								RotateDef *endPhysicalObjects = markPhysicalObjects + m_rotateSelection.size();
								while(markPhysicalObjects != endPhysicalObjects)
								{
									markPhysicalObjects->object->SetRotation(RadiansToDegrees() * BaseAngleRadians(angleOffset + markPhysicalObjects->angle));
									float offsetAngle = BaseAngleRadians(angleOffset + markPhysicalObjects->offsetAngle);
									markPhysicalObjects->object->SetPosition(Vector2<float>(cos(offsetAngle), sin(offsetAngle)) * markPhysicalObjects->offsetLength + m_rotateCenter);
									markPhysicalObjects++;
								}
							}
							app->UnlockWorldMutex();
						}
						else
							CheckHover(curPos);
					}
				}
				else if(m_mode == Mode_Tile)
					if(m_currentLayer)
					{ 
						Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseMotion.x, event.mouseMotion.y);
						game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
						Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
						Vector2<unsigned short> size = tileLayer->GetSize();
						float tileScale = tileLayer->GetScale();
						float tileWidth = tileScale * tileSize[0];
						float tileHeight = tileScale * tileSize[1];
						m_tileHover[0] = floor(curPos[0] / tileWidth);
						m_tileHover[1] = floor(curPos[1] / tileHeight);
						unsigned short x = (unsigned short)m_tileHover[0];
						unsigned short y = (unsigned short)m_tileHover[1];
						m_tileHover[0] *= tileWidth;
						m_tileHover[1] *= tileHeight;

						if(m_tileMode == TileMode_Placing)
						{
							if(m_tilePlacementGroup)
							{
								short startX = (short)x - m_tilePlacementGroupSize[0] / 2;
								short startY = (short)y - m_tilePlacementGroupSize[1] / 2;
								short endX = startX + m_tilePlacementGroupSize[0] - 1;
								short endY = startY + m_tilePlacementGroupSize[1] - 1;

								//- Determine Real Start and End -
								if(startX > endX)
									swap(startX, endX);
								if(startY > endY)
									swap(startY, endY);

								short originX = startX;
								short originY = startY;

								//- Limit Selection -
								if(startX < 0)
									startX = 0;
								if(startX >= size[0])
									startX = size[0] - 1;
								if(endX < 0)
									endX = 0;
								if(endX >= size[0])
									endX = size[0] - 1;
								if(startY < 0)
									startY = 0;
								if(startY >= size[1])
									startY = size[1] - 1;
								if(endY < 0)
									endY = 0;
								if(endY >= size[1])
									endY = size[1] - 1;

								if((originX + m_tilePlacementGroupSize[0]) > 0 && (originY + m_tilePlacementGroupSize[1]) > 0 && originX < (short)size[0] && originY < (short)size[1])
								{
									//- Place Selection -
									for(short a = startX; a <= endX; a++)
										for(short b = startY; b <= endY; b++)
										{
											Vector3<unsigned char> &tile = m_tilePlacementGroup[a - originX][b - originY];
											if(tile[0] == 255 || tile[1] == 255 || tile[2] == 255)
												continue;
											tileLayer->SetTile(a, b, Vector2<unsigned char>(tile[1], tile[2]), tile[0]);
										}
								}
							}
							else if(x < size[0] && y < size[1])
							{
								if(!tileLayer->HasTileSet(m_currentTileSet))
									tileLayer->AddTileSet(m_currentTileSet);
								tileLayer->SetTile(x, y, Vector2<unsigned char>(m_currentTile[0], m_currentTile[1]), tileLayer->GetTileSetIndex(m_currentTileSet));
							}
						}
						else if(m_tileMode == TileMode_Deleting)
						{
							if(m_tilePlacementGroup)
							{
								short startX = (short)x - m_tilePlacementGroupSize[0] / 2;
								short startY = (short)y - m_tilePlacementGroupSize[1] / 2;
								short endX = startX + m_tilePlacementGroupSize[0] - 1;
								short endY = startY + m_tilePlacementGroupSize[1] - 1;

								//- Determine Real Start and End -
								if(startX > endX)
									swap(startX, endX);
								if(startY > endY)
									swap(startY, endY);

								short originX = startX;
								short originY = startY;

								//- Limit Selection -
								if(startX < 0)
									startX = 0;
								if(startX >= size[0])
									startX = size[0] - 1;
								if(endX < 0)
									endX = 0;
								if(endX >= size[0])
									endX = size[0] - 1;
								if(startY < 0)
									startY = 0;
								if(startY >= size[1])
									startY = size[1] - 1;
								if(endY < 0)
									endY = 0;
								if(endY >= size[1])
									endY = size[1] - 1;

								if((originX + m_tilePlacementGroupSize[0]) > 0 && (originY + m_tilePlacementGroupSize[1]) > 0 && originX < (short)size[0] && originY < (short)size[1])
								{
									//- Delete Selection -
									for(short a = startX; a <= endX; a++)
										for(short b = startY; b <= endY; b++)
											tileLayer->SetTile(a, b, Vector2<unsigned char>(255, 255), 255);
								}
							}
							else if(x < size[0] && y < size[1])
								tileLayer->SetTile(x, y, Vector2<unsigned char>(255, 255), 255);
						}
					}
				break;
			case event::KeyDown:
				break;
			case event::KeyUp:
				if(event.key.keyCode == Key_Backspace || event.key.keyCode == Key_Delete)
				{
					if(m_selection.size())
					{
						app->LockWorldMutex();
						if(m_selection.empty() == false)
						{
							game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
							game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
							while(markPhysicalObjects != endPhysicalObjects)
								(*markPhysicalObjects++)->Delete();
						}
						app->UnlockWorldMutex();
						m_selection.clear();
					}
				}
				else if(event.key.keyCode == Key_C)
				{
					app->LockWorldMutex();
/*					if(app->m_player->GetCollisionMask())
						app->m_player->SetCollisionMask(0);
					else
						app->m_player->ResetCollisionMask();*/
					app->UnlockWorldMutex();
				}
				else if(event.key.keyCode == Key_Backslash || event.key.keyCode == Key_VerticalBar)
				{
					app->LockWorldMutex();
	//				app->m_currentZone->SaveToFile("assets/zones/output.tmx");
//					app->m_currentZone->Save();
					app->UnlockWorldMutex();
				}
				break;
			}

			return Control::OnEvent(event);
		}
		void Editor2DCtrl::DoRender()
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			game2d::Camera *camera = game2d::Camera::GetCurrent();
			ui::CameraView2DCtrl *currentViewport = app->GetCurrentViewport();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPushMatrix();
				if(m_isSelecting)
				{
					glTranslatef((float)m_dragStart[0], (float)(m_dragStart[1]), 0.f);
					glScalef((float)(m_curMouse[0] - m_dragStart[0]), -(float)(m_dragStart[1] - m_curMouse[1]), 1.f);
					glColor4ub(0, 0, 255, 200);
					RenderSquare();
				}
				if(m_mode == Mode_Object || m_mode == Mode_Prop)
				{
					if(m_selection.size())
					{
						glTranslatef(0.f, (float)m_extent[1], 0.f);
						glScalef(1.f, -1.f, 1.f);
						Vector2<float> focalPoint = camera->GetFocalPoint();
						Vector2<int_canvas> viewExtent = currentViewport->GetExtent();
						Vector2<float> half((float)viewExtent[0] / 2.f, (float)viewExtent[1] / 2.f);
						Vector2<float> viewScale = currentViewport->GetViewScale();
						glTranslatef(half[0] - viewScale[0] * focalPoint[0], half[1] - viewScale[1] * focalPoint[1], 0.f);
						glScalef(viewScale[0], viewScale[1], 1.f);

						glColor4ub(0, 0, 255, 200);
						if(m_selection.empty() == false)
						{
							game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
							game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
							while(markPhysicalObjects != endPhysicalObjects)
							{
								game2d::PhysicalObject *object = *markPhysicalObjects++;
								glPushMatrix();
									Vector2<float> position = object->GetPosition();
									Vector2<float> extent = object->GetExtent();

									glTranslatef(position[0], position[1], 0.f);
									glRotatef(object->GetRotation(), 0.f, 0.f, 1.f);
									glScalef(extent[0], extent[1], 1.f);
									glTranslatef(-0.5f, -0.5f, 0.f);
									RenderSquare();
								glPopMatrix();
							}
						}
						glColor4ub(255, 255, 255, 255);
					}
				}
				else if(m_mode == Mode_Tile)
				{
					if(m_currentLayer && ((m_currentTile[0] != 255 || m_currentTile[1] != 255) || m_tilePlacementGroup))
					{
						glTranslatef(0.f, (float)m_extent[1], 0.f);
						glScalef(1.f, -1.f, 1.f);
						Vector2<float> focalPoint = camera->GetFocalPoint();
						Vector2<int_canvas> viewExtent = currentViewport->GetExtent();
						Vector2<float> half((float)viewExtent[0] / 2.f, (float)viewExtent[1] / 2.f);
						Vector2<float> viewScale = currentViewport->GetViewScale();
						glTranslatef(half[0] - viewScale[0] * focalPoint[0], half[1] - viewScale[1] * focalPoint[1], 0.f);
						glScalef(viewScale[0], viewScale[1], 1.f);

						game2d::World::TileLayer *tileLayer = (game2d::World::TileLayer *)m_currentLayer;
						Vector2<unsigned short> tileSize = tileLayer->GetTileSize();
						float tileScale = tileLayer->GetScale();

						//TODO: Cache the tile size
						if(m_currentTile[0] != 255 || m_currentTile[1] != 255)
						{
							glPushMatrix();
								glColor4ub(127, 255, 127, 200);
								glTranslatef(m_tileHover[0], m_tileHover[1], 0.f);
								glScalef((float)tileSize[0] * tileScale, (float)tileSize[1] * tileScale, 1.f);
								m_currentTileSet->Render(m_currentTile[0], m_currentTile[1]);
								glColor4ub(255, 255, 255, 255);
							glPopMatrix();
						}
						if(m_tilePlacementGroup)
						{
							glPushMatrix();
								glColor4ub(127, 255, 127, 200);
								glTranslatef(m_tileHover[0], m_tileHover[1], 0.f);
								glScalef((float)tileSize[0] * tileScale, (float)tileSize[1] * tileScale, 1.f);

								glTranslatef(-(float)(m_tilePlacementGroupSize[0] / 2), -(float)(m_tilePlacementGroupSize[1] / 2), 0.f);
								for(unsigned short a = 0; a < m_tilePlacementGroupSize[0]; a++)
								{
									glPushMatrix();
									for(unsigned short b = 0; b < m_tilePlacementGroupSize[1]; b++)
									{
										Vector3<unsigned char> &tile = m_tilePlacementGroup[a][b];
										if(tile[1] != 255 && tile[2] != 255)
											tileLayer->GetTileSet(tile[0])->Render(tile[1], tile[2]);
										glTranslatef(0.f, 1.f, 0.f);
									}
									glPopMatrix();
									glTranslatef(1.f, 0.f, 0.f);
								}
								glColor4ub(255, 255, 255, 255);
							glPopMatrix();
						}
					}
				}

			glPopMatrix();

			glDisable(GL_BLEND);
		}
		void Editor2DCtrl::StartDragging(int_canvas x, int_canvas y)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(x, y);
			m_dragSelection.reserve(m_selection.size());
			app->LockWorldMutex();
			if(m_selection.empty() == false)
			{
				game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
				game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
				while(markPhysicalObjects != endPhysicalObjects)
				{
					game2d::PhysicalObject *object = *markPhysicalObjects++;
					object->SetCollidable(false);
					object->SetVelocity(Vector2<float>(0.f, 0.f));
					object->SetAngularVelocity(0.f);
					m_dragSelection.push_back(pair<game2d::PhysicalObject *, Vector2<float> >(object, object->GetPosition() - curPos));
				}
			}
			app->UnlockWorldMutex();
			m_hover = 0;
			m_isDragging = true;
		}
		void Editor2DCtrl::StopDragging()
		{
			if(m_selection.empty() == false)
			{
				game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
				game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
				while(markPhysicalObjects != endPhysicalObjects)
					(*markPhysicalObjects++)->SetCollidable(true);
			}
			m_dragSelection.clear();
			m_isDragging = false;
		}
		void Editor2DCtrl::StartRotating(int_canvas x, int_canvas y)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			m_rotateSelection.reserve(m_selection.size());
			Vector2<float> totalPos;
			Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(x, y);

			int N = 0;
			unsigned short ignoreMask = 0;// game2d::PhysicalObject::Mask_Wall | game2d::PhysicalObject::Mask_Body;
			app->LockWorldMutex();
			if(m_selection.empty() == false)
			{
				game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
				game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
				while(markPhysicalObjects != endPhysicalObjects)
				{
					game2d::PhysicalObject *object = *markPhysicalObjects++;
					if(object->GetTypeMask() & ignoreMask)
						continue;
					totalPos += object->GetPosition();
					N++;
				}

				m_rotateCenter = totalPos / (float)N;

				Vector2<float> dif = curPos - m_rotateCenter;
				m_startRotation = atan2f(dif[1], dif[0]);
				Vector2<float> offset;

				markPhysicalObjects = &m_selection.front();
				while(markPhysicalObjects != endPhysicalObjects)
				{
					game2d::PhysicalObject *object = *markPhysicalObjects++;
					if(object->GetTypeMask() & ignoreMask)
						continue;
					object->SetCollidable(false);
					object->SetVelocity(Vector2<float>(0.f, 0.f));
					object->SetAngularVelocity(0.f);
					RotateDef rotateDef;
					rotateDef.object = object;
					offset = object->GetPosition() - m_rotateCenter;
					rotateDef.angle = object->GetRotation() * DegreesToRadians();
					rotateDef.offsetLength = offset.GetLength();
					rotateDef.offsetAngle = atan2f(offset[1], offset[0]);
					m_rotateSelection.push_back(rotateDef);
				}
			}
			app->UnlockWorldMutex();

			m_hover = 0;
			m_isRotating = true;
		}
		void Editor2DCtrl::StopRotating()
		{
			if(m_selection.empty() == false)
			{
				game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
				game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
				while(markPhysicalObjects != endPhysicalObjects)
					(*markPhysicalObjects++)->SetCollidable(true);
			}
			m_rotateSelection.clear();
			m_isRotating = false;
		}
		void Editor2DCtrl::CheckHover(Vector2<float> position)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			if(m_selection.empty() == false)
			{
				game2d::PhysicalObject **markPhysicalObjects = &m_selection.front();
				game2d::PhysicalObject **endPhysicalObjects = markPhysicalObjects + m_selection.size();
				while(markPhysicalObjects != endPhysicalObjects)
				{
					game2d::PhysicalObject *object = *markPhysicalObjects++;
					if(object->CollidesWith(position))
					{
						m_hover = object;
						return;
					}
				}
			}
			m_hover = 0;
		}

		//- Prop Selector -
		Color g_propSelectDefault(0, 0, 0, 127), g_propSelectHighlight(0, 255, 0, 63);
		Editor2DCtrl::PropSelectorCtrl::PropSelectorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Skin *skin) : ScrollCtrl(position, extent, skin), m_lastSelection(0)
		{
		}
		void Editor2DCtrl::PropSelectorCtrl::OnSelect(PropSelectCtrl *btn)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)GetParent();
			if(editor->m_propPlaceID == btn->m_propID)
			{
				editor->m_propPlaceID = -1;
				m_lastSelection = 0;
				btn->SetColor(g_propSelectDefault);
			}
			else
			{
				if(m_lastSelection)
					m_lastSelection->SetColor(g_propSelectDefault);
				btn->SetColor(g_propSelectHighlight);
				editor->m_propPlaceID = btn->m_propID;
				m_lastSelection = btn;
			}
		}
		void Editor2DCtrl::PropSelectorCtrl::GenerateButtons(Font *font)
		{
			map<unsigned short, game2d::PropDef *> *propDefTable = game2d::PropDef::GetPropDefTable();
			int_canvas padding = 8, buttonWidth = 214, buttonHeight = 48;
			int_canvas startX = padding;
			int_canvas startY = padding;
			for(map<unsigned short, game2d::PropDef *>::iterator it = propDefTable->begin(); it != propDefTable->end(); it++)
			{
				Vector2<float> extent = it->second->GetExtent();
				PropSelectCtrl *btn = new PropSelectCtrl(Vector2<int_canvas>(startX, startY), Vector2<int_canvas>(buttonWidth, buttonHeight), it->second->GetID(), font);
				Add((ui::ButtonCtrl *)btn);

				startY += buttonHeight + padding;
			}
			UpdateDimensions();
		}

		bool Editor_PropSelectBtnDown(ui::ButtonCtrl *button)
		{
			Editor2DCtrl::PropSelectorCtrl *selector = (Editor2DCtrl::PropSelectorCtrl *)button->GetParent();
			selector->OnSelect((Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl *)button);
			return false;
		}
		Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl::PropSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, unsigned short propID, Font *font) : ButtonCtrl(position, extent), ColorCtrl(position, extent, g_propSelectDefault), m_propID(propID)
		{
			m_propDef = game2d::PropDef::GetPropDefByID(propID);
			SetOnButtonDown(Editor_PropSelectBtnDown);

			ui::TextCtrl *propLabel = new ui::TextCtrl(Vector2<int_canvas>(52, 14), Vector2<int_canvas>(256, 20), font, m_propDef->GetName().c_str());
			ButtonCtrl::Add(propLabel);
		}
		void Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl::DoRender()
		{
			ColorCtrl::DoRender();

			glPushMatrix();
				glScalef((float)ButtonCtrl::m_extent[1], (float)ButtonCtrl::m_extent[1], 0.f);
				m_propDef->UIRender();
			glPopMatrix();
		}

		//- Tile Set & Tile Selector -
		Color g_tileSelectDefault(255, 255, 255, 255), g_tileSelectHighlight(127, 255, 127, 255);
		void Editor2DCtrl::SelectTileSet(unsigned char tileSetId)
		{
			ClearTilePlacementGroup();

			if(m_tileMode == TileMode_Placing && m_tilePlacementGroup)
			{
				ClearTilePlacementGroup();
				m_tileMode = TileMode_None;
			}
			game2d::World *world = (game2d::World *)game2d::Camera::GetCurrent()->GetFocusGroup();
			vector<game2d::TileSet *> *tileSets = world->GetAssociatedTileSets();
			//TODO: Handle out of bounds
			game2d::TileSet *target = tileSets->operator[](tileSetId);

			if(m_currentTileSet != target)
			{
				m_currentTileSet = target;
				m_currentTile = Vector2<unsigned char>(255, 255);
				m_tileSelectorCtrl->GenerateButtons(m_font, target);
			}
		}
		Editor2DCtrl::TileSelectorCtrl::TileSelectorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Skin *skin) : ScrollCtrl(position, extent, skin), m_lastSelection(0)
		{
		}
		void Editor2DCtrl::TileSelectorCtrl::OnSelect(TileSelectCtrl *btn)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)GetParent();
			editor->ClearTilePlacementGroup();
			Vector2<unsigned char> tile(btn->m_tileX, btn->m_tileY);
			if(editor->m_currentTile == tile)
			{
				editor->m_currentTile = Vector2<unsigned char>(255, 255);
				m_lastSelection = 0;
				btn->SetColor(g_tileSelectDefault);
			}
			else
			{
				if(m_lastSelection)
					m_lastSelection->SetColor(g_tileSelectDefault);
				btn->SetColor(g_tileSelectHighlight);
				editor->m_currentTile = tile;
				m_lastSelection = btn;
			}
		}
		void Editor2DCtrl::TileSelectorCtrl::GenerateButtons(Font *font, game2d::TileSet *tileSet)
		{
			DeleteChildren();

			Vector2<unsigned short> tileSize = tileSet->GetTileSize();
			int_canvas padding = 4, buttonWidth = tileSize[0], buttonHeight = tileSize[1];

			Vector2<unsigned char> size = tileSet->GetSize(), currentTile = ((Editor2DCtrl *)m_parent)->m_currentTile;
				for(unsigned char y = 0; y < size[1]; y++)
			{
			for(unsigned char x = 0; x < size[0]; x++)
				{
					TileSelectCtrl *btn = new TileSelectCtrl(Vector2<int_canvas>(x * buttonWidth + (x + 1) * padding, y * buttonHeight + (y + 1) * padding), Vector2<int_canvas>(buttonWidth, buttonHeight), x, y, font);
					btn->SetColor((x == currentTile[0] && y == currentTile[1]) ? g_tileSelectHighlight : g_tileSelectDefault);
					Add((ui::ButtonCtrl *)btn);
					((ButtonCtrl *)btn)->UpdateAbsoluteMatrix();
				}
			}
		}

		bool Editor_TileSelectBtnDown(ui::ButtonCtrl *button)
		{
			Editor2DCtrl::TileSelectorCtrl *selector = (Editor2DCtrl::TileSelectorCtrl *)button->GetParent();
			selector->OnSelect((Editor2DCtrl::TileSelectorCtrl::TileSelectCtrl *)button);
			return false;
		}
		bool Editor_TileSelectBtnUp(ui::ButtonCtrl *button)
		{
			return false;
		}
		Editor2DCtrl::TileSelectorCtrl::TileSelectCtrl::TileSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, unsigned char tileX, unsigned char tileY, Font *font)
			: ButtonCtrl(position, extent), ColorCtrl(position, extent, g_tileSelectDefault),
			m_tileX(tileX), m_tileY(tileY)
		{
			SetOnButtonDown(Editor_TileSelectBtnDown);
			SetOnButtonUp(Editor_TileSelectBtnUp);
		}
		void Editor2DCtrl::TileSelectorCtrl::TileSelectCtrl::DoRender()
		{
			glPushMatrix();
				glScalef((float)ButtonCtrl::m_extent[1], (float)ButtonCtrl::m_extent[1], 0.f);
				glColor4ubv(&m_color[0]);
				((Editor2DCtrl *)ButtonCtrl::GetParent()->GetParent())->m_currentTileSet->UIRender(m_tileX, m_tileY);
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}

		//------------------- Layer Selection --------------
		void Editor2DCtrl::UpdateLayerSelection()
		{
			if(!m_layerSelection)
				return;
			game2d::World *world = (game2d::World *)game2d::Camera::GetCurrent()->GetFocusGroup();
			vector<game2d::World::Layer *> *layers = world->GetLayers();
			m_layerSelection->ClearSelections();
			unsigned char idx = 0;
			if(layers->empty() == false)
			{
				game2d::World::Layer **markLayers = &layers->front();
				game2d::World::Layer **endLayers = markLayers + layers->size();
				while(markLayers != endLayers)
					m_layerSelection->AddSelection(idx++, (*markLayers++)->GetName());
			}
		}
		void Editor2DCtrl::SelectLayer(unsigned char layerId)
		{
			game2d::World *world = (game2d::World *)game2d::Camera::GetCurrent()->GetFocusGroup();
			game2d::World::Layer *layer = world->GetLayer(layerId);

			unsigned char layerType = layer->GetType();
			switch(layerType)
			{
			case game2d::World::Layer_Object:
				m_objectModeSelection->Select(0);
				SetMode(Mode_Object);
				break;
			case game2d::World::Layer_Tile:
				SetMode(Mode_Tile);
				{
					vector<game2d::TileSet *> *tileSets = world->GetAssociatedTileSets();
					unsigned char idx = 0;
					m_tileSetSelection->ClearSelections();
					//TODO: Use the actual id of the tileset instead of the index
					if(tileSets->empty() == false)
					{
						game2d::TileSet **markTileSets = &tileSets->front();
						game2d::TileSet **endTileSets = markTileSets + tileSets->size();
						while(markTileSets != endTileSets)
							m_tileSetSelection->AddSelection(idx++, (*markTileSets++)->GetName());
					}
				}
				break;
			}

			m_currentLayer = layer;
		}
		void Editor2DCtrl::ClearTilePlacementGroup()
		{
			for(unsigned short a = 0; a < m_tilePlacementGroupSize[0]; a++)
				delete [] m_tilePlacementGroup[a];
			delete [] m_tilePlacementGroup;
			m_tilePlacementGroup = 0;
			m_tilePlacementGroupSize = Vector2<unsigned short>(0, 0);
		}

		//------------------- Mode Selection --------------
		void Editor2DCtrl::SetMode(unsigned char mode)
		{
			if(m_mode == mode)
				return;

			m_mode = mode;
			ClearTilePlacementGroup();
			m_tileMode = TileMode_None;

			if(m_mode == Mode_Object || m_mode == Mode_Prop)
				m_selection.clear();

			if(m_mode == Mode_Prop)
				m_propPlaceID = -1;

			m_objectModeSelection->SetVisible(mode == Mode_Object || mode == Mode_Prop);
			m_propSelectorCtrl->SetVisible(m_mode == Mode_Prop);
			m_tileSelectorCtrl->SetVisible(m_mode == Mode_Tile);
			m_tileSetSelection->SetVisible(m_mode == Mode_Tile);
		}
	}
}
