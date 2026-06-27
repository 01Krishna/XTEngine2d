#pragma once
#include "Utility.h"

namespace XTEngine2d
{
	struct AnimationState
	{
		bool looping;
		float value;
		int startingFrame;
	};

	class AnimationStateMachine
	{
	public:
		void AddState(const std::string stateName, float value, bool looping, int startingframe)
		{
			if (m_States.find(stateName) != m_States.end())
				return;

			m_States[stateName] = AnimationState{ looping,value, startingframe };
			m_TotalStates++;
		}

		std::pair<std::string, AnimationState> GetState(const std::string stateName)
		{
			if (m_States.find(stateName) == m_States.end())
				return std::make_pair("No State Found", AnimationState{ 0, 0.f, false });

			return std::make_pair(stateName, m_States[stateName]);
		}

		void UpdateState(const std::string stateName, int startingframe, float value, bool looping)
		{
			if (m_States.find(stateName) == m_States.end())
				return;

			m_States[stateName] = AnimationState{ looping, value, startingframe };
		}

		void RemoveState(const std::string stateName)
		{
			if (m_States.find(stateName) == m_States.end())
				return;
			m_States.erase(stateName);
			m_TotalStates--;
		}

		void ClearStates()
		{
			m_States.clear();
		}

		std::unordered_map<std::string, AnimationState>& GetAllStates()
		{
			return m_States;
		}


	public:
		std::unordered_map<std::string, AnimationState> m_States;
		std::pair<std::string, AnimationState> m_CurrentState = std::make_pair("No State", AnimationState{ 0, 0.f, false });
		int m_TotalStates = 0;
	};
}