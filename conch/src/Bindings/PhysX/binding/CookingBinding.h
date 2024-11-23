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
using namespace laya;
namespace laya
{
extern char* PHYSX_HEAP_BASE_ADDRESS;
}
#else
using namespace emscripten;
#endif
#ifdef CONCH
PxConvexMesh *createConvexMeshFromBuffer(jsvm::Value verticesObj, PxPhysics &physics, PxU32 VetexLimit, PxTolerancesScale &scale, int ConvexFlags)
{
    VectorWrapper<PxVec3>* pVertices = (VectorWrapper<PxVec3>* )jsbind::as<VectorWrapper<PxVec3>*>(verticesObj);
    std::vector<PxVec3>& vertices = pVertices->data_;
#else
// eCOMPUTE_CONVEX;
PxConvexMesh *createConvexMeshFromBuffer(std::vector<PxVec3> vertices, PxPhysics &physics, PxU32 VetexLimit, PxTolerancesScale &scale, int ConvexFlags)
{
#endif
    PxConvexMeshDesc convexDesc;
    convexDesc.points.count = vertices.size();
    convexDesc.points.stride = sizeof(PxVec3);
    convexDesc.points.data = vertices.data();
    convexDesc.flags = PxConvexFlag::Enum(ConvexFlags);
    convexDesc.vertexLimit = VetexLimit;
    PxCookingParams params(scale);
    PxDefaultMemoryOutputStream buf;
    PxConvexMeshCookingResult::Enum result;
    if (!PxCookConvexMesh(params, convexDesc, buf, &result))
    {
        return NULL;
    }
    PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    PxConvexMesh *convexMesh = physics.createConvexMesh(input);
    return convexMesh;
}
#ifdef CONCH
PxTriangleMesh *createTriMesh(jsvm::Value verticesObj,
                              int indice_sptr,
                              int indiceCount,
                              bool isU16,
                              PxTolerancesScale &scale,
                              PxPhysics &physics)
{
	VectorWrapper<PxVec3>* pVertices = (VectorWrapper<PxVec3>* )jsbind::as<VectorWrapper<PxVec3>*>(verticesObj);
    std::vector<PxVec3>& vertices = pVertices->data_;
#else
PxTriangleMesh *createTriMesh(std::vector<PxVec3> vertices,
                              int indice_sptr,
                              int indiceCount,
                              bool isU16,
                              PxTolerancesScale &scale,
                              PxPhysics &physics)
{
#endif
    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = vertices.size();
    meshDesc.points.stride = sizeof(PxVec3);
    meshDesc.points.data = vertices.data();
    meshDesc.triangles.count = indiceCount / 3;
    if (isU16)
    {
        meshDesc.triangles.stride = 3 * sizeof(PxU16);
        #ifdef CONCH
        meshDesc.triangles.data = reinterpret_cast<PxU16 *>(indice_sptr + PHYSX_HEAP_BASE_ADDRESS);
        #else
        meshDesc.triangles.data = reinterpret_cast<PxU16 *>(indice_sptr);
        #endif
        meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;
    }
    else
    {
        meshDesc.triangles.stride = 3 * sizeof(PxU32);
        #ifdef CONCH
        meshDesc.triangles.data = reinterpret_cast<PxU32 *>(indice_sptr + PHYSX_HEAP_BASE_ADDRESS);
        #else
        meshDesc.triangles.data = reinterpret_cast<PxU32 *>(indice_sptr);
        #endif
    }
    PxCookingParams params(scale);

    PxDefaultMemoryOutputStream writeBuffer;
    PxTriangleMeshCookingResult::Enum result;
    bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
    if (!status)
        return NULL;

    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    return physics.createTriangleMesh(readBuffer);
}

PxHeightField *createHeightField(int numRows, int numCols,
                                 int height_ptr,
                                 int flag_ptr,
                                 PxDefaultAllocator &gAllocator,
                                 PxTolerancesScale &scale,
                                 PxPhysics &physics)
{
    PxHeightFieldSample *samples = (PxHeightFieldSample *)gAllocator.allocate(sizeof(PxHeightFieldSample) * (numRows * numCols), 0, 0, 0);
    PxReal quantization = (PxReal)0x7fff;
    #ifdef CONCH
    float *heightData = reinterpret_cast<float *>(height_ptr + PHYSX_HEAP_BASE_ADDRESS);
    uint8_t *flag = reinterpret_cast<uint8_t *>(flag_ptr + PHYSX_HEAP_BASE_ADDRESS);
    #else
    float *heightData = reinterpret_cast<float *>(height_ptr);
    uint8_t *flag = reinterpret_cast<uint8_t *>(flag_ptr);
    #endif

    for (int i = 0, n = numRows * numCols; i < n; i++)
    {
        samples[i].height = PxI16(quantization * heightData[i]);
        samples[i].materialIndex0 = (PxBitAndByte)((flag[i] == 0?0:1)<<7);
    }

    PxHeightFieldDesc hfDesc;
    hfDesc.format = PxHeightFieldFormat::eS16_TM;
    hfDesc.nbColumns = numCols;
    hfDesc.nbRows = numRows;
    hfDesc.samples.data = samples;
    hfDesc.samples.stride = sizeof(PxHeightFieldSample);
    PxHeightField *aHeightField = PxCreateHeightField(hfDesc,
                                                      physics.getPhysicsInsertionCallback());
    return aHeightField;
}

EMSCRIPTEN_BINDINGS(physx_cooking)
{
    function("PxCreateCooking", &PxCreateCooking, allow_raw_pointers());
    function("createConvexMeshFromBuffer", &createConvexMeshFromBuffer, allow_raw_pointers());
    function("createHeightField", &createHeightField, allow_raw_pointers());
    function("createTriMesh", &createTriMesh, allow_raw_pointers());
    class_<PxMeshScale>("PxMeshScale").constructor<const PxVec3 &, const PxQuat &>().property("scale", &PxMeshScale::scale).property("rotation", &PxMeshScale::rotation);
    // triangleMesh
    class_<PxTriangleMesh>("PxTriangleMesh").function("release", &PxTriangleMesh::release);
    class_<PxTriangleMeshGeometry, base<PxGeometry>>("PxTriangleMeshGeometry")
        .constructor<PxTriangleMesh *, const PxMeshScale &, PxMeshGeometryFlags>()
        .property("scale", &PxTriangleMeshGeometry::scale),
        class_<PxMeshGeometryFlags>("PxMeshGeometryFlags").constructor<int>();
    enum_<PxMeshGeometryFlag::Enum>("PxMeshGeometryFlag")
        .value("eTIGHT_BOUNDS", PxMeshGeometryFlag::Enum::eTIGHT_BOUNDS)
        .value("eDOUBLE_SIDED", PxMeshGeometryFlag::Enum::eDOUBLE_SIDED);
    // ConvexMesh
    class_<PxConvexMesh>("PxConvexMesh")
    .function("release", &PxConvexMesh::release)
    .function_optional_override("getVertices",optional_override([](PxConvexMesh &convexMesh) {
        PxU32 vertexNumber =convexMesh.getNbVertices();
        const PxVec3*	vertexs =convexMesh.getVertices();
        std::vector<PxVec3> vecVertex(vertexs,vertexs+vertexNumber);
        return vecVertex;
                      }))
    .function_optional_override("getIndexBuffer",optional_override([](PxConvexMesh &convexMesh) {
        const PxU8* index = convexMesh.getIndexBuffer();
		PxU32 indexBufferSize = 0;
        const PxU32 nbPolys = convexMesh.getNbPolygons();
        for(PxU32 i = 0; i < nbPolys; i++)
        {
            PxHullPolygon data;
            convexMesh.getPolygonData(i, data);
            const PxU32 offlength = PxU32(data.mIndexBase+data.mNbVerts);
            if(indexBufferSize<offlength){
                indexBufferSize = offlength;
            }
        }
        printf("getIndexBuffer Size:%d\n",indexBufferSize);
        std::vector<PxU8> u8vec(index,index+indexBufferSize);
        return u8vec;
                      }))
    .function_optional_override("getPolygons",optional_override([](PxConvexMesh &convexMesh) {
        std::vector<int> returnData;
		PxHullPolygon data;
		PxU32 polygonsNums = convexMesh.getNbPolygons();
		PxU32 index = 0;
		for(index=0;index<polygonsNums;index++)
		{
			convexMesh.getPolygonData(index, data);
			returnData.push_back(int(data.mNbVerts));
			returnData.push_back(int(data.mIndexBase));
		}
        return returnData;
                      }));
    class_<PxConvexMeshGeometry, base<PxGeometry>>("PxConvexMeshGeometry")
        .constructor<PxConvexMesh *, const PxMeshScale &, PxConvexMeshGeometryFlags>()
        .property("scale", &PxConvexMeshGeometry::scale);
    class_<PxConvexMeshGeometryFlags>("PxConvexMeshGeometryFlags").constructor<int>();
    enum_<PxConvexMeshGeometryFlag::Enum>("PxConvexMeshGeometryFlag")
        .value("eTIGHT_BOUNDS", PxConvexMeshGeometryFlag::Enum::eTIGHT_BOUNDS);

    // heightField
    class_<PxHeightField>("PxHeightField")
        .function("release", &PxHeightField::release)
        // TODO.function("modifySamples",&PxHeightField::modifySamples)
        .function("getNbRows", &PxHeightField::getNbRows)
        .function("getNbColumns", &PxHeightField::getNbColumns)
        .function("getFormat", &PxHeightField::getFormat)
        .function("getHeight", &PxHeightField::getHeight);

    class_<PxHeightFieldGeometry, base<PxGeometry>>("PxHeightFieldGeometry").constructor<PxHeightField *, PxMeshGeometryFlags, PxReal, PxReal, PxReal>();
    // class_<PxCooking>("PxCooking")
    //         .function("createConvexMeshWithIndices",
    //                   optional_override([](PxCooking &cooking, int vertices, PxU32 vertCount, int indices,
    //                                        PxU32 indexCount, bool isU16, PxPhysics &physics) {
    //                       return createConvexMeshFromBuffer(vertices, vertCount, indices, indexCount, isU16, cooking,
    //                                                         physics);
    //                   }),
    //                   allow_raw_pointers())
    //         .function("createConvexMesh",
    //                   optional_override([](PxCooking &cooking, int vertices, PxU32 vertCount, PxPhysics &physics) {
    //                       return createConvexMeshFromBuffer(vertices, vertCount, cooking, physics);
    //                   }),
    //                   allow_raw_pointers())
    //         .function("createTriMesh",
    //                   optional_override([](PxCooking &cooking, int vertices, PxU32 vertCount, int indices,
    //                                        PxU32 indexCount, bool isU16, PxPhysics &physics) {
    //                       return createTriMesh(vertices, vertCount, indices, indexCount, isU16, cooking, physics);
    //                   }),
    //                   allow_raw_pointers());
    class_<PxCookingParams>("PxCookingParams").constructor<PxTolerancesScale>();
}

#ifdef CONCH
namespace laya {
#else
namespace emscripten {
#endif
    namespace internal
    {
        template <>
        void raw_destructor<PxHeightField>(PxHeightField *)
        { /* do nothing */
        }

        template <>
        void raw_destructor<PxConvexMesh>(PxConvexMesh *)
        { /* do nothing */
        }

        template <>
        void raw_destructor<PxTriangleMesh>(PxTriangleMesh *)
        { /* do nothing */
        }

        template <>
        void raw_destructor<PxCooking>(PxCooking *)
        { /* do nothing */
        }
    } // namespace internal
} // namespace emscripten
