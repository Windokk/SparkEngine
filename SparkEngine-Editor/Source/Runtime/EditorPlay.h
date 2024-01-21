#include "Physics/Dynamics.h"
#include <Level Management/LevelLoader.h>
#include <chrono>
#include <thread>

using namespace physics;

class EditorPlayer {
public:
	EditorPlayer();

	PhysicsWorld m_world;

	bool isCompiled;

	bool isPlaying;

	void Play();
	void Stop(LevelLoader* loader);
	void Prepare(LevelLoader* loader);
	void CompileScripts();
	void RunScripts();
	void StopScripts();
};