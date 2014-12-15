//- Standard Library -
#include <cstring>

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/ScrollCtrl.h>
#include <CE/Base.h>
#include <CE/RenderPrimitives.h>

using namespace std;

//TODO: overflow based scroll bar sizing
//TODO: move rendering calculations into a function, called in scroll and resize

namespace ce
{
	namespace ui
	{
		ScrollCtrl::ScrollCtrl(Vector2<short> position, Vector2<short> extent, Skin *skin) : Control(position, extent), m_skin(skin), m_horizontalScroll(false), m_verticalScroll(false)
		{
			m_type = Type_ScrollCtrl;
			m_eventMask |= event::Mask_MouseScroll;
			m_hasOverlay = true;

			m_hasControlZones = true;
			ControlZone horizontalScroll, verticalScroll;
			memset(&horizontalScroll, 0, sizeof(ControlZone));
			memset(&verticalScroll, 0, sizeof(ControlZone));
			horizontalScroll.id = 1;
			verticalScroll.id = 2;
			horizontalScroll.canMove = true;
			verticalScroll.canMove = true;

			m_controlZones.push_back(horizontalScroll);
			m_controlZones.push_back(verticalScroll);
		}
		void ScrollCtrl::DoRender()
		{
			glPushMatrix();
				Vector2<int_canvas> size = GetExtent();

				Image *source = m_skin->GetImage();
				if(source)
					source->Bind();

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);

				glColor4f(1.f, 1.f, 1.f, 1.f);

				//- Render Background -
				{
					Rect<int> topLefti = m_skin->GetRect(0, 0);
					Rect<float> topLeftf = m_skin->GetRelativeRect(0, 0);

					Rect<int> topi = m_skin->GetRect(1, 0);
					Rect<float> topf = m_skin->GetRelativeRect(1, 0);
			
					Rect<int> topRighti = m_skin->GetRect(2, 0);
					Rect<float> topRightf = m_skin->GetRelativeRect(2, 0);
			
					Rect<int> lefti = m_skin->GetRect(0, 1);
					Rect<float> leftf = m_skin->GetRelativeRect(0, 1);

					Rect<float> centerf = m_skin->GetRelativeRect(1, 1);
			
					Rect<int> righti = m_skin->GetRect(2, 1);
					Rect<float> rightf = m_skin->GetRelativeRect(2, 1);
			
					Rect<int> bottomLefti = m_skin->GetRect(0, 2);
					Rect<float> bottomLeftf = m_skin->GetRelativeRect(0, 2);

					Rect<int> bottomi = m_skin->GetRect(1, 2);
					Rect<float> bottomf = m_skin->GetRelativeRect(1, 2);
			
					Rect<int> bottomRighti = m_skin->GetRect(2, 2);
					Rect<float> bottomRightf = m_skin->GetRelativeRect(2, 2);

					glBegin(GL_QUADS);

						//- Top -
						glTexCoord2f(topLeftf[0], topLeftf[3]); glVertex2i(0, topLefti.GetHeight());
						glTexCoord2f(topLeftf[0], topLeftf[1]); glVertex2i(0, 0);
						glTexCoord2f(topLeftf[2], topLeftf[1]); glVertex2i(topLefti.GetWidth(), 0);
						glTexCoord2f(topLeftf[2], topLeftf[3]); glVertex2i(topLefti.GetWidth(), topLefti.GetHeight());
				
						glTexCoord2f(topf[0], topf[3]); glVertex2i(topLefti.GetWidth(), topi.GetHeight());
						glTexCoord2f(topf[0], topf[1]); glVertex2i(topLefti.GetWidth(), 0);
						glTexCoord2f(topf[2], topf[1]); glVertex2i(size[0] - topRighti.GetWidth(), 0);
						glTexCoord2f(topf[2], topf[3]); glVertex2i(size[0] - topRighti.GetWidth(), topi.GetHeight());

						glTexCoord2f(topRightf[0], topRightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), topRighti.GetHeight());
						glTexCoord2f(topRightf[0], topRightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), 0);
						glTexCoord2f(topRightf[2], topRightf[1]); glVertex2i(size[0], 0);
						glTexCoord2f(topRightf[2], topRightf[3]); glVertex2i(size[0], topRighti.GetHeight());

