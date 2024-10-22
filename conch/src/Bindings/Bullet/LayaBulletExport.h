#ifndef __LayaBulletExport_H__
#define __LayaBulletExport_H__

#include <JCConch.h>
#include "../../JCScriptRuntime.h"
#include <binder/JSInterface.h>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include <Bindings/JSRuntime.h>
#define  WASM_EXP JSLayaConchBullet::
#define __BTWASM_SYSCALL_NAME(name)
typedef intptr_t pointer_t;
namespace laya
{
    inline void layaMotionStateGetWorldTransform(int32_t rigidBodyID, pointer_t worldTrans)
    {
		JCConch::s_pScriptRuntime->m_bJSBulletGetWorldTransformHandle.call<void>(getCurrentContext().global(),rigidBodyID, worldTrans);
    }
    inline void layaMotionStateSetWorldTransform(int32_t rigidBodyID, const pointer_t worldTrans)
    {
		JCConch::s_pScriptRuntime->m_bJSBulletSetWorldTransformHandle.call<void>(getCurrentContext().global(),rigidBodyID, worldTrans);
    }


    class LayaMotionState : public btMotionState
    {
    public:
		int32_t rigidBodyID;
        virtual void getWorldTransform(btTransform &centerOfMassWorldTrans)
        {
            layaMotionStateGetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
        }
        virtual void setWorldTransform(const btTransform &centerOfMassWorldTrans)
        {
            layaMotionStateSetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
        }
    };
    class JSLayaConchBullet
    {
    public:
        static void exportJS(jsbind::Object& context);

