#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/UI/CameraView2DCtrl.h>
#include <CE/Game2D/PhysicalGroup.h>

namespace ce
{
	namespace ui
	{
		CameraView2DCtrl::CameraView2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent) : Control(position, extent),
			m_vmChanged(true)
		{
			m_camera = 0;
			m_viewScale = Vector2<float>(1.f, 1.f);
		}
		void CameraView2DCtrl::OnSetPosition()
		{
			m_vmChanged = true;
		}
		void CameraView2DCtrl::OnSetExtent()
		{
			m_vmChanged = true;
		}
		game2d::Camera *CameraView2DCtrl::GetCamera() const
		{
			return m_camera;
		}
		void CameraView2DCtrl::SetCamera(game2d::Camera *camera)
		{
			m_camera = camera;
			m_vmChanged = true;
		}
		void CameraView2DCtrl::DoRender(RenderContext &context)
		{
			if(m_camera)
			{
				//TODO: Apply control's position and size to the view
				RenderContext worldContext = context;

				//- Calculate View Matrix -
				worldContext.modelViewMatrix = m_absoluteMatrix;

				//- Convert to World Coordinates -
				if(m_camera->VMChanged() || m_vmChanged)
					CalculateViewMatrix();
				worldContext.modelViewMatrix *= m_viewMatrix;

				if(worldContext.useShaders)
					worldContext.mvpMatrix = worldContext.projectionMatrix * worldContext.modelViewMatrix;

				//- Determine rendering bounds and render -
				Vector2<float> half((float)m_extent[0] / 2.f, (float)m_extent[1] / 2.f);
				Vector2<float> focalPoint = m_camera->GetFocalPoint();
				m_camera->GetFocusGroup()->Render((focalPoint[0]) - half[0] / m_viewScale[0], (focalPoint[1]) - half[1] / m_viewScale[1], (focalPoint[0]) + half[0] / m_viewScale[0], (focalPoint[1]) + half[1] / m_viewScale[1], worldContext);
			}
		}
		Vector2<float> CameraView2DCtrl::GetViewScale() const
		{
			return m_viewScale;
		}
		void CameraView2DCtrl::SetViewScale(Vector2<float> viewScale)
		{
			m_viewScale = viewScale;
			m_vmChanged = true;
		}

		//- View Matrix -
		void CameraView2DCtrl::CalculateViewMatrix()
		{
			if(m_camera->VMChanged())
				m_camera->CalculateViewMatrix();

			Vector2<float> half((float)m_extent[0] / 2.f, (float)m_extent[1] / 2.f);
			Vector2<float> focalPoint = m_camera->GetFocalPoint();
			m_viewMatrix = Matrix4x4<float>::BuildFromScale(Vector3<float>(1.f, -1.f, 1.f));
			m_viewMatrix *= Matrix4x4<float>::BuildFromTranslation(Vector3<float>(half[0] - m_viewScale[0] * focalPoint[0], -half[1] - m_viewScale[1] * focalPoint[1], 0.f));
			m_viewMatrix *= Matrix4x4<float>::BuildFromScale(Vector3<float>(m_viewScale[0], m_viewScale[1], 1.f));
			m_vmChanged = false;
		}
	}
}
