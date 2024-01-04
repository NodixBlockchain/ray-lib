#include	"../libC/libc_math.h"
#include	"../libC/cpu_infos.h"

#include	"../libray/vec_types.h"
#include	"../libray/ray_int.h"
#include	"../libray/ray_float.h"
#include	"../libray/mat3.h"
#include	"../libray/geom_f.h"

#include	<btBulletDynamicsCommon.h>		// Header File For Windows

#include	"physics.h"






#define NUM_LEGS 5
#define BODYPART_COUNT 2 * NUM_LEGS + 1
#define JOINT_COUNT BODYPART_COUNT - 1

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif

#ifndef M_PI_8
#define M_PI_8     0.5 * M_PI_4
#endif

btCollisionShape*	m_shapes[BODYPART_COUNT];
btRigidBody*		m_bodies[BODYPART_COUNT];
btTypedConstraint*	m_joints[JOINT_COUNT];


btScalar	CPhysics::getDeltaTimeMicroseconds()
{
	btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
	m_clock.reset();
	return dt;
}

void CPhysics::add_cube(cubef_t *cube,btScalar	mass)
{
	btTransform				cubeTransform;
	btRigidBody				*body;
	btDefaultMotionState	*myMotionState;
	bool					isDynamic	= (mass != 0.f);
	btVector3				localInertia(0,0,0);
	btCollisionShape* cubeShape			= new btBoxShape(btVector3(cube->size[0],cube->size[1],cube->size[2]));
	
	//m_collisionShapes.push_back(cubeShape);

	cubeTransform.setIdentity();
	cubeTransform.setOrigin(btVector3(cube->center[0],cube->center[1],cube->center[2]));
	
	if (isDynamic)
		cubeShape->calculateLocalInertia(mass,localInertia);

	myMotionState = new btDefaultMotionState(cubeTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,cubeShape,localInertia);

	body = new btRigidBody(rbInfo);

	body->setFriction(2.0f);
	
	DynamicsWorld->addRigidBody(body);
	

	phys_cubes[n_phys_cubes].rbody	=body;
	phys_cubes[n_phys_cubes].cube	=cube;
	n_phys_cubes++;
}


void CPhysics::add_sphere(spheref_t *sphere,btScalar	mass)
{
	btTransform				sphereTransform;
	btRigidBody				*body;
	btDefaultMotionState	*myMotionState;
	bool					isDynamic	= (mass != 0.f);
	btVector3				localInertia(0,0,0);
	btCollisionShape* sphereShape			= new btSphereShape(sphere->radius);
	
	//m_collisionShapes.push_back(cubeShape);

	sphereTransform.setIdentity();
	sphereTransform.setOrigin(btVector3(sphere->center[0],sphere->center[1],sphere->center[2]));
	
	if (isDynamic)
		sphereShape->calculateLocalInertia(mass,localInertia);

	myMotionState = new btDefaultMotionState(sphereTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,sphereShape,localInertia);
	
	body = new btRigidBody(rbInfo);
	//body->setFriction(0.5f);
	body->setDamping(0.1,0.1);
	
	
	DynamicsWorld->addRigidBody(body);

	phys_sphere[n_phys_spheres].rbody	=body;
	phys_sphere[n_phys_spheres].sphere	=sphere;
	n_phys_spheres++;
}


void CPhysics::add_cylinder(cylf_t *cyl,btScalar	mass)
{
	btTransform				cylTransform;
	btRigidBody				*body;
	btDefaultMotionState	*myMotionState;
	bool					isDynamic	= (mass != 0.f);
	btVector3				localInertia(0,0,0);
	btCollisionShape* cylShape			= new btCylinderShape(btVector3(cyl->radius,cyl->half_height,cyl->radius));
	
	//m_collisionShapes.push_back(cubeShape);

	cylTransform.setIdentity	();
	cylTransform.setOrigin		(btVector3(cyl->center[0],cyl->center[1]+cyl->half_height,cyl->center[2]));
	
	if (isDynamic)
		cylShape->calculateLocalInertia(mass,localInertia);

	myMotionState = new btDefaultMotionState(cylTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,cylShape,localInertia);
	
	body = new btRigidBody(rbInfo);

	body->setDamping(0.1,0.1);

	DynamicsWorld->addRigidBody(body);

	phys_cyl[n_phys_cyls].rbody	=body;
	phys_cyl[n_phys_cyls].cyl	=cyl;
	n_phys_cyls++;
}


btRigidBody* CPhysics::localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	DynamicsWorld->addRigidBody(body);

	return body;
}



