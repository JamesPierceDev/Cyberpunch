#include "Camera.h"

Camera::Camera(bool use) :
	m_useCamera(use),
	m_scalar(MIN_ZOOM)
{
	zoom(MIN_ZOOM);

	//Set the view
	m_view.x = 0;
	m_view.y = 0;
	m_view.w = SCREEN_WIDTH;
	m_view.h = SCREEN_HEIGHT;
}

void Camera::update(double dt)
{
	if (m_scalar != m_desiredScalar)
	{
		if (m_desiredScalar > m_scalar)
		{
			m_scalar += .0002 * dt;
		}
		else
		{
			m_scalar -= .0002 * dt;
		}

		if ((m_scalar - m_desiredScalar) > -0.005
			|| (m_scalar - m_desiredScalar) < 0.005)
		{
			m_scalar = m_desiredScalar;
		}
	}
}

void Camera::centerCamera(float x, float y)
{
	m_centerPoint.x = x;
	m_centerPoint.y = y;
	center();
}

void Camera::centerCamera(Vector2f pos)
{
	m_centerPoint = pos;
	center();
}

void Camera::zoom(float scalar)
{
	m_desiredScalar = scalar;

	if (m_desiredScalar < MIN_ZOOM)
		m_desiredScalar = MIN_ZOOM;
	if (m_desiredScalar > MAX_ZOOM)
		m_desiredScalar = MAX_ZOOM;
}

void Camera::setZoom(SDL_Renderer * renderer)
{
	if (m_useCamera)
	{
		SDL_RenderSetScale(renderer, m_scalar, m_scalar);

		m_view.w = SCREEN_WIDTH / m_scalar;
		m_view.h = SCREEN_HEIGHT / m_scalar;
	}
}

void Camera::center()
{
	m_view.x = m_centerPoint.x * m_scalar - (SCREEN_WIDTH / 2);
	m_view.y = m_centerPoint.y * m_scalar - (SCREEN_HEIGHT / 2);

	//Clamping the view
	if (m_view.x < 0)
		m_view.x = 0;
	if (m_view.y < 0)
		m_view.y = 0;
	if (m_view.x + m_view.w > SCREEN_WIDTH)
		m_view.x = SCREEN_WIDTH - m_view.w;
		//m_view.x = ((SCREEN_WIDTH / m_scalar) - m_view.w) ;
	if (m_view.y + m_view.h > SCREEN_HEIGHT)
		m_view.y = SCREEN_HEIGHT - m_view.h;
}
