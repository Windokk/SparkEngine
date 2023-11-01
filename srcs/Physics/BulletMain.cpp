#include "BulletMain.h"

BulletMain::BulletMain()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
}

void BulletMain::Update()
{
}

void BulletMain::Unload()
{
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}