btRigidBody			* CPhysics::add_bones(const btVector3& positionOffset,CScene *Scene)
{
	btVector3 vUp(0, 1, 0);
	spheref_t	*sphere_ptr;
	cylf_t		*cyl_ptr;

	//
	// Setup geometry
	//

	float fBodySize  = 50.0f;
	float fLegLength = 60.0f;
	float fForeLegLength = 50.5f;
	
	m_shapes[0] = new btSphereShape(btScalar(fBodySize));

	
	int i;
	for ( i=0; i<NUM_LEGS; i++)
	{
		m_shapes[1 + 2*i] = new btCylinderShape(btVector3(10.0 ,btScalar(fLegLength),10.0));
		m_shapes[2 + 2*i] = new btCylinderShape(btVector3(8.0, btScalar(fForeLegLength),8.0));

	}

		//
		// Setup rigid bodies
		//
		float fHeight = 100.0;
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);		

		// root
		btVector3 vRoot = btVector3(btScalar(0.), btScalar(fHeight), btScalar(0.));
		btTransform transform;
		btVector3	origin;
		transform.setIdentity();
		transform.setOrigin(vRoot);
		
		
		m_bodies[0] =	localCreateRigidBody(btScalar(100.), offset*transform, m_shapes[0]);
		

		origin=m_bodies[0]->getWorldTransform().getOrigin();
		
		sphere_ptr=Scene->newSpheref	(origin[0],origin[1],origin[2],fBodySize);
		Scene->setSphereMaterialf		(sphere_ptr,0,0,0.2,0.8);
		Scene->setSphereNormalMap		(sphere_ptr,9,3);
		Scene->add_boxf					(&sphere_ptr->AABOX);
		
		phys_sphere[n_phys_spheres].rbody	=m_bodies[0];
		phys_sphere[n_phys_spheres].sphere	=sphere_ptr;
		n_phys_spheres++;

		// legs
		for ( i=0; i<NUM_LEGS; i++)
		{
			float fAngle = 2 * M_PI * i / NUM_LEGS;
			float fSin = sin(fAngle);
			float fCos = cos(fAngle);

			transform.setIdentity();
			btVector3 vBoneOrigin = btVector3(btScalar(fCos*(fBodySize*1.2+fLegLength)), btScalar(fHeight), btScalar(fSin*(fBodySize*1.2+fLegLength)));
			transform.setOrigin(vBoneOrigin);

			// thigh
			btVector3 vToBone	= (vBoneOrigin - vRoot).normalize();
			btVector3 vAxis		= vToBone.cross(vUp);			
			transform.setRotation(btQuaternion(vAxis, M_PI_2));
			m_bodies[1+2*i]		= localCreateRigidBody(btScalar(100), offset*transform, m_shapes[1+2*i]);

			origin=m_bodies[1+2*i]->getWorldTransform().getOrigin();
			cyl_ptr=Scene->newCylinderf	(origin[0],origin[1],origin[2],fLegLength,10.0);

			if(i==1)
				Scene->setCylMaterialf(cyl_ptr,0,0,1.0,0.0);
			else
				Scene->setCylMaterialf(cyl_ptr,8,3,0.0,0.0);
			
			Scene->add_boxf					(&cyl_ptr->AABOX);
			
			phys_cyl[n_phys_cyls].rbody	=m_bodies[1+2*i];
			phys_cyl[n_phys_cyls].cyl	=cyl_ptr;
			n_phys_cyls++;

			// shin
			transform.setIdentity();
			transform.setOrigin(btVector3(btScalar(fCos*(fBodySize*1.2+2.0*fLegLength)), fHeight-fForeLegLength, btScalar(fSin*(fBodySize*1.2+2.0*fLegLength))));
			m_bodies[2+2*i] = localCreateRigidBody(btScalar(100), offset*transform, m_shapes[2+2*i]);

			origin=m_bodies[2+2*i]->getWorldTransform().getOrigin();
			cyl_ptr=Scene->newCylinderf	(origin[0],origin[1],origin[2],fForeLegLength,10.0);
			Scene->add_boxf					(&cyl_ptr->AABOX);

			if(i==1)
				Scene->setCylMaterialf(cyl_ptr,0,0,1.0,0.0);
			else
				Scene->setCylMaterialf(cyl_ptr,8,3,0.0,0.0);
			
			
			
			phys_cyl[n_phys_cyls].rbody	=m_bodies[2+2*i];
			phys_cyl[n_phys_cyls].cyl	=cyl_ptr;
			n_phys_cyls++;
		}
		/*
		// Setup some damping on the m_bodies
		for (i = 0; i < BODYPART_COUNT; ++i)
		{
			m_bodies[i]->setDamping(0.05, 0.85);
			m_bodies[i]->setDeactivationTime(0.8);
			//m_bodies[i]->setSleepingThresholds(1.6, 2.5);
			m_bodies[i]->setSleepingThresholds(0.5f, 0.5f);
		}
		*/
		
		//
		// Setup the constraints
		//
		btHingeConstraint* hingeC;
		//btConeTwistConstraint* coneC;

		btTransform localA, localB, localC;

		for ( i=0; i<NUM_LEGS; i++)
		{
			float fAngle = 2 * M_PI * i / NUM_LEGS;
			float fSin = sin(fAngle);
			float fCos = cos(fAngle);

			// hip joints
			localA.setIdentity(); localB.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(btScalar(fCos*fBodySize*1.2), btScalar(0.), btScalar(fSin*fBodySize*1.2)));
			localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
			hingeC = new btHingeConstraint(*m_bodies[0], *m_bodies[1+2*i], localA, localB);
			hingeC->setLimit(btScalar(-0.75 * M_PI_4), btScalar(M_PI_8));
			//hingeC->setLimit(btScalar(-0.1), btScalar(0.1));
			m_joints[2*i] = hingeC;
			DynamicsWorld->addConstraint(m_joints[2*i], true);

			// knee joints
			localA.setIdentity(); localB.setIdentity(); localC.setIdentity();
			localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(btScalar(fCos*(fBodySize*1.2+2.0*fLegLength)), btScalar(0.), btScalar(fSin*(fBodySize*1.2+2.0*fLegLength))));
			localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
			localC = m_bodies[2+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
			hingeC = new btHingeConstraint(*m_bodies[1+2*i], *m_bodies[2+2*i], localB, localC);
			//hingeC->setLimit(btScalar(-0.01), btScalar(0.01));
			hingeC->setLimit(btScalar(-M_PI_4), btScalar(0.0));
			m_joints[1+2*i] = hingeC;
			DynamicsWorld->addConstraint(m_joints[1+2*i], true);
		}


		return m_bodies[0];
}



