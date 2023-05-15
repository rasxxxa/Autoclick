#include "Autoclick.h"

void Autoclick::ClickListener()
{
	while (true)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x800) != 0 && !m_position.has_value())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			m_clicks++;
			LPPOINT point = new tagPOINT();
			GetCursorPos(point);
			std::cout << 3 - m_clicks << " clicks till fixation!" << std::endl;
			std::cout << point->x << " " << point->y << std::endl;
			if (m_clicks >= 3)
			{
				std::lock_guard<std::recursive_mutex> lockWrite(m_writeLock);
				m_position.emplace(point->x, point->y);
				m_conditional.notify_all();
			}
			delete point;
		}
		else if ((GetKeyState(VK_RBUTTON) & 0x800) != 0)
		{
			std::lock_guard<std::recursive_mutex> lockWrite(m_writeLock);
			m_clicks = 0;
			m_position.reset();
		}
	}
}

void Autoclick::RunAutoClickThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_conditionalLock);
		m_conditional.wait(lock, [this] { return m_position.has_value(); });
		while (m_timesToClick && m_position.has_value())
		{
			std::unique_lock<std::recursive_mutex> lockWrite(m_writeLock);
			if (m_position.has_value())
			{
				SetCursorPos(m_position.value().first, m_position.value().second);

				mouse_event(MOUSEEVENTF_LEFTDOWN, m_position.value().first, m_position.value().second, 0, 0);

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				mouse_event(MOUSEEVENTF_LEFTUP, m_position.value().first, m_position.value().second, 0, 0);
			}
			lockWrite.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepBetweenClicks));
		}
	}
}

void Autoclick::RunAutoclicker()
{
	m_clickListener = std::jthread(&Autoclick::ClickListener, this);
	m_clicker = std::jthread(&Autoclick::RunAutoClickThread, this);
	m_clicker.join();
}
