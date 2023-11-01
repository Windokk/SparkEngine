#include "btBulletDynamicsCommon.h"

class BulletMain {
public :
	BulletMain();
	void Update();
	void Unload();
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	
};
