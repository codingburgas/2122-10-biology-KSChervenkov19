#pragma once
#include "sceneManager.h"
#include "simulation_data.h"
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>

namespace ss::pl::graph
{
	class Graph : public Scene
	{
	public:
		Graph(std::string sceneName, SceneManager& sceneManager);

		inline static std::string fileName;

		virtual void Start();

		virtual void Update();

		virtual void onExit();

		SceneManager& m_sceneManager;

	private:
		std::vector <ss::types::Cycle> cycleInfo;
	};
}