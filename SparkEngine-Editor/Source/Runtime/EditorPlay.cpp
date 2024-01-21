#include "EditorPlay.h"

EditorPlayer::EditorPlayer() {

}

void EditorPlayer::Prepare(LevelLoader* loader)
{
	for (auto& obj : loader->parser.objects) {
		if (obj.HasComponent<RigidbodyComponent>()) {
			m_world.AddObject(&loader->rigidbodies[obj.name]);
		}
	}
	isPlaying = true;
	EditorPlayer::CompileScripts();
}

void EditorPlayer::Play(){

	
	//////////////////////////////
	//////////////////////////////
	////					  ////
	////   Default (Editor)   ////
	////					  ////
	//////////////////////////////
	//////////////////////////////
	
	/// Physics

	m_world.Step(0.005);

	

	//////////////////////////////
	//////////////////////////////
	////					  ////
	////         User         ////
	////					  ////
	//////////////////////////////
	//////////////////////////////


	

}

void EditorPlayer::Stop(LevelLoader* loader) {

	//////////////////////////////
	//////////////////////////////
	////					  ////
	////   Default (Editor)   ////
	////					  ////
	//////////////////////////////
	//////////////////////////////

	for (int i = 0; i < loader->parser.objects.size(); i++) {
		if (loader->parser.objects[i].HasComponent<RigidbodyComponent>()) {
			loader->parser.objects[i].GetComponent<TransformComponent>() = loader->objects_Transforms[i].toTransformComponent();
		}
	}

	/// Physics

	m_world.RemoveAllObjects();


	//////////////////////////////
	//////////////////////////////
	////					  ////
	////         User         ////
	////					  ////
	//////////////////////////////
	//////////////////////////////


	isPlaying = false;

}

void EditorPlayer::StopScripts(){

}

void EditorPlayer::CompileScripts(){

}

void EditorPlayer::RunScripts(){

}