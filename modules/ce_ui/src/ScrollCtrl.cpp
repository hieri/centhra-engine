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

using namespace std;

namespace ce
{
	namespace ui
	{
		ScrollCtrl::ScrollCtrl(Vector2<short> position, Vector2<short> extent, Skin *skin) : Control(position, extent), m_skin(skin)
		{
			m_type = Type_ScrollCtrl;
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

				glBegin(GL_QUADS);

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
				}

				//- Overlay Scrolls -
				int hScrollWidth = 0;
				{
					Rect<int> topRighti = m_skin->GetRect(3, 0);
					Rect<float> topRightf = m_skin->GetRelativeRect(3, 0);

					Rect<int> righti = m_skin->GetRect(3, 1);
					Rect<float> rightf = m_skin->GetRelativeRect(3, 1);

					Rect<int> bottomRighti = m_skin->GetRect(3, 2);
					Rect<float> bottomRightf = m_skin->GetRelativeRect(3, 2);
					hScrollWidth = righti[0];

					glTexCoord2f(topRightf[0], topRightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), topRighti.GetHeight());
					glTexCoord2f(topRightf[0], topRightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), 0);
					glTexCoord2f(topRightf[2], topRightf[1]); glVertex2i(size[0], 0);
					glTexCoord2f(topRightf[2], topRightf[3]); glVertex2i(size[0], topRighti.GetHeight());

					glTexCoord2f(rightf[0], rightf[3]); glVertex2i(size[0] - topRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
					glTexCoord2f(rightf[0], rightf[1]); glVertex2i(size[0] - topRighti.GetWidth(), bottomRighti.GetHeight());
					glTexCoord2f(rightf[2], rightf[1]); glVertex2i(size[0], bottomRighti.GetHeight());
					glTexCoord2f(rightf[2], rightf[3]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());

					glTexCoord2f(bottomRightf[0], bottomRightf[3]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1]);
					glTexCoord2f(bottomRightf[0], bottomRightf[1]); glVertex2i(size[0] - bottomRighti.GetWidth(), size[1] - bottomRighti.GetHeight());
					glTexCoord2f(bottomRightf[2], bottomRightf[1]); glVertex2i(size[0], size[1] - bottomRighti.GetHeight());
					glTexCoord2f(bottomRightf[2], bottomRightf[3]); glVertex2i(size[0], size[1]);


//					Rect<int> rightScrollPiece = m_skin->GetRect(3, 2);
//					Rect<float> bottomRightf = m_skin->GetRelativeRect(3, 2);
				}

				glEnd();

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
			m_scrollExtent = wrap;
			m_childOffset[0] = (int_canvas)(m_scrollPercentage[0] * -m_scrollExtent[0]);
			m_childOffset[1] = (int_canvas)(m_scrollPercentage[1] * -m_scrollExtent[1]);
		}
		void ScrollCtrl::Scroll(Vector2<int_canvas> amt)
		{
			m_childOffset -= amt;
			Vector2<int_canvas> scrollEnd = (m_scrollExtent - m_extent) * -1;
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

			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->UpdateDimensions();
		}
		bool ScrollCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::MouseWheel:
				if(event.mouseWheel.isHorizontal)
					Scroll(Vector2<int_canvas>(-event.mouseWheel.delta / 10, 0));
				else
					Scroll(Vector2<int_canvas>(0, -event.mouseWheel.delta / 10));
				return false;
			}

			return true;
		}
	}
}