        static void  btGImpactCollisionAlgorithm_RegisterAlgorithm(pointer_t dispatcher);
        static pointer_t  btVector3_create(btScalar x, btScalar y, btScalar z);
        static void  btVector3_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z);
        static btScalar  btVector3_x(pointer_t ptr);
        static btScalar  btVector3_y(pointer_t ptr);
        static btScalar  btVector3_z(pointer_t ptr);
        static pointer_t  btQuaternion_create(btScalar x, btScalar y, btScalar z, btScalar w);
        static void  btQuaternion_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z, btScalar w);
        static btScalar  btQuaternion_x(pointer_t ptr);
        static btScalar  btQuaternion_y(pointer_t ptr);
        static btScalar  btQuaternion_z(pointer_t ptr);
        static btScalar  btQuaternion_w(pointer_t ptr);
        static pointer_t  btTransform_create();
        static void  btTransform_setOrigin(pointer_t ptr, pointer_t origin);
        static void  btTransform_setRotation(pointer_t ptr, pointer_t q);
        static pointer_t  btTransform_getOrigin(pointer_t ptr);
        static pointer_t  btTransform_getRotation(pointer_t ptr);
        static void btTransform_setIdentity(pointer_t ptr);
		static pointer_t btTransform_getBasis(pointer_t ptr);
		static pointer_t btMatrix3x3_getRow(pointer_t ptr, int row);
        static void  btMotionState_destroy(pointer_t ptr);
        static pointer_t  layaMotionState_create();
        static void  layaMotionState_set_rigidBodyID(pointer_t ptr, int rigidBodyID);
        static pointer_t  btCollisionObject_create();
        static void  btCollisionObject_setContactProcessingThreshold(pointer_t ptr, btScalar contactProcessingThreshold);
        static void  btCollisionObject_setActivationState(pointer_t ptr, int newState);
        static void  btCollisionObject_forceActivationState(pointer_t ptr, int newState);
        static void  btCollisionObject_activate(pointer_t ptr, bool forceActivation);
        static bool  btCollisionObject_isActive(pointer_t ptr);
        static void  btCollisionObject_setRestitution(pointer_t ptr, btScalar rest);
        static void  btCollisionObject_setFriction(pointer_t ptr, btScalar frict);
        static void  btCollisionObject_setRollingFriction(pointer_t ptr, btScalar frict);
        static int  btCollisionObject_getCollisionFlags(pointer_t ptr);
        static void  btCollisionObject_setCollisionFlags(pointer_t ptr, int flags);
        static pointer_t  btCollisionObject_getWorldTransform(pointer_t ptr);
        static void  btCollisionObject_setCollisionShape(pointer_t ptr, pointer_t collisionShape);
        static btScalar  btCollisionObject_getCcdMotionThreshold(pointer_t ptr);
        static void  btCollisionObject_setCcdMotionThreshold(pointer_t ptr, btScalar ccdMotionThreshold);
        static btScalar  btCollisionObject_getCcdSweptSphereRadius(pointer_t ptr);
        static void  btCollisionObject_setCcdSweptSphereRadius(pointer_t ptr, btScalar radius);
        static int  btCollisionObject_getUserIndex(pointer_t ptr);
        static void  btCollisionObject_setUserIndex(pointer_t ptr, int index);
        static int  btCollisionObject_getActivationState(pointer_t ptr);
        static void  btCollisionObject_setInterpolationAngularVelocity(pointer_t ptr, pointer_t angvel);
        static void  btCollisionObject_setInterpolationLinearVelocity(pointer_t ptr, pointer_t linvel);
        static void  btCollisionObject_destroy(pointer_t ptr);
		static void RayResultCallback_set_m_flags(pointer_t ptr, int flags);
        static bool  RayResultCallback_hasHit(pointer_t ptr);
        static void  RayResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group);
        static void  RayResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask);
		//void  RayResultCallback_set_m_ignoreTrigger(pointer_t ptr, bool ignoreTrigger);
        static btScalar  RayResultCallback_get_m_closestHitFraction(pointer_t ptr);
        static void  RayResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction);
        static pointer_t  RayResultCallback_get_m_collisionObject(pointer_t ptr);
        static void  RayResultCallback_set_m_collisionObject(pointer_t ptr, pointer_t collisionObject);
        static pointer_t  ClosestRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld);
        static pointer_t  ClosestRayResultCallback_get_m_rayFromWorld(pointer_t ptr);
        static void  ClosestRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld);
        static pointer_t  ClosestRayResultCallback_get_m_rayToWorld(pointer_t ptr);
        static void  ClosestRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld);
        static pointer_t  ClosestRayResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        static pointer_t  ClosestRayResultCallback_get_m_hitPointWorld(pointer_t ptr);
        static int  tBtCollisionObjectArray_size(pointer_t ptr);
        static pointer_t  tBtCollisionObjectArray_at(pointer_t ptr, int n);
        static void  tBtCollisionObjectArray_clear(pointer_t ptr);
        static  pointer_t  tVector3Array_at(pointer_t ptr, int n);
        static void  tVector3Array_clear(pointer_t ptr);
        static btScalar  tScalarArray_at(pointer_t ptr, int n);
        static void  tScalarArray_clear(pointer_t ptr);
        static pointer_t  AllHitsRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld);
        static pointer_t  AllHitsRayResultCallback_get_m_rayFromWorld(pointer_t ptr);
        static void  AllHitsRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld);
        static pointer_t  AllHitsRayResultCallback_get_m_rayToWorld(pointer_t ptr);
        static void  AllHitsRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld);
        static pointer_t  AllHitsRayResultCallback_get_m_hitPointWorld(pointer_t ptr);
        static pointer_t  AllHitsRayResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        static pointer_t  AllHitsRayResultCallback_get_m_collisionObjects(pointer_t ptr);
        static pointer_t  AllHitsRayResultCallback_get_m_hitFractions(pointer_t ptr);
        static pointer_t  btManifoldPoint_get_m_positionWorldOnA(pointer_t ptr);
        static pointer_t  btManifoldPoint_get_m_positionWorldOnB(pointer_t ptr);
        static pointer_t  btManifoldPoint_get_m_normalWorldOnB(pointer_t ptr);
        static btScalar  btManifoldPoint_getDistance(pointer_t ptr);
        static bool  ConvexResultCallback_hasHit(pointer_t ptr);
        static void  ConvexResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group);
        static void  ConvexResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask);
        static btScalar  ConvexResultCallback_get_m_closestHitFraction(pointer_t ptr);
        static void  ConvexResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction);
        static pointer_t  ClosestConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld);
        static pointer_t  ClosestConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        static pointer_t  ClosestConvexResultCallback_get_m_hitPointWorld(pointer_t ptr);
        static pointer_t  ClosestConvexResultCallback_get_m_hitCollisionObject(pointer_t ptr);
        static void  ClosestConvexResultCallback_set_m_hitCollisionObject(pointer_t ptr, pointer_t hitCollisionObject);
        static pointer_t  AllConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld);
        static pointer_t  AllConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        static pointer_t  AllConvexResultCallback_get_m_hitPointWorld(pointer_t ptr);
        static pointer_t  AllConvexResultCallback_get_m_hitFractions(pointer_t ptr);
        static pointer_t  AllConvexResultCallback_get_m_collisionObjects(pointer_t ptr);

        static int btCollisionShape_getShapeType(pointer_t ptr);
        static pointer_t  btCollisionShape_getLocalScaling(pointer_t ptr);
        static void  btCollisionShape_setLocalScaling(pointer_t ptr, pointer_t scaling);
        static void  btCollisionShape_calculateLocalInertia(pointer_t ptr, btScalar mass, pointer_t inertia);
        static void  btCollisionShape_destroy(pointer_t ptr);
        static pointer_t  btBoxShape_create(pointer_t boxHalfExtents);
        static void  btBoxShape_getAabb(pointer_t ptr, pointer_t t, pointer_t aabbMin, pointer_t aabbMax);
        static pointer_t  btBoxShape_getHalfExtentsWithMargin(pointer_t ptr, pointer_t t, pointer_t aabbMin, pointer_t aabbMax);
        static pointer_t  btCapsuleShape_create(btScalar radius, btScalar height);
        static pointer_t  btCapsuleShapeX_create(btScalar radius, btScalar height);
        static pointer_t  btCapsuleShapeZ_create(btScalar radius, btScalar height);
        static pointer_t  btCylinderShape_create(pointer_t halfExtents);
        static pointer_t  btCylinderShapeX_create(pointer_t halfExtents);
        static pointer_t  btCylinderShapeZ_create(pointer_t halfExtents);
        static pointer_t  btSphereShape_create(btScalar radius);
        static pointer_t  btConeShape_create(btScalar radius, btScalar height);
        static pointer_t  btConeShapeX_create(btScalar radius, btScalar height);
        static pointer_t  btConeShapeZ_create(btScalar radius, btScalar height);
        static pointer_t  btStaticPlaneShape_create(pointer_t planeNormal, btScalar planeConstant);
        static void  btGImpactShapeInterface_updateBound(pointer_t ptr);
        static pointer_t  btGImpactMeshShape_create(pointer_t meshInterface);
        static pointer_t  btCompoundShape_create();
        static void  btCompoundShape_addChildShape(pointer_t ptr, pointer_t localTransform, pointer_t shape);
        static void  btCompoundShape_removeChildShapeByIndex(pointer_t ptr, int childShapeIndex);
        static pointer_t  btCompoundShape_getChildShape(pointer_t ptr, int index);
        static void  btCompoundShape_updateChildTransform(pointer_t ptr, int index, pointer_t newChildTransform, bool shouldRecalculateLocalAabb);
        static pointer_t btShapeHull_create(pointer_t ptr);
        static int btShapeHull_numVertices(pointer_t ptr);
        static int btShapeHull_numIndices(pointer_t ptr);
        static pointer_t btShapeHull_getVertexPointer(pointer_t ptr, int index);
        static int btShapeHull_getIndexPointer(pointer_t ptr, int index);
        static pointer_t btConvexHullShape_create(pointer_t ptr);
        static void btConvexHullShape_addPoint(pointer_t ptr, pointer_t vertex1, bool recalculateLocalAabb);
        static int btConvexHullShape_getNumEdges(pointer_t ptr);
        static pointer_t btConvexHullShape_getVertex(pointer_t ptr, int i);
        static void  btStridingMeshInterface_destroy(pointer_t ptr);
        static pointer_t  btTriangleMesh_create();
        static void  btTriangleMesh_addTriangle(pointer_t ptr, pointer_t vertex1, pointer_t vertex2, pointer_t vertex3, bool removeDuplicateVertices);
        static pointer_t  btDefaultCollisionConfiguration_create();
        static void  btDefaultCollisionConfiguration_destroy(pointer_t ptr);
        static pointer_t  btPersistentManifold_getBody0(pointer_t ptr);
        static pointer_t  btPersistentManifold_getBody1(pointer_t ptr);
        static int  btPersistentManifold_getNumContacts(pointer_t ptr);
        static pointer_t  btPersistentManifold_getContactPoint(pointer_t ptr, int index);
        static int  btDispatcher_getNumManifolds(pointer_t ptr);
        static pointer_t  btDispatcher_getManifoldByIndexInternal(pointer_t ptr, int index);
        static pointer_t  btCollisionDispatcher_create(pointer_t collisionConfiguration);
        static void  btCollisionDispatcher_destroy(pointer_t ptr);
        static void  btOverlappingPairCache_setInternalGhostPairCallback(pointer_t ptr, pointer_t ghostPairCallback);
        static pointer_t  btDbvtBroadphase_create();
        static pointer_t  btDbvtBroadphase_getOverlappingPairCache(pointer_t ptr);
        static void  btDbvtBroadphase_destroy(pointer_t ptr);
        static pointer_t  btRigidBodyConstructionInfo_create(btScalar mass, pointer_t motionState, pointer_t collisionShape, pointer_t localInertia);
        static void  btRigidBodyConstructionInfo_destroy(pointer_t ptr);
        static pointer_t  btRigidBody_create(pointer_t constructionInfo);
        static void  btRigidBody_setCenterOfMassTransform(pointer_t ptr, pointer_t xform);
        static void  btRigidBody_setSleepingThresholds(pointer_t ptr, btScalar linear, btScalar angular);
        static btScalar  btRigidBody_getLinearSleepingThreshold(pointer_t ptr);
        static btScalar  btRigidBody_getAngularSleepingThreshold(pointer_t ptr);
        static void  btRigidBody_setDamping(pointer_t ptr, btScalar lin_damping, btScalar ang_damping);
        static void  btRigidBody_setMassProps(pointer_t ptr, btScalar mass, pointer_t inertia);
        static void  btRigidBody_setLinearFactor(pointer_t ptr, pointer_t linearFactor);
        static void  btRigidBody_applyTorque(pointer_t ptr, pointer_t torque);
        static void  btRigidBody_applyForce(pointer_t ptr, pointer_t force, pointer_t rel_pos);
        static void  btRigidBody_applyCentralForce(pointer_t ptr, pointer_t force);
        static void  btRigidBody_applyTorqueImpulse(pointer_t ptr, pointer_t torque);
        static void  btRigidBody_applyImpulse(pointer_t ptr, pointer_t impulse, pointer_t rel_pos);
        static void  btRigidBody_applyCentralImpulse(pointer_t ptr, pointer_t impulse);
        static void  btRigidBody_updateInertiaTensor(pointer_t ptr);
        static pointer_t  btRigidBody_getLinearVelocity(pointer_t ptr);
        static pointer_t  btRigidBody_getAngularVelocity(pointer_t ptr);
        static void  btRigidBody_setLinearVelocity(pointer_t ptr, pointer_t lin_vel);
        static void  btRigidBody_setAngularVelocity(pointer_t ptr, pointer_t ang_vel);
        static void  btRigidBody_setAngularFactor(pointer_t ptr, pointer_t angularFactor);
        static pointer_t  btRigidBody_getGravity(pointer_t ptr);
        static void  btRigidBody_setGravity(pointer_t ptr, pointer_t acceleration);
        static void  btKinematicCharacterController_setUp(pointer_t ptr, pointer_t up);
        static void  btKinematicCharacterController_setStepHeight(pointer_t ptr, btScalar h);
        static void  btKinematicCharacterController_setMaxPenetrationDepth(pointer_t ptr, btScalar d);
        static btScalar btKinematicCharacterController_getMaxPenetrationDepth(pointer_t ptr);
        static void  btCollisionObject_setInterpolationWorldTransform(pointer_t ptr, pointer_t worldTrans);
        static void  btCollisionObject_setWorldTransform(pointer_t ptr, pointer_t worldTrans);
        static pointer_t  btRigidBody_getTotalForce(pointer_t ptr);
        static pointer_t  btRigidBody_getTotalTorque(pointer_t ptr);
        static int  btRigidBody_getFlags(pointer_t ptr);
        static void  btRigidBody_setFlags(pointer_t ptr, int flags);
        static void  btRigidBody_clearForces(pointer_t ptr);
        static pointer_t  btSequentialImpulseConstraintSolver_create();
        static bool  btCollisionWorld_get_m_useContinuous(pointer_t ptr);
        static void  btCollisionWorld_set_m_useContinuous(pointer_t ptr, bool useContinuous);
        static void  btCollisionWorld_rayTest(pointer_t ptr, pointer_t rayFromWorld, pointer_t rayToWorld, pointer_t resultCallback);
        static pointer_t  btCollisionWorld_getDispatchInfo(pointer_t ptr);
        static void  btCollisionWorld_addCollisionObject(pointer_t ptr, pointer_t collisionObject, int collisionFilterGroup, int collisionFilterMask);
        static void  btCollisionWorld_removeCollisionObject(pointer_t ptr, pointer_t collisionObject);
        static void  btCollisionWorld_convexSweepTest(pointer_t ptr, pointer_t castShape, pointer_t from, pointer_t to, pointer_t resultCallback, float allowedCcdPenetration);
        static void  btCollisionWorld_destroy(pointer_t ptr);
        static void  btDynamicsWorld_addAction(pointer_t ptr, pointer_t action);
        static void  btDynamicsWorld_removeAction(pointer_t ptr, pointer_t action);
        static pointer_t  btDynamicsWorld_getSolverInfo(pointer_t ptr);
        static pointer_t  btDiscreteDynamicsWorld_create(pointer_t dispatcher, pointer_t pairCache, pointer_t constraintSolver, pointer_t collisionConfiguration);
        static void  btDiscreteDynamicsWorld_setGravity(pointer_t ptr, pointer_t gravity);
        static pointer_t  btDiscreteDynamicsWorld_getGravity(pointer_t ptr);
        static void  btDiscreteDynamicsWorld_addRigidBody(pointer_t ptr, pointer_t body, int group, int mask);
        static void  btDiscreteDynamicsWorld_removeRigidBody(pointer_t ptr, pointer_t body);
        static void  btDiscreteDynamicsWorld_stepSimulation(pointer_t ptr, btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep);
        static void  btDiscreteDynamicsWorld_clearForces(pointer_t ptr);
        static void  btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution(pointer_t ptr, bool enable);
        static bool  btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution(pointer_t ptr);
        static pointer_t  btKinematicCharacterController_create(pointer_t ghostObject, pointer_t convexShape, btScalar stepHeight, pointer_t up);
        static void  btKinematicCharacterController_setWalkDirection(pointer_t ptr, pointer_t walkDirection);
        static void  btKinematicCharacterController_setFallSpeed(pointer_t ptr, btScalar fallSpeed);
        static void  btKinematicCharacterController_setJumpSpeed(pointer_t ptr, btScalar jumpSpeed);
        static void  btKinematicCharacterController_setMaxSlope(pointer_t ptr, btScalar slopeRadians);
        static bool  btKinematicCharacterController_onGround(pointer_t ptr);
        static void  btKinematicCharacterController_jump(pointer_t ptr, pointer_t v);
        static void  btKinematicCharacterController_setGravity(pointer_t ptr, pointer_t gravity);
        static void  btKinematicCharacterController_destroy(pointer_t ptr);
        static pointer_t  btPairCachingGhostObject_create();
        static pointer_t  btGhostPairCallback_create();
        static void  btTransform_equal(pointer_t ptr, pointer_t other);

        static void btTypedConstraint_setEnabled(pointer_t constraintptr, bool enabled);
        static void btCollisionWorld_addConstraint(pointer_t ptr, pointer_t constraintptr, bool disableCollisionsBetweenLinkedBodies);
        static void btCollisionWorld_removeConstraint(pointer_t ptr, pointer_t constraintptr);
        static pointer_t btJointFeedback_create();
        static void btJointFeedback_destroy(pointer_t jointFeedbackptr);
        static void btTypedConstraint_setJointFeedback(pointer_t constraintptr, pointer_t jointFeedbackptr);
        static pointer_t btTypedConstraint_getJointFeedback(pointer_t constraintptr);
        static void btTypedConstraint_enableFeedback(pointer_t constraintptr, bool needsFeedback);
        static void btTypedConstraint_setParam(pointer_t constraintptr, int axis1, int constraintParams, btScalar value); 
        static void btTypedConstraint_setOverrideNumSolverIterations(pointer_t constraintptr, int overideNumIterations);
        static void btTypedConstraint_destroy(pointer_t constraintptr);
        static pointer_t btJointFeedback_getAppliedForceBodyA(pointer_t jointFeedbackptr);
        static pointer_t btJointFeedback_getAppliedForceBodyB(pointer_t jointFeedbackptr);
        static pointer_t btJointFeedback_getAppliedTorqueBodyA(pointer_t jointFeedbackptr);
        static pointer_t btJointFeedback_getAppliedTorqueBodyB(pointer_t jointFeedbackptr);
        static pointer_t btFixedConstraint_create(pointer_t rigidBodyA, pointer_t frameInAptr, pointer_t rigidBodyB, pointer_t frameInBptr);
        static  pointer_t btGeneric6DofSpring2Constraint_create(pointer_t rigidBodyAptr, pointer_t frameInAptr, pointer_t rigidBodyBptr, pointer_t frameInBptr, int rotOrder = 0);
        static void btGeneric6DofSpring2Constraint_setAxis(pointer_t g6ds2Constraintptr, pointer_t axis1, pointer_t axis2);
        static void btGeneric6DofSpring2Constraint_setLimit(pointer_t g6ds2Constraintptr, int axis, btScalar lo, btScalar hi);
        static void btGeneric6DofSpring2Constraint_enableSpring(pointer_t g6ds2Constraintptr, int index, bool enableSpring);
        static void btGeneric6DofSpring2Constraint_setBounce(pointer_t g6ds2Constraintptr, int index, btScalar bounce);
        static void btGeneric6DofSpring2Constraint_setStiffness(pointer_t g6ds2Constraintptr, int index, btScalar stiffness, bool limitIfNeeded = true);
        static void btGeneric6DofSpring2Constraint_setDamping(pointer_t g6ds2Constraintptr, int index, btScalar damping, bool limitIfNeeded = true);
        static void btGeneric6DofSpring2Constraint_setEquilibriumPoint(pointer_t g6ds2Constraintptr, int index, btScalar val);
        static void btGeneric6DofSpring2Constraint_enableMotor(pointer_t g6ds2Constraintptr, int index, bool onOff);
        static void btGeneric6DofSpring2Constraint_setServo(pointer_t g6ds2Constraintptr, int index, bool onOff);
        static void btGeneric6DofSpring2Constraint_setTargetVelocity(pointer_t g6ds2Constraintptr, int index, btScalar velocity);
        static void btGeneric6DofSpring2Constraint_setServoTarget(pointer_t g6ds2Constraintptr, int index, btScalar target);
        static void btGeneric6DofSpring2Constraint_setMaxMotorForce(pointer_t g6ds2Constraintptr, int index, btScalar force);
        static void btGeneric6DofSpring2Constraint_setFrames(pointer_t g6ds2Constraintptr, pointer_t frameAptr, pointer_t frameBptr);


		static void btQuaternion_delete(pointer_t ptr);
		static pointer_t btBvhTriangleMeshShape_create(pointer_t meshInterface);
		static void btRigidBody_setCenterOfMassPos(pointer_t ptr, btScalar x, btScalar y, btScalar z);
		static void btCollisionWorld_set_UserInfo(pointer_t worldptr, pointer_t worldid);
        static pointer_t btCollisionWorld_get_UserInfo(pointer_t worldptr);
		static void btDynamicsWorld_enableDebugDrawer(pointer_t ptr, int b);
		static void btKinematicCharacterController_setJumpAxis(pointer_t ptr, btScalar x, btScalar y, btScalar z);
		static int btKinematicCharacterController_getHitFlag(pointer_t ptr);
		static btScalar btKinematicCharacterController_getVerticalVelocity(pointer_t ptr);
		static pointer_t btKinematicCharacterController_getCurrentPosition(pointer_t ptr);
		static pointer_t btKinematicCharacterController_getCurrentOrientation(pointer_t ptr);

		static int layaMotionState_get_rigidBodyID(pointer_t ptr);
		static int btCollisionObject_getNumOverlappingObjects(pointer_t ptr);
		static pointer_t btCollisionObject_getOverlappingObject(pointer_t ptr, int i);
		static void btSphereShpae_setUnscaledRadius(pointer_t ptr, btScalar r);
		static pointer_t btRigidBody_getMotionState(pointer_t ptr);
		static void btDiscreteDynamicsWorld_SphereQuery(pointer_t ptr, btScalar posx, btScalar posy, btScalar posz, btScalar radius);
		static pointer_t btRaycastVehicle_create(pointer_t pWorld, pointer_t pRigid);
		static pointer_t btRaycastVehicle_addWheel(pointer_t ptr,
			btScalar connectionPointCSx, btScalar connectionPointCSy, btScalar connectionPointCSz,	// ����ռ����ӵ�
			btScalar wheelDirectionCS0x, btScalar wheelDirectionCS0y, btScalar wheelDirectionCS0z,	// ���ֳ���
			btScalar wheelAxleCSx, btScalar wheelAxleCSy, btScalar wheelAxleCSz, 					// ������
			btScalar suspensionRestLength, btScalar wheelRadius,
			//tuning
			btScalar suspensionStiffness,
			btScalar suspensionCompression,
			btScalar suspensionDamping,
			btScalar frictionSlip,
			btScalar maxSuspensionTravelCm,
			btScalar maxSuspensionForce,
			bool isFrontWheel
		);
		static int  btRaycastVehicle_getNumWheels(pointer_t ptr);
		static void  btRaycastVehicle_destroy(pointer_t ptr);
		static pointer_t  btRaycastVehicle_getWheelInfo(pointer_t ptr, int i);
		static void btWheelInfo_setEengineForce(pointer_t ptr, btScalar force);
		static btScalar btWheelInfo_getEengineForce(pointer_t ptr, btScalar force);
		static void btWheelInfo_setSteeringValue(pointer_t ptr, btScalar steering);
		static btScalar btWheelInfo_getSteeringValue(pointer_t ptr);
		static void btWheelInfo_setBrake(pointer_t ptr, btScalar brake);
		static btScalar btWheelInfo_getBrake(pointer_t ptr);
		static pointer_t btWheelInfo_getWorldTransform(pointer_t ptr);
		static btScalar btWheelInfo_getRrotation(pointer_t ptr);
		static btScalar btWheelInfo_getDeltaRotation(pointer_t ptr);

		static pointer_t btHeightfieldTerrainShape_create(int heightStickWidth, int heightStickLength, const pointer_t heightfieldData, btScalar heightScale,
			btScalar minHeight, btScalar maxHeight, int datatype);
		static pointer_t _malloc(int size);
		static void copyJSArray(pointer_t ptr, JSValueAsParam jsarray);
		static void _free(pointer_t ptr);
		static void setDrawlineFunction(JSValueAsParam pFunction);
		static void setClearlineFunction(JSValueAsParam pFunction);
		static void btConcaveShape_setMargin(pointer_t ptr, btScalar collisionMargin);
		static btScalar btConcaveShape_getMargin(pointer_t ptr);
		static void btRigidBody_setCenterOfMassOrientation(pointer_t ptr, btScalar x, btScalar y, btScalar z, btScalar w);
		static void btKinematicCharacterController_setPushForce(pointer_t ptr, btScalar force);
		static btScalar btKinematicCharacterController_getPushForce(pointer_t ptr);
		static void btKinematicCharacterController_setCurrentPosition(pointer_t ptr, btScalar x, btScalar y, btScalar z);
		static void btKinematicCharacterController_setUseGhostSweepTest(pointer_t ptr, bool b);
		static pointer_t btKinematicCharacterController_AllHitInfo_get_m_collisionObjects(pointer_t ptr);
		static void btGeneric6DofSpring2Constraint_setRotationOrder(pointer_t g6ds2Constraintptr, int order);
        static bool solveSphereHit(btScalar fromx, btScalar fromy, btScalar fromz, btScalar tox, btScalar toy, btScalar toz, pointer_t npSphere, int group, int mask, pointer_t npWorld, pointer_t noutPos);

		//pointer_t btSoftRigidDynamicsWorld_create(pointer_t dispatcher, pointer_t pairCache, pointer_t constraintSolver, pointer_t collisionConfiguration);
		//void btSoftRigidDynamicsWorld_addSoftBody(pointer_t ptr, pointer_t bodyptr);
		//void btSoftRigidDynamicsWorld_removeSoftBody(pointer_t worldptr, pointer_t bodyptr);
		//void btSoftRigidDynamicsWorld_removeCollisionObject(pointer_t worldptr, pointer_t bodyptr);
		//void btRigidBody_setSurfaceVelocity(pointer_t ptr, pointer_t vel, bool isLocal);

		//void btRigidBody_setSurfaceVelocityxyz(pointer_t ptr, btScalar x, btScalar y, btScalar z, bool isLocal);
		static pointer_t btRigidBody_getSurfaceVelocity(pointer_t ptr);
		//void btCollisionObject_setHasCDCallback(pointer_t ptr,bool b);
    };
}
#endif