void CPhysics::initPhysic()
{
	m_collisionConfiguration	= new btDefaultCollisionConfiguration();
	m_dispatcher				= new btCollisionDispatcher(m_collisionConfiguration);

	btVector3 worldAabbMin		(-10000,-10000,-10000);
	btVector3 worldAabbMax		( 10000, 10000, 10000);
	m_broadphase				= new btAxisSweep3			(worldAabbMin, worldAabbMax);
	m_solver					= new btSequentialImpulseConstraintSolver();
	DynamicsWorld				= new btDiscreteDynamicsWorld	(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	DynamicsWorld->setGravity	(m_gravity);
	

	n_phys_cubes	=	0;
	n_phys_spheres	=	0;
	
}
bool CPhysics::getBonePos(vec3f_t origin)
{
	btVector3 pos;

	if(m_bodies[0]==NULL)return false;

	pos=m_bodies[0]->getWorldTransform().getOrigin();

	origin[0]=pos.x();
	origin[1]=pos.y();
	origin[2]=pos.z();

	return true;

}
void CPhysics::syncScene(CScene *Scene)
{
	int i;
	float deltaTime = getDeltaTimeMicroseconds()/1000000.f;
	btVector3 pos;


	DynamicsWorld->stepSimulation(deltaTime);

	i=0;
	while(i<n_phys_cubes)
	{
		btMatrix3x3 btMat3;
		//CMat3 <float,cmat_funcs> Mat3;
		mat3x3f_t m,glMat;
		//pos=DynamicsWorld->getCollisionObjectArray()[1]->getInterpolationWorldTransform().getOrigin();

		if(phys_cubes[i].cube!=NULL)
		{
			pos		=phys_cubes[i].rbody->getWorldTransform().getOrigin();
			btMat3	=phys_cubes[i].rbody->getWorldTransform().getBasis();

			btMat3.getOpenGLSubMatrix(glMat);

			m[0]=glMat[0];
			m[1]=glMat[1];
			m[2]=glMat[2];
			
			m[4]=glMat[4];
			m[5]=glMat[5];
			m[6]=glMat[6];

			m[8]=glMat[8];
			m[9]=glMat[9];
			m[10]=glMat[10];

			//identity_mat3x3_c				(m);


			Scene->setCubeCenter_matf	(phys_cubes[i].cube	,pos[0],pos[1],pos[2],m);
		}

		i++;
	}

	i=0;
	while(i<n_phys_spheres)
	{
		btMatrix3x3 btMat3;
		mat3x3f_t m,glMat;

		if(phys_sphere[i].sphere!=NULL)
		{
			pos		=phys_sphere[i].rbody->getWorldTransform().getOrigin();
			btMat3	=phys_sphere[i].rbody->getWorldTransform().getBasis();

			btMat3.getOpenGLSubMatrix(glMat);

			m[0]=glMat[0];
			m[1]=glMat[1];
			m[2]=glMat[2];
			
			m[4]=glMat[4];
			m[5]=glMat[5];
			m[6]=glMat[6];

			m[8]=glMat[8];
			m[9]=glMat[9];
			m[10]=glMat[10];

			Scene->setSphereCenter_matf		(phys_sphere[i].sphere	,pos[0],pos[1],pos[2],m);
		}

		i++;
	}

	i=0;
	while(i<n_phys_cyls)
	{
		btMatrix3x3 btMat3;
		mat3x3f_t m,glMat;

		if(phys_cyl[i].cyl!=NULL)
		{
			pos		=phys_cyl[i].rbody->getWorldTransform().getOrigin();
			btMat3	=phys_cyl[i].rbody->getWorldTransform().getBasis();

			btMat3.getOpenGLSubMatrix(glMat);

			m[0]=glMat[0];
			m[1]=glMat[1];
			m[2]=glMat[2];
			
			m[4]=glMat[4];
			m[5]=glMat[5];
			m[6]=glMat[6];

			m[8]=glMat[8];
			m[9]=glMat[9];
			m[10]=glMat[10];

			Scene->setCylCenter_matf		(phys_cyl[i].cyl	,pos[0],pos[1],pos[2],m);
		}

		i++;
	}
}