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
//TODO: ScrollCtrl
//TODO: Global MouseUp event

namespace ce
{
	namespace ui
	{
		//TODO: Modify Control class to intercept events
		ui::TextCtrl *test = 0;
		bool Editor_BtnDown(ui::ButtonCtrl *button)
		{
			return false;
		}
		bool Editor_ObjectBtnUp(ui::ButtonCtrl *button)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)button->GetParent();
			editor->SetMode(Editor2DCtrl::Mode_Object);
			return false;
		}
		bool Editor_PropBtnUp(ui::ButtonCtrl *button)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)button->GetParent();
			editor->SetMode(Editor2DCtrl::Mode_Prop);
			return false;
		}
		bool Editor_TileBtnUp(ui::ButtonCtrl *button)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)button->GetParent();
			editor->SetMode(Editor2DCtrl::Mode_Tile);
			return false;
		}
		bool Editor_WallBtnUp(ui::ButtonCtrl *button)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)button->GetParent();
			editor->SetMode(Editor2DCtrl::Mode_Wall);
			return false;
		}

		Editor2DCtrl::Editor2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font) : ui::Control(position, extent), m_isSelecting(false), m_isDragging(false), m_isRotating(false), m_mode(0), m_propPlaceType(-1), m_hover(0), m_font(font)
		{
			m_modeObjectBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(32, 0), Vector2<int_canvas>(76, 22), m_font, "Object", Color(255, 255, 255), Color(63, 63, 63));
			m_modePropBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(124, 0), Vector2<int_canvas>(51, 22), m_font, "Prop", Color(255, 255, 255), Color(63, 63, 63));
			m_modeTileBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(191, 0), Vector2<int_canvas>(51, 22), m_font, "Tile", Color(255, 255, 255), Color(63, 63, 63));
			m_modeWallBtn = new ui::TextButtonCtrl(Vector2<int_canvas>(258, 0), Vector2<int_canvas>(51, 22), m_font, "Wall", Color(255, 255, 255), Color(63, 63, 63));
			m_modeObjectBtn->SetOnButtonDown(Editor_BtnDown);
			m_modeObjectBtn->SetOnButtonUp(Editor_ObjectBtnUp);
			m_modePropBtn->SetOnButtonDown(Editor_BtnDown);
			m_modePropBtn->SetOnButtonUp(Editor_PropBtnUp);
			m_modeTileBtn->SetOnButtonDown(Editor_BtnDown);
			m_modeTileBtn->SetOnButtonUp(Editor_TileBtnUp);
			m_modeWallBtn->SetOnButtonDown(Editor_BtnDown);
			m_modeWallBtn->SetOnButtonUp(Editor_WallBtnUp);

/*			m_propSelectorCtrl = new PropSelectorCtrl(Vector2<int_canvas>(extent[0] - 160 - 320, 0), Vector2<int_canvas>(128 + 320, 128));
			m_propSelectorCtrl->GenerateButtons();
			m_propSelectorCtrl->SetAnchor(ui::Control::Anchor_Right);*/

			Add((ui::ButtonCtrl *)m_modeObjectBtn);
			Add((ui::ButtonCtrl *)m_modePropBtn);
			Add((ui::ButtonCtrl *)m_modeTileBtn);
			Add((ui::ButtonCtrl *)m_modeWallBtn);
