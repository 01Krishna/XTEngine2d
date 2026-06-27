#pragma once

namespace XTEngine2d
{
	class Animation
	{
	public:
		float m_Fps = 0;
		bool m_RowOrColumnAnchor = true; // true = row, false = column
		int m_StartFrame = 0;
		int m_EndFrame = 0;

		int m_StartingRow = 0;
		int m_StartingColumn = 0;

		int m_CurrentFrame = 0;

		float m_Timer = 0.f;

		bool m_Looping = false;
		bool m_Playing = false;
	};
}