						//- Middle -
						glTexCoord2f(leftf[0], leftf[3]); glVertex2i(0, size[1] - bottomLefti.GetHeight());
						glTexCoord2f(leftf[0], leftf[1]); glVertex2i(0, topLefti.GetHeight());
						glTexCoord2f(leftf[2], leftf[1]); glVertex2i(topLefti.GetWidth(), topLefti.GetHeight());
						glTexCoord2f(leftf[2], leftf[3]); glVertex2i(topLefti.GetWidth(), size[1] - bottomLefti.GetHeight());
				
						glTexCoord2f(centerf[0], centerf[3]); glVertex2i(topLefti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(centerf[0], centerf[1]); glVertex2i(topLefti.GetWidth(),  bottomRighti.GetHeight());
						glTexCoord2f(centerf[2], centerf[1]); glVertex2i(size[0] - topRighti.GetWidth(),  bottomRighti.GetHeight());
						glTexCoord2f(centerf[2], centerf[3]); glVertex2i(size[0] - topRighti.GetWidth(), size[1] - bottomRighti.GetHeight());

						glTexCoord2f(rightf[0], rightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(rightf[0], rightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), bottomRighti.GetHeight());
						glTexCoord2f(rightf[2], rightf[1]); glVertex2i(size[0], bottomRighti.GetHeight());
						glTexCoord2f(rightf[2], rightf[3]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());

						//- Bottom -
						glTexCoord2f(bottomLeftf[0], bottomLeftf[3]); glVertex2i(0, size[1]);
						glTexCoord2f(bottomLeftf[0], bottomLeftf[1]); glVertex2i(0, size[1] - bottomLefti.GetHeight());
						glTexCoord2f(bottomLeftf[2], bottomLeftf[1]); glVertex2i(bottomLefti.GetWidth(), size[1] - bottomLefti.GetHeight());
						glTexCoord2f(bottomLeftf[2], bottomLeftf[3]); glVertex2i(bottomLefti.GetWidth(), size[1]);
				
						glTexCoord2f(bottomf[0], bottomf[3]); glVertex2i(bottomLefti.GetWidth(), size[1]);
						glTexCoord2f(bottomf[0], bottomf[1]); glVertex2i(bottomLefti.GetWidth(), size[1] - bottomi.GetHeight());
						glTexCoord2f(bottomf[2], bottomf[1]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1] - bottomi.GetHeight());
						glTexCoord2f(bottomf[2], bottomf[3]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1]);

						glTexCoord2f(bottomRightf[0], bottomRightf[3]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1]);
						glTexCoord2f(bottomRightf[0], bottomRightf[1]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(bottomRightf[2], bottomRightf[1]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());
						glTexCoord2f(bottomRightf[2], bottomRightf[3]); glVertex2i(size[0], size[1]);

					glEnd();
				}

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
			glPopMatrix();
			glPushMatrix();
				glTranslatef((float)m_controlZones[0].x, (float)m_controlZones[0].y, 0.f);
				glScalef((float)m_controlZones[0].width, (float)m_controlZones[0].height, 1.f);
				RenderSquare();
			glPopMatrix();
			glPushMatrix();
				glTranslatef((float)m_controlZones[1].x, (float)m_controlZones[1].y, 0.f);
				glScalef((float)m_controlZones[1].width, (float)m_controlZones[1].height, 1.f);
				RenderSquare();
			glPopMatrix();
		}
		void ScrollCtrl::DoOverlay()
		{
			glPushMatrix();
				Vector2<int_canvas> size = GetExtent();

				Image *source = m_skin->GetImage();
				if(source)
					source->Bind();

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);

				glColor4f(1.f, 1.f, 1.f, 1.f);

				//- Render Background -
				{
					Rect<int> topLefti = m_skin->GetRect(3, 0);
					Rect<float> topLeftf = m_skin->GetRelativeRect(3, 0);

					Rect<int> topi = m_skin->GetRect(4, 0);
					Rect<float> topf = m_skin->GetRelativeRect(4, 0);
			
					Rect<int> topRighti = m_skin->GetRect(5, 0);
					Rect<float> topRightf = m_skin->GetRelativeRect(5, 0);
			
					Rect<int> lefti = m_skin->GetRect(3, 1);
					Rect<float> leftf = m_skin->GetRelativeRect(3, 1);

					Rect<float> centerf = m_skin->GetRelativeRect(4, 1);
			
					Rect<int> righti = m_skin->GetRect(5, 1);
					Rect<float> rightf = m_skin->GetRelativeRect(5, 1);
			
					Rect<int> bottomLefti = m_skin->GetRect(3, 2);
					Rect<float> bottomLeftf = m_skin->GetRelativeRect(3, 2);

					Rect<int> bottomi = m_skin->GetRect(4, 2);
					Rect<float> bottomf = m_skin->GetRelativeRect(4, 2);
			
					Rect<int> bottomRighti = m_skin->GetRect(5, 2);
					Rect<float> bottomRightf = m_skin->GetRelativeRect(5, 2);

					glBegin(GL_QUADS);

						//- Top -
						glTexCoord2f(topLeftf[0], topLeftf[3]); glVertex2i(0, topLefti.GetHeight());
						glTexCoord2f(topLeftf[0], topLeftf[1]); glVertex2i(0, 0);
						glTexCoord2f(topLeftf[2], topLeftf[1]); glVertex2i(topLefti.GetWidth(), 0);
						glTexCoord2f(topLeftf[2], topLeftf[3]); glVertex2i(topLefti.GetWidth(), topLefti.GetHeight());
				
						glTexCoord2f(topf[0], topf[3]); glVertex2i(topLefti.GetWidth(), topi.GetHeight());
						glTexCoord2f(topf[0], topf[1]); glVertex2i(topLefti.GetWidth(), 0);
						glTexCoord2f(topf[2], topf[1]); glVertex2i(size[0] - topRighti.GetWidth(), 0);
						glTexCoord2f(topf[2], topf[3]); glVertex2i(size[0] - topRighti.GetWidth(), topi.GetHeight());

						glTexCoord2f(topRightf[0], topRightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), topRighti.GetHeight());
						glTexCoord2f(topRightf[0], topRightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), 0);
						glTexCoord2f(topRightf[2], topRightf[1]); glVertex2i(size[0], 0);
						glTexCoord2f(topRightf[2], topRightf[3]); glVertex2i(size[0], topRighti.GetHeight());

						//- Middle -
						glTexCoord2f(leftf[0], leftf[3]); glVertex2i(0, size[1] - bottomLefti.GetHeight());
						glTexCoord2f(leftf[0], leftf[1]); glVertex2i(0, topLefti.GetHeight());
						glTexCoord2f(leftf[2], leftf[1]); glVertex2i(topLefti.GetWidth(), topLefti.GetHeight());
						glTexCoord2f(leftf[2], leftf[3]); glVertex2i(topLefti.GetWidth(), size[1] - bottomLefti.GetHeight());
				
						glTexCoord2f(centerf[0], centerf[3]); glVertex2i(topLefti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(centerf[0], centerf[1]); glVertex2i(topLefti.GetWidth(),  bottomRighti.GetHeight());
						glTexCoord2f(centerf[2], centerf[1]); glVertex2i(size[0] - topRighti.GetWidth(),  bottomRighti.GetHeight());
						glTexCoord2f(centerf[2], centerf[3]); glVertex2i(size[0] - topRighti.GetWidth(), size[1] - bottomRighti.GetHeight());

						glTexCoord2f(rightf[0], rightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(rightf[0], rightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), bottomRighti.GetHeight());
						glTexCoord2f(rightf[2], rightf[1]); glVertex2i(size[0], bottomRighti.GetHeight());
						glTexCoord2f(rightf[2], rightf[3]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());

						//- Bottom -
						glTexCoord2f(bottomLeftf[0], bottomLeftf[3]); glVertex2i(0, size[1]);
						glTexCoord2f(bottomLeftf[0], bottomLeftf[1]); glVertex2i(0, size[1] - bottomLefti.GetHeight());
						glTexCoord2f(bottomLeftf[2], bottomLeftf[1]); glVertex2i(bottomLefti.GetWidth(), size[1] - bottomLefti.GetHeight());
						glTexCoord2f(bottomLeftf[2], bottomLeftf[3]); glVertex2i(bottomLefti.GetWidth(), size[1]);
				
						glTexCoord2f(bottomf[0], bottomf[3]); glVertex2i(bottomLefti.GetWidth(), size[1]);
						glTexCoord2f(bottomf[0], bottomf[1]); glVertex2i(bottomLefti.GetWidth(), size[1] - bottomi.GetHeight());
						glTexCoord2f(bottomf[2], bottomf[1]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1] - bottomi.GetHeight());
						glTexCoord2f(bottomf[2], bottomf[3]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1]);

						glTexCoord2f(bottomRightf[0], bottomRightf[3]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1]);
						glTexCoord2f(bottomRightf[0], bottomRightf[1]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
						glTexCoord2f(bottomRightf[2], bottomRightf[1]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());
						glTexCoord2f(bottomRightf[2], bottomRightf[3]); glVertex2i(size[0], size[1]);

					glEnd();
				}

				//- Overlay Scrolls -
				{
					int vBackWidth, hBackHeight;

					if(m_verticalScroll)
					{
						//- Get Skin Dimensions -
						Rect<int> backTopi = m_skin->GetRect(3, 3);
						Rect<float> backTopf = m_skin->GetRelativeRect(3, 3);

						Rect<int> backMidi = m_skin->GetRect(4, 3);
						Rect<float> backMidf = m_skin->GetRelativeRect(4, 3);

						Rect<int> backBottomi = m_skin->GetRect(5, 3);
						Rect<float> backBottomf = m_skin->GetRelativeRect(5, 3);

						Rect<int> barTopi = m_skin->GetRect(0, 5);
						Rect<float> barTopf = m_skin->GetRelativeRect(0, 5);

						Rect<int> barMidi = m_skin->GetRect(1, 5);
						Rect<float> barMidf = m_skin->GetRelativeRect(1, 5);

						Rect<int> barBottomi = m_skin->GetRect(2, 5);
						Rect<float> barBottomf = m_skin->GetRelativeRect(2, 5);

						//- Get Scroll Dimensions -
						vBackWidth = backMidi.GetWidth();
						int barSize = 32;
						int backSize = m_extent[1];
						int barArea = m_extent[1] - barSize;

						if(m_horizontalScroll)
						{
							//- Account for Horizontal Scroll -
							int horizontalHeight = m_skin->GetRect(0, 3).GetHeight();
							barArea -= horizontalHeight;
							backSize -= horizontalHeight;
						}

						int backMidLength = backSize - backTopi.GetHeight() - backBottomi.GetHeight();

						int barOffset = (int)((float)(vBackWidth - barMidi.GetWidth()) / 2.f);
						int barStart = (int)(m_scrollPercentage[1] * barArea);
						int barMidLength = barSize - barTopi.GetHeight() - barBottomi.GetHeight();

						//- Render Scroll Background -
						glPushMatrix();
							glTranslatef((float)(size[0] - vBackWidth), 0.f, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backTopf[0], backTopf[3]); glVertex2i(0, backTopi.GetHeight());
								glTexCoord2f(backTopf[0], backTopf[1]); glVertex2i(0, 0);
								glTexCoord2f(backTopf[2], backTopf[1]); glVertex2i(backTopi.GetWidth(), 0);
								glTexCoord2f(backTopf[2], backTopf[3]); glVertex2i(backTopi.GetWidth(), backTopi.GetHeight());
							glEnd();
							glTranslatef(0.f, (float)backTopi.GetHeight(), 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backMidf[0], backMidf[3]); glVertex2i(0, 0);
								glTexCoord2f(backMidf[0], backMidf[1]); glVertex2i(0, backMidLength);
								glTexCoord2f(backMidf[2], backMidf[1]); glVertex2i(backTopi.GetWidth(), backMidLength);
								glTexCoord2f(backMidf[2], backMidf[3]); glVertex2i(backTopi.GetWidth(), 0);
								glEnd();
								glTranslatef(0.f, (float)backMidLength, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backBottomf[0], backBottomf[3]); glVertex2i(0, backBottomi.GetHeight());
								glTexCoord2f(backBottomf[0], backBottomf[1]); glVertex2i(0, 0);
								glTexCoord2f(backBottomf[2], backBottomf[1]); glVertex2i(backBottomi.GetWidth(), 0);
								glTexCoord2f(backBottomf[2], backBottomf[3]); glVertex2i(backBottomi.GetWidth(), backBottomi.GetHeight());
							glEnd();
						glPopMatrix();

						//- Render Scroll Bar -
						glPushMatrix();
							glTranslatef((float)(size[0] - vBackWidth + barOffset), (float)barStart, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barTopf[0], barTopf[3]); glVertex2i(0, barTopi.GetHeight());
								glTexCoord2f(barTopf[0], barTopf[1]); glVertex2i(0, 0);
								glTexCoord2f(barTopf[2], barTopf[1]); glVertex2i(barTopi.GetWidth(), 0);
								glTexCoord2f(barTopf[2], barTopf[3]); glVertex2i(barTopi.GetWidth(), barTopi.GetHeight());
							glEnd();
							glTranslatef(0.f, (float)barTopi.GetHeight(), 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barMidf[0], barMidf[3]); glVertex2i(0, 0);
								glTexCoord2f(barMidf[0], barMidf[1]); glVertex2i(0, barMidLength);
								glTexCoord2f(barMidf[2], barMidf[1]); glVertex2i(barTopi.GetWidth(), barMidLength);
								glTexCoord2f(barMidf[2], barMidf[3]); glVertex2i(barTopi.GetWidth(), 0);
							glEnd();
							glTranslatef(0.f, (float)barMidLength, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barBottomf[0], barBottomf[3]); glVertex2i(0, barBottomi.GetHeight());
								glTexCoord2f(barBottomf[0], barBottomf[1]); glVertex2i(0, 0);
								glTexCoord2f(barBottomf[2], barBottomf[1]); glVertex2i(barBottomi.GetWidth(), 0);
								glTexCoord2f(barBottomf[2], barBottomf[3]); glVertex2i(barBottomi.GetWidth(), barBottomi.GetHeight());
							glEnd();
						glPopMatrix();
					}

					if(m_horizontalScroll)
					{
						//- Get Skin Dimensions -
						Rect<int> backLefti = m_skin->GetRect(0, 3);
						Rect<float> backLeftf = m_skin->GetRelativeRect(0, 3);

						Rect<int> backMidi = m_skin->GetRect(1, 3);
						Rect<float> backMidf = m_skin->GetRelativeRect(1, 3);

						Rect<int> backRighti = m_skin->GetRect(2, 3);
						Rect<float> backRightf = m_skin->GetRelativeRect(2, 3);

						Rect<int> barLefti = m_skin->GetRect(0, 4);
						Rect<float> barLeftf = m_skin->GetRelativeRect(0, 4);

						Rect<int> barMidi = m_skin->GetRect(1, 4);
						Rect<float> barMidf = m_skin->GetRelativeRect(1, 4);

						Rect<int> barRighti = m_skin->GetRect(2, 4);
						Rect<float> barRightf = m_skin->GetRelativeRect(2, 4);

						//- Get Scroll Dimensions -
						hBackHeight = backMidi.GetHeight();
						int barSize = 32;
						int backSize = m_extent[0];
						int barArea = m_extent[0] - barSize;

						if(m_verticalScroll)
						{
							//- Account for Vertical Scroll -
							int verticalWidth = m_skin->GetRect(3, 3).GetWidth();
							barArea -= verticalWidth;
							backSize -= verticalWidth;
						}

						int backMidLength = backSize - backLefti.GetWidth() - backRighti.GetWidth();

						int barOffset = (int)((float)(hBackHeight - barMidi.GetHeight()) / 2.f);
						int barStart = (int)(m_scrollPercentage[0] * barArea);
						int barMidLength = barSize - barLefti.GetWidth() - barRighti.GetWidth();

						//- Render Scroll Background -
						glPushMatrix();
							glTranslatef(0.f, (float)(size[1] - hBackHeight), 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backLeftf[0], backLeftf[3]); glVertex2i(0, backLefti.GetHeight());
								glTexCoord2f(backLeftf[0], backLeftf[1]); glVertex2i(0, 0);
								glTexCoord2f(backLeftf[2], backLeftf[1]); glVertex2i(backLefti.GetWidth(), 0);
								glTexCoord2f(backLeftf[2], backLeftf[3]); glVertex2i(backLefti.GetWidth(), backLefti.GetHeight());
							glEnd();
							glTranslatef((float)backLefti.GetWidth(), 0.f, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backMidf[0], backMidf[3]); glVertex2i(0, 0);
								glTexCoord2f(backMidf[0], backMidf[1]); glVertex2i(0, backLefti.GetHeight());
								glTexCoord2f(backMidf[2], backMidf[1]); glVertex2i(backMidLength, backLefti.GetHeight());
								glTexCoord2f(backMidf[2], backMidf[3]); glVertex2i(backMidLength, 0);
							glEnd();
							glTranslatef((float)backMidLength, 0.f, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(backRightf[0], backRightf[3]); glVertex2i(0, backRighti.GetHeight());
								glTexCoord2f(backRightf[0], backRightf[1]); glVertex2i(0, 0);
								glTexCoord2f(backRightf[2], backRightf[1]); glVertex2i(backRighti.GetWidth(), 0);
								glTexCoord2f(backRightf[2], backRightf[3]); glVertex2i(backRighti.GetWidth(), backRighti.GetHeight());
							glEnd();
						glPopMatrix();

						//- Render Scroll Bar -
						glPushMatrix();
							glTranslatef((float)barStart, (float)(size[1] - hBackHeight + barOffset), 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barLeftf[0], barLeftf[3]); glVertex2i(0, barLefti.GetHeight());
								glTexCoord2f(barLeftf[0], barLeftf[1]); glVertex2i(0, 0);
								glTexCoord2f(barLeftf[2], barLeftf[1]); glVertex2i(barLefti.GetWidth(), 0);
								glTexCoord2f(barLeftf[2], barLeftf[3]); glVertex2i(barLefti.GetWidth(), barLefti.GetHeight());
							glEnd();
							glTranslatef((float)barLefti.GetWidth(), 0.f, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barMidf[0], barMidf[3]); glVertex2i(0, 0);
								glTexCoord2f(barMidf[0], barMidf[1]); glVertex2i(0, barLefti.GetHeight());
								glTexCoord2f(barMidf[2], barMidf[1]); glVertex2i(barMidLength, barLefti.GetHeight());
								glTexCoord2f(barMidf[2], barMidf[3]); glVertex2i(barMidLength, 0);
							glEnd();
							glTranslatef((float)barMidLength, 0.f, 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(barRightf[0], barRightf[3]); glVertex2i(0, barRighti.GetHeight());
								glTexCoord2f(barRightf[0], barRightf[1]); glVertex2i(0, 0);
								glTexCoord2f(barRightf[2], barRightf[1]); glVertex2i(barRighti.GetWidth(), 0);
								glTexCoord2f(barRightf[2], barRightf[3]); glVertex2i(barRighti.GetWidth(), barRighti.GetHeight());
							glEnd();
						glPopMatrix();
					}

					if(m_verticalScroll && m_horizontalScroll)
					{
						Rect<int> corneri = m_skin->GetRect(6, 3);
						Rect<float> cornerf = m_skin->GetRelativeRect(6, 3);
						glPushMatrix();
							glTranslatef((float)(size[0] - vBackWidth), (float)(size[1] - hBackHeight), 0.f);
							glBegin(GL_QUADS);
								glTexCoord2f(cornerf[0], cornerf[3]); glVertex2i(0, corneri.GetHeight());
								glTexCoord2f(cornerf[0], cornerf[1]); glVertex2i(0, 0);
								glTexCoord2f(cornerf[2], cornerf[1]); glVertex2i(corneri.GetWidth(), 0);
								glTexCoord2f(cornerf[2], cornerf[3]); glVertex2i(corneri.GetWidth(), corneri.GetHeight());
							glEnd();
						glPopMatrix();
					}
				}

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
			glPopMatrix();
		}
		void ScrollCtrl::OnMemberAdded(Control *ctrl)
		{
			UpdateScroll();
		}
		void ScrollCtrl::OnMemberRemoved(Control *ctrl)
		{
			UpdateScroll();
		}
		void ScrollCtrl::UpdateScroll()
		{
			Vector2<int_canvas> wrap;
			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
			{
				Control *ctrl = *it;
				Vector2<int_canvas> farthest = ctrl->GetPosition() + ctrl->GetExtent();
				if(farthest[0] > wrap[0])
					wrap[0] = farthest[0];
				if(farthest[1] > wrap[1])
					wrap[1] = farthest[1];
			}
			if(wrap[0] > m_extent[0])
			{
				m_horizontalScroll = true;
				m_scrollExtent[0] = wrap[0] - m_extent[0];
			}
			else
				m_scrollExtent[0] = 0;
			if(wrap[1] > m_extent[1])
			{
				m_verticalScroll = true;
				m_scrollExtent[1] = wrap[1] - m_extent[1];
			}
			else
				m_scrollExtent[1] = 0;

			m_childOffset[0] = (int_canvas)(m_scrollPercentage[0] * -m_scrollExtent[0]);
			m_childOffset[1] = (int_canvas)(m_scrollPercentage[1] * -m_scrollExtent[1]);

			UpdateControlZones();

			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->UpdateDimensions();
		}
		void ScrollCtrl::UpdateControlZones()
		{
			ControlZone &horizontalScroll = m_controlZones[0];
			ControlZone &verticalScroll = m_controlZones[1];
			int barSize = 32;
			int vBackWidth = m_skin->GetRect(4, 3).GetWidth(), hBackHeight = m_skin->GetRect(1, 3).GetHeight();

			if(m_horizontalScroll)
			{
				horizontalScroll.minY = m_extent[1] - hBackHeight;
				horizontalScroll.maxY = m_extent[1];
				if(m_verticalScroll)
					horizontalScroll.maxX = m_extent[0] - vBackWidth;
				else
					horizontalScroll.maxX = m_extent[0];
				horizontalScroll.width = barSize;
				horizontalScroll.height = hBackHeight;

				int barArea = m_extent[0] - barSize;
				if(m_verticalScroll)
				{
					//- Account for Vertical Scroll -
					barArea -= m_skin->GetRect(3, 3).GetHeight();
				}
				horizontalScroll.x = (int)(m_scrollPercentage[0] * barArea);

				if(horizontalScroll.y < horizontalScroll.minY)
					horizontalScroll.y = horizontalScroll.minY;
			}
			else
			{
				horizontalScroll.height = 0;
				horizontalScroll.width = 0;
			}

			if(m_verticalScroll)
			{
				verticalScroll.minX = m_extent[0] - vBackWidth;
				verticalScroll.maxX = m_extent[0];
				if(m_horizontalScroll)
					verticalScroll.maxY = m_extent[1] - hBackHeight;
				else
					verticalScroll.maxY = m_extent[1];
				verticalScroll.height = barSize;
				verticalScroll.width = vBackWidth;

				int barArea = m_extent[1] - barSize;
				if(m_horizontalScroll)
				{
					//- Account for Horizontal Scroll -
					barArea -= m_skin->GetRect(0, 3).GetHeight();
				}
				verticalScroll.y = (int)(m_scrollPercentage[1] * barArea);

				if(verticalScroll.x < verticalScroll.minX)
					verticalScroll.x = verticalScroll.minX;
			}
			else
			{
				verticalScroll.height = 0;
				verticalScroll.width = 0;
			}
		}
		void ScrollCtrl::Scroll(Vector2<int_canvas> amt)
		{
			m_childOffset -= amt;
			Vector2<int_canvas> scrollEnd = m_scrollExtent * -1;
			if(scrollEnd[0] > 0)
				scrollEnd[0] = 0;
			if(scrollEnd[1] > 0)
				scrollEnd[1] = 0;

			if(m_childOffset[0] < scrollEnd[0])
				m_childOffset[0] = scrollEnd[0];
			if(m_childOffset[1] < scrollEnd[1])
				m_childOffset[1] = scrollEnd[1];
			if(m_childOffset[0] > 0)
				m_childOffset[0] = 0;
			if(m_childOffset[1] > 0)
				m_childOffset[1] = 0;

			m_scrollPercentage[0] = ((float)m_childOffset[0]) / -m_scrollExtent[0];
			m_scrollPercentage[1] = ((float)m_childOffset[1]) / -m_scrollExtent[1];

			UpdateControlZones();

			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->UpdateDimensions();
		}
		bool ScrollCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::MouseScroll:
				if(event.mouseScroll.isHorizontal)
					Scroll(Vector2<int_canvas>(-event.mouseScroll.delta / 10, 0));
				else
					Scroll(Vector2<int_canvas>(0, -event.mouseScroll.delta / 10));
				return false;
			}
			return Control::OnEvent(event);
		}
		void ScrollCtrl::OnControlZoneMove(ControlZone *zone)
		{
			if(zone->id == 1)
			{
				int barSize = 32;
				int barArea = m_extent[0] - barSize;
				if(m_verticalScroll)
				{
					//- Account for Vertical Scroll -
					barArea -= m_skin->GetRect(3, 3).GetHeight();
				}
				m_scrollPercentage[0] = (float)zone->x / barArea;
				m_childOffset[0] = (int_canvas)(m_scrollPercentage[0] * -m_scrollExtent[0]);
				for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
					(*it)->UpdateDimensions();
			}
			else if(zone->id == 2)
			{
				int barSize = 32;
				int barArea = m_extent[1] - barSize;
				if(m_horizontalScroll)
				{
					//- Account for Horizontal Scroll -
					barArea -= m_skin->GetRect(0, 3).GetHeight();
				}
				m_scrollPercentage[1] = (float)zone->y / barArea;
				m_childOffset[1] = (int_canvas)(m_scrollPercentage[1] * -m_scrollExtent[1]);
				for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
					(*it)->UpdateDimensions();
			}
		}
	}
}
