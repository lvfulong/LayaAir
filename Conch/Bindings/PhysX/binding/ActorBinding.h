//
// Created by Oasis on 2023/3/21.
//

#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>

#include "../BindingHelper.h"
#include "PxPhysicsAPI.h"

using namespace physx;
#ifdef CONCH

#else
using namespace emscripten;
#endif
EMSCRIPTEN_BINDINGS(physx_actor) {
    enum_<PxForceMode::Enum>("PxForceMode")
            .value("eFORCE", PxForceMode::Enum::eFORCE)
            .value("eIMPULSE", PxForceMode::Enum::eIMPULSE)
            .value("eVELOCITY_CHANGE", PxForceMode::Enum::eVELOCITY_CHANGE)
            .value("eACCELERATION", PxForceMode::Enum::eACCELERATION);

    enum_<PxActorFlag::Enum>("PxActorFlag").value("eDISABLE_GRAVITY", PxActorFlag::Enum::eDISABLE_GRAVITY)
                                           .value("eVISUALIZATION", PxActorFlag::Enum::eVISUALIZATION)
                                           .value("eSEND_SLEEP_NOTIFIES", PxActorFlag::Enum::eSEND_SLEEP_NOTIFIES)
                                           .value("eDISABLE_SIMULATION", PxActorFlag::Enum::eDISABLE_SIMULATION);

    /** PhysXCollider ✅ */
    class_<PxActor>("PxActor").function("setActorFlag", &PxActor::setActorFlag)
    .function("release", &PxActor::release)
    .function_optional_override("setUUID", optional_override([](PxActor &actor, uint32_t uuid) {
                          auto ptr = malloc(sizeof(uint32_t));
                          memcpy(ptr, &uuid, sizeof(uint32_t));
                          actor.userData = ptr;
            }))
    .function_optional_override("getUUID", optional_override([](PxActor &actor) {
                    return getActorUUID(&actor);
            }))
    .function_optional_override("setCustomFlag", optional_override([](PxActor &actor, int flag,bool value) {
                          actor.setActorFlag(PxActorFlag::Enum(flag),value);
            }));

    class_<PxRigidActor, base<PxActor>>("PxRigidActor")
            .function("attachShape", &PxRigidActor::attachShape)                            // ✅
            .function("detachShape", &PxRigidActor::detachShape)                            // ✅
            .function("getGlobalPose", &PxRigidActor::getGlobalPose, allow_raw_pointers())  // ✅
            .function("setGlobalPose", &PxRigidActor::setGlobalPose, allow_raw_pointers())  // ✅
            .function_optional_override("getShape", optional_override([](PxRigidActor &actor) {
                          PxShape *shape;
                          actor.getShapes(&shape, 1);
                          return shape;
                      }),
                      allow_raw_pointers());
    /** PhysXStaticCollider ✅ */
    class_<PxRigidStatic, base<PxRigidActor>>("PxRigidStatic");
    /** PhysXDynamicCollider ✅ */
    class_<PxRigidBody, base<PxRigidActor>>("PxRigidBody")
            .function("setAngularDamping", &PxRigidBody::setAngularDamping)  // ✅
            .function("getAngularDamping", &PxRigidBody::getAngularDamping)
            .function("setLinearDamping", &PxRigidBody::setLinearDamping)  // ✅
            .function("getLinearDamping", &PxRigidBody::getLinearDamping)

            .function("getAngularVelocity", &PxRigidBody::getAngularVelocity)
    
            .function("getLinearVelocity", &PxRigidBody::getLinearVelocity)
            .function("setMaxAngularVelocity", &PxRigidBody::setMaxAngularVelocity)  // ✅
            .function("getMaxAngularVelocity", &PxRigidBody::getMaxAngularVelocity)
            .function("setMaxDepenetrationVelocity", &PxRigidBody::setMaxDepenetrationVelocity)  // ✅
            .function("getMaxDepenetrationVelocity", &PxRigidBody::getMaxDepenetrationVelocity)
            .function("setMass", &PxRigidBody::setMass)  // ✅
            .function("getMass", &PxRigidBody::getMass)
            .function_optional_override("setCMassLocalPose", optional_override([](PxRigidBody &body, const PxVec3 &pos) {
                          return body.setCMassLocalPose(PxTransform(pos, PxQuat(PxIDENTITY::PxIdentity)));
                      }))                                                                    // ✅
            .function("setMassSpaceInertiaTensor", &PxRigidBody::setMassSpaceInertiaTensor)  // ✅
            .function_optional_override("addTorque", optional_override([](PxRigidBody &body, const PxVec3 &torque) {
                          body.addTorque(torque, PxForceMode::eFORCE, true);
                      }))  // ✅
            .function_optional_override("addForce", optional_override([](PxRigidBody &body, const PxVec3 &force) {
                          body.addForce(force, PxForceMode::eFORCE, true);
                      }))  // ✅
            .function_optional_override("addForceAtPos", optional_override([](PxRigidBody &body, const PxVec3 &force, const PxVec3 &pos) {
                          PxRigidBodyExt::addForceAtPos(body, force, pos, PxForceMode::eFORCE, true);
                      }))
            .function_optional_override("addForceAtLocalPos",
                      optional_override([](PxRigidBody &body, const PxVec3 &force, const PxVec3 &pos) {
                          PxRigidBodyExt::addForceAtLocalPos(body, force, pos, PxForceMode::eFORCE, true);
                      }))
            .function_optional_override("addLocalForceAtLocalPos",
                      optional_override([](PxRigidBody &body, const PxVec3 &force, const PxVec3 &pos) {
                          PxRigidBodyExt::addLocalForceAtLocalPos(body, force, pos, PxForceMode::eFORCE, true);
                      }))
            .function_optional_override("addImpulseAtPos",
                      optional_override([](PxRigidBody &body, const PxVec3 &impulse, const PxVec3 &pos) {
                          PxRigidBodyExt::addForceAtPos(body, impulse, pos, PxForceMode::eIMPULSE, true);
                      }))
            .function_optional_override("addImpulseAtLocalPos",
                      optional_override([](PxRigidBody &body, const PxVec3 &impulse, const PxVec3 &pos) {
                          PxRigidBodyExt::addForceAtLocalPos(body, impulse, pos, PxForceMode::eIMPULSE, true);
                      }))
            .function_optional_override("addLocalImpulseAtLocalPos",
                      optional_override([](PxRigidBody &body, const PxVec3 &impulse, const PxVec3 &pos) {
                          PxRigidBodyExt::addLocalForceAtLocalPos(body, impulse, pos, PxForceMode::eIMPULSE, true);
                      }))
            .function_optional_override("getVelocityAtPos", optional_override([](PxRigidBody &body, const PxVec3 &pos) {
                          return PxRigidBodyExt::getVelocityAtPos(body, pos);
                      }))
            .function_optional_override("getLocalVelocityAtLocalPos", optional_override([](PxRigidBody &body, const PxVec3 &pos) {
                          return PxRigidBodyExt::getLocalVelocityAtLocalPos(body, pos);
                      }))
            .function("setRigidBodyFlag", &PxRigidBody::setRigidBodyFlag)
            .function_optional_override("getRigidBodyFlags", optional_override([](PxRigidBody &body) {
                          return (bool)(body.getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC);
                      }))  // ✅
            .function_optional_override("setMassAndUpdateInertia", optional_override([](PxRigidBody &body, PxReal mass) {
                          return PxRigidBodyExt::setMassAndUpdateInertia(body, mass, nullptr, false);
                      }));
    /** PhysXDynamicCollider ✅ */
    class_<PxRigidDynamic, base<PxRigidBody>>("PxRigidDynamic")
            .function("setSleepThreshold", &PxRigidDynamic::setSleepThreshold)  // ✅
            .function("getSleepThreshold", &PxRigidDynamic::getSleepThreshold)
            .function("setSolverIterationCounts", &PxRigidDynamic::setSolverIterationCounts)  // ✅
            .function("wakeUp", &PxRigidDynamic::wakeUp)                                      // ✅
            .function("setWakeCounter", &PxRigidDynamic::setWakeCounter)
            .function("isSleeping", &PxRigidDynamic::isSleeping)
            .function("putToSleep", &PxRigidDynamic::putToSleep)  // ✅
            .function("getWakeCounter", &PxRigidDynamic::getWakeCounter)
            .function_optional_override("setKinematicTarget",
                      optional_override([](PxRigidDynamic &body, const PxVec3 &pos, const PxQuat &rot) {
                          return body.setKinematicTarget(PxTransform(pos, rot));
                      }))  // ✅
            .function("setRigidDynamicLockFlag", &PxRigidDynamic::setRigidDynamicLockFlag)
            .function_optional_override("setRigidDynamicLockFlags", optional_override([](PxRigidDynamic &body, int flags) {
                          return body.setRigidDynamicLockFlags(PxRigidDynamicLockFlags(flags));
                      }))  // ✅
            .function("setAngularVelocity", &PxRigidDynamic::setAngularVelocity)  // ✅
            .function("setLinearVelocity", &PxRigidDynamic::setLinearVelocity);  // ✅
    class_<PxRigidBodyFlags>("PxRigidBodyFlags");
    enum_<PxRigidBodyFlag::Enum>("PxRigidBodyFlag")
            .value("eKINEMATIC", PxRigidBodyFlag::Enum::eKINEMATIC)
            .value("eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES",
                   PxRigidBodyFlag::Enum::eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES)
            .value("eENABLE_CCD", PxRigidBodyFlag::Enum::eENABLE_CCD)
            .value("eENABLE_CCD_FRICTION", PxRigidBodyFlag::Enum::eENABLE_CCD_FRICTION)
            .value("eENABLE_POSE_INTEGRATION_PREVIEW", PxRigidBodyFlag::Enum::eENABLE_POSE_INTEGRATION_PREVIEW)
            .value("eENABLE_SPECULATIVE_CCD", PxRigidBodyFlag::Enum::eENABLE_SPECULATIVE_CCD)
            .value("eENABLE_CCD_MAX_CONTACT_IMPULSE", PxRigidBodyFlag::Enum::eENABLE_CCD_MAX_CONTACT_IMPULSE)
            .value("eRETAIN_ACCELERATIONS", PxRigidBodyFlag::Enum::eRETAIN_ACCELERATIONS);
}
#ifdef CONCH
namespace laya {
#else
namespace emscripten {
#endif
namespace internal {
template <>
void raw_destructor<PxRigidDynamic>(PxRigidDynamic *) { /* do nothing */
}

template <>
void raw_destructor<PxRigidBody>(PxRigidBody *) { /* do nothing */
}

template <>
void raw_destructor<PxRigidActor>(PxRigidActor *) { /* do nothing */
}

template <>
void raw_destructor<PxActor>(PxActor *) { /* do nothing */
}

template <>
void raw_destructor<PxRigidStatic>(PxRigidStatic *) { /* do nothing */
}


}  // namespace internal
}  // namespace emscripten
