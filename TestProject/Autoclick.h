#pragma once
#include <atomic>
#include <condition_variable>
#include <Windows.h>
#include <optional>
#include <iostream>
#include <barrier>
#include <thread>

struct AutoclickSetup
{
	unsigned howManyClicks;
	unsigned sleepBetweenClicks;
};

class Autoclick final
{
private:
	std::atomic_int m_clicks;
	std::condition_variable m_conditional;
	std::recursive_mutex m_writeLock;
	std::mutex m_conditionalLock;
	std::jthread m_clickListener, m_clicker;
	unsigned m_timesToClick;
	unsigned m_sleepBetweenClicks;
	std::optional<std::pair<unsigned, unsigned>> m_position;
	void ClickListener();
	void RunAutoClickThread();
public:
	Autoclick(const AutoclickSetup& setup) : m_clicks(0), m_timesToClick(setup.howManyClicks), m_sleepBetweenClicks(setup.sleepBetweenClicks)
	{

	}

	Autoclick(const Autoclick&) = delete;
	Autoclick() = default;
	Autoclick& operator=(const Autoclick&) = delete;

	void RunAutoclicker();
};

