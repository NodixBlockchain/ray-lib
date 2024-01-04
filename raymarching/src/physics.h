
typedef struct
{
	btRigidBody		*rbody;
	cubef_t			*cube;
}phys_cube_t;

typedef struct
{
	btRigidBody		*rbody;
	spheref_t		*sphere;
}phys_sphere_t;


typedef struct
{
	btRigidBody		*rbody;
	cylf_t			*cyl;
}phys_cyl_t;

class CPhysics
{

public :
CPhysics()
{
	m_gravity[0]=0;
	m_gravity[1]=-400.0;
	m_gravity[2]=0;
}

void add_cube(cubef_t *cube,btScalar	mass);
void add_sphere(spheref_t *sphere,btScalar	mass);
void add_cylinder(cylf_t *cyl,btScalar	mass);
btRigidBody *localCreateRigidBody	(btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
btRigidBody	*add_bones				(const btVector3& positionOffset,CScene *scene);
void initPhysic();
void syncScene(CScene *Scene);
bool getBonePos(vec3f_t origin);

private:

btScalar						getDeltaTimeMicroseconds();
btClock							m_clock;
btDynamicsWorld					*DynamicsWorld;
btDefaultCollisionConfiguration	*m_collisionConfiguration;
btBroadphaseInterface			*m_broadphase;
btCollisionDispatcher			*m_dispatcher;
btConstraintSolver				*m_solver;
btVector3						m_gravity;

phys_cube_t						phys_cubes[32];
unsigned int					n_phys_cubes;

phys_sphere_t					phys_sphere[32];
unsigned int					n_phys_spheres;

phys_cyl_t						phys_cyl[32];
unsigned int					n_phys_cyls;



};