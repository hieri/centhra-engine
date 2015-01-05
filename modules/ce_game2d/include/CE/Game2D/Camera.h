#ifndef _CE_GAME2D_CAMERA_H_
#define _CE_GAME2D_CAMERA_H_

//- Centhra Engine -
#include <CE/Matrix4x4.h>
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Camera : public Entity
		{
			static Camera *ms_current;

			//- Mode -
		protected:
			unsigned char m_mode;
		public:
			typedef enum CameraMode {
				Mode_Free,
				Mode_Follow
			} CameraMode;
			unsigned char GetMode() const;
			void SetMode(unsigned char mode);

		public:
			static Camera *GetCurrent();

			Camera(PhysicalGroup *group, unsigned char mode = Mode_Follow);
			void SetCurrent();

			//- Focus -
		protected:
			PhysicalObject *m_focus;
			PhysicalGroup *m_focusGroup;
		public:
			PhysicalObject *GetFocus() const;
			void SetFocus(PhysicalObject *focus);
			//- Returns focus's parent group if following focus -
			PhysicalGroup *GetFocusGroup() const;
			void SetFocusGroup(PhysicalGroup *focusGroup);

			//- Position -
		protected:
			Vector2<float> m_position;
		public:
			Vector2<float> GetPosition() const;
			void SetPosition(Vector2<float> position);

			//- Focal Point -
		public:
			//- Returns focus position if following focus -
			Vector2<float> GetFocalPoint();

			//- Offset -
		protected:
			Vector2<float> m_offset;
		public:
			Vector2<float> GetOffset() const;
			void SetOffset(Vector2<float> offset);

			//- View Matrix -
		protected:
			Matrix4x4<float> m_viewMatrix;
			bool m_vmChanged;
		public:
			bool VMChanged() const;
			void CalculateViewMatrix();
			inline Matrix4x4<float> GetViewMatrix() const
			{
				return m_viewMatrix;
			}
		};
	}
}

#endif