//			Add(m_propSelectorCtrl);
			SetMode(Mode_Object);
		}
		void Editor2DCtrl::SetMode(unsigned char mode)
		{
			if((m_mode == Mode_Object || m_mode == Mode_Prop) && m_mode != mode)
				m_selection.clear();

			if(m_mode == Mode_Prop && m_mode != mode)
				m_propPlaceType = -1;

			m_mode = mode;

//			m_propSelectorCtrl->SetVisible(m_mode == Mode_Prop);

			m_modeObjectBtn->SetBackgroundColor(mode == Mode_Object ? Color(63, 127, 63) : Color(63, 63, 63));
			m_modePropBtn->SetBackgroundColor(mode == Mode_Prop ? Color(63, 127, 63) : Color(63, 63, 63));
			m_modeTileBtn->SetBackgroundColor(mode == Mode_Tile ? Color(63, 127, 63) : Color(63, 63, 63));
			m_modeWallBtn->SetBackgroundColor(mode == Mode_Wall ? Color(63, 127, 63) : Color(63, 63, 63));
		}
		bool Editor2DCtrl::OnEvent(Event &event)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();

			switch(event.type)
			{
			case event::MouseButtonDown:
				{
					ui::Control *target = GetFromPosition(Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y), true);
					if(target != this)
						if(!target->OnEvent(event))
						{
							if(m_selection.size())
								m_selection.clear();
							return false;
						}
				}
				if(event.mouseButton.button == event::MouseButtonLeft)
				{
					if(m_isRotating)
						return true;
					if(m_isDragging)
						StopDragging();
					if(m_mode == Mode_Prop)
					{
						if(m_propPlaceType != -1)
						{
							/*PropDef *propDef = PropDef::GetByType(m_propPlaceType);
							if(propDef)
							{
								Vector2<float> pos = app->GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
								app->LockWorldMutex();
								Prop *prop = new Prop(pos, m_propPlaceType);
								app->m_currentZone->Add(prop);
								app->UnlockWorldMutex();
							}*/
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
				}
				else if(event.mouseButton.button == event::MouseButtonRight)
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

							game2d::PhysicalGroup *currentGroup = (game2d::PhysicalGroup *)app->GetReferenceObject()->GetParentGroup();
							vector<game2d::PhysicalObject *> objects = currentGroup->BoxSearch(minX, minY, maxX, maxY, m_mode == Mode_Object ? (-1 & ~ignoreMask) : 0);// game2d::PhysicalObject::Mask_Prop);
							m_selection.clear();

							if((maxX - minX) < 0.1f && (maxY - minY) < 0.1f)
							{
								Vector2<float> pt = (startPos + endPos) / 2.f;

								if(objects.size())
									for(vector<game2d::PhysicalObject *>::iterator it = objects.begin(); it != objects.end(); it++)
									{
										game2d::PhysicalObject *obj = *it;
										if(obj->CollidesWith(pt))
										{
											m_selection.push_back((game2d::PhysicalObject *)obj);
											break;
										}
									}
							}
							else
							{
								m_selection.reserve(objects.size());
								for(vector<game2d::PhysicalObject *>::iterator it = objects.begin(); it != objects.end(); it++)
									m_selection.push_back((game2d::PhysicalObject *)*it);
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
				{
					ui::Control *target = GetFromPosition(Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y), true);
					if(target != this)
						if(!target->OnEvent(event))
							return false;
				}
				break;
			case event::MouseMotion:
				if(m_isSelecting)
					m_curMouse = Vector2<int_canvas>(event.mouseMotion.x, event.mouseMotion.y);
				if(m_selection.size())
				{
					Vector2<float> curPos = app->GetWorldPositionFromCanvasPosition(event.mouseMotion.x, event.mouseMotion.y);
					if(m_isDragging)
					{
						app->LockWorldMutex();
						for(vector<pair<game2d::PhysicalObject *, Vector2<float> > >::iterator it = m_dragSelection.begin(); it != m_dragSelection.end(); it++)
							it->first->SetPosition(curPos + it->second);
						app->UnlockWorldMutex();
					}
					else if(m_isRotating)
					{
						Vector2<float> dif = curPos - m_rotateCenter;
						float angleOffset = BaseAngleRadians(atan2f(dif[1], dif[0]) - m_startRotation);

						app->LockWorldMutex();
						for(vector<RotateDef>::iterator it = m_rotateSelection.begin(); it != m_rotateSelection.end(); it++)
						{
							it->object->SetRotation(RadiansToDegrees() * BaseAngleRadians(angleOffset + it->angle));
							float offsetAngle = BaseAngleRadians(angleOffset + it->offsetAngle);
							it->object->SetPosition(Vector2<float>(cos(offsetAngle), sin(offsetAngle)) * it->offsetLength + m_rotateCenter);
						}
						app->UnlockWorldMutex();
					}
					else
						CheckHover(curPos);
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
						for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
							(*it)->Delete();
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

			return true;
		}
		void Editor2DCtrl::DoRender()
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();
			game2d::PhysicalObject *referenceObject = app->GetReferenceObject();
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
			else
			{
				glTranslatef(0.f, (float)m_extent[1], 0.f);
				glScalef(1.f, -1.f, 1.f);
				if(referenceObject)
				{
					Vector2<float> focusPosition = referenceObject->GetPosition();
					Vector2<float> focusExtent = referenceObject->GetExtent();

					Vector2<int_canvas> viewExtent = currentViewport->GetExtent();
					Vector2<float> half((float)viewExtent[0] / 2.f, (float)viewExtent[1] / 2.f);
					Vector2<float> viewScale = currentViewport->GetViewScale();
					glTranslatef(half[0] - viewScale[0] * focusPosition[0], half[1] - viewScale[1] * focusPosition[1], 0.f);
					glScalef(viewScale[0], viewScale[1], 1.f);
				}

				for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
				{
					game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;

					glPushMatrix();
					Vector2<float> position = object->GetPosition();
					Vector2<float> extent = object->GetExtent();

					glTranslatef(position[0], position[1], 0.f);
					glRotatef(object->GetRotation(), 0.f, 0.f, 1.f);
					glScalef(extent[0], extent[1], 1.f);
					glTranslatef(-0.5f, -0.5f, 0.f);
					glColor4ub(0, 0, 255, 200);
					RenderSquare();
					glPopMatrix();
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
			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
			{
				game2d::PhysicalObject *obj = *it;
				obj->SetCollidable(false);
				obj->SetVelocity(Vector2<float>(0.f, 0.f));
				obj->SetAngularVelocity(0.f);
				m_dragSelection.push_back(pair<game2d::PhysicalObject *, Vector2<float> >(obj, obj->GetPosition() - curPos));
			}
			app->UnlockWorldMutex();
			m_hover = 0;
			m_isDragging = true;
		}
		void Editor2DCtrl::StopDragging()
		{
			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
				(*it)->SetCollidable(true);
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
			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
			{
				game2d::PhysicalObject *obj = *it;
				if(obj->GetTypeMask() & ignoreMask)
					continue;
				totalPos += obj->GetPosition();
				N++;
			}

			m_rotateCenter = totalPos / (float)N;

			Vector2<float> dif = curPos - m_rotateCenter;
			m_startRotation = atan2f(dif[1], dif[0]);

			Vector2<float> offset;
			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
			{
				game2d::PhysicalObject *obj = *it;
				if(obj->GetTypeMask() & ignoreMask)
					continue;
				obj->SetCollidable(false);
				obj->SetVelocity(Vector2<float>(0.f, 0.f));
				obj->SetAngularVelocity(0.f);
				RotateDef rotateDef;
				rotateDef.object = obj;
				offset = obj->GetPosition() - m_rotateCenter;
				rotateDef.angle = obj->GetRotation() * DegreesToRadians();
				rotateDef.offsetLength = offset.GetLength();
				rotateDef.offsetAngle = atan2f(offset[1], offset[0]);
				m_rotateSelection.push_back(rotateDef);
			}
			app->UnlockWorldMutex();

			m_hover = 0;
			m_isRotating = true;
		}
		void Editor2DCtrl::StopRotating()
		{
			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
				(*it)->SetCollidable(true);
			m_rotateSelection.clear();
			m_isRotating = false;
		}
		void Editor2DCtrl::CheckHover(Vector2<float> position)
		{
			game2d::AppGame2D *app = (game2d::AppGame2D *)App::GetCurrent();

			for(vector<game2d::PhysicalObject *>::iterator it = m_selection.begin(); it != m_selection.end(); it++)
			{
				game2d::PhysicalObject *obj = *it;
				if(obj->CollidesWith(position))
				{
					m_hover = obj;
					return;
				}
			}
			m_hover = 0;
		}
		/*
		//- Prop Selector -
		Editor2DCtrl::PropSelectorCtrl::PropSelectorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent) : ColorCtrl(position, extent, Color(63, 63, 63, 127))
		{
		}
		void Editor2DCtrl::PropSelectorCtrl::DoRender()
		{
			ColorCtrl::DoRender();
		}
		void Editor2DCtrl::PropSelectorCtrl::OnSelect(short propType)
		{
			Editor2DCtrl *editor = (Editor2DCtrl *)GetParent();
			editor->m_propPlaceType = propType;
		}
		void Editor2DCtrl::PropSelectorCtrl::GenerateButtons()
		{
			vector<PropDef> *propDefTable = GetPropDefTable();
			int_canvas spacer = 16;
			int_canvas startX = spacer;
			int_canvas startY = spacer;
			for(vector<PropDef>::iterator it = propDefTable->begin(); it != propDefTable->end(); it++)
			{
				int_canvas width = (int_canvas)(64.f * it->extent[0]);
				int_canvas height = (int_canvas)(64.f * it->extent[1]);

				PropSelectCtrl *btn = new PropSelectCtrl(Vector2<int_canvas>(startX, startY), Vector2<int_canvas>(width, height), it->propType);
				Add(btn);

				startX += width;
				startX += spacer;
			}
		}
		bool Editor2DCtrl::PropSelectorCtrl::OnEvent(Event &event)
		{
			OnSelect(-1);
			return false;
		}

		bool Editor_PropSelectBtnDown(ui::ButtonCtrl *button)
		{
			return false;
		}
		bool Editor_PropSelectBtnUp(ui::ButtonCtrl *button)
		{
			Editor2DCtrl::PropSelectorCtrl *selector = (Editor2DCtrl::PropSelectorCtrl *)button->GetParent();
			selector->OnSelect((short)((Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl *)button)->m_propType);
			return false;
		}
		Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl::PropSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, unsigned short propType) : ButtonCtrl(position, extent), m_propType(propType)
		{
			m_propDef = PropDef::GetByType(propType);
			SetOnButtonDown(Editor_PropSelectBtnDown);
			SetOnButtonUp(Editor_PropSelectBtnUp);
		}
		void Editor2DCtrl::PropSelectorCtrl::PropSelectCtrl::DoRender()
		{
			if(!m_propDef)
				return;
			glScalef((float)m_extent[0], (float)m_extent[1], 0.f);
			m_propDef->UIRender();
		}*/
	}
}
