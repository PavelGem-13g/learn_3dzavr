//
// Created by Иван Ильин on 14.01.2021.
//

#include <cmath>

#include "Camera.h"
#include "utils/Log.h"
#include "Consts.h"

std::vector<Triangle> Camera::project(std::shared_ptr<Mesh> mesh) {

    if (!_ready) {
        Log::log("Camera::project(): cannot project _tris without camera initialization ( Camera::init() ) ");
        return _triangles;
    }

    if (!mesh->isVisible()) {
        return this->_triangles;
    }

    Matrix4x4 M = mesh->model();
    Matrix4x4 V = Matrix4x4::View(left(), up(), lookAt(), position());

    std::vector<Triangle> clippedTriangles, tempBuffer;
    for (auto &t : mesh->triangles()) {
        Triangle modelTriangle = t*M;

        double dot = modelTriangle.norm().dot((position() - Vec3D (modelTriangle[0])).normalized());

        if(dot < 0){
            continue;
        }

        Triangle modelViewTriangle = modelTriangle*V;

        clippedTriangles.emplace_back(modelViewTriangle);
        for(auto &plane : _clipPlanes){
            while (!clippedTriangles.empty()){
                 auto clipResult = plane.clip(clippedTriangles.back());
                 clippedTriangles.pop_back();
                 for(auto &clipTri : clipResult){
                     tempBuffer.emplace_back(clipTri);
                 }
            }
            clippedTriangles.swap(tempBuffer);
        }

        for(auto &clippedTriangle : clippedTriangles){
            Triangle projected = clippedTriangle*_SP;
            Triangle projected_normalized =
                    Triangle(
                            projected[0]/projected[0].w(),
                            projected[1]/projected[1].w(),
                            projected[2]/projected[2].w());

            double dotColor = (0.1*dot)+0.9;

            projected_normalized.setColor(
                    sf::Color(t.color().r * dotColor,
                              t.color().g * dotColor,
                              t.color().b * dotColor));

            _triangles.emplace_back(projected_normalized);
        }
    }

    return this->_triangles;
}

void Camera::init(int width, int height, double fov, double ZNear, double ZFar) {
    // We need to init camera only after creation or changing width, height, fov, ZNear or ZFar.
    // Because here we calculate matrix that does not change during the motion of _objects or camera
    _aspect = (double) width / (double) height;
    Matrix4x4 P = Matrix4x4::Projection(fov, _aspect, ZNear, ZFar);
    Matrix4x4 S = Matrix4x4::ScreenSpace(width, height);

    _SP = S * P; // screen-space-projections matrix

    // This is planes for clipping _tris.
    // Motivation: we are not interest in _tris that we cannot see.
    _clipPlanes.emplace_back(Plane(Vec3D{0, 0, 1}, Vec3D{0, 0, ZNear})); // near plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, 0, -1}, Vec3D{0, 0, ZFar})); // far plane

    double thetta1 = Consts::PI * fov * 0.5 / 180.0;
    double thetta2 = atan(_aspect * tan(thetta1));
    _clipPlanes.emplace_back(Plane(Vec3D{-cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0})); // left plane
    _clipPlanes.emplace_back(Plane(Vec3D{cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0})); // right plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0})); // down plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, -cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0})); // up plane

    _ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

std::vector<Triangle> Camera::sorted() {

    // Sort _tris from back to front
    // This is some replacement for Z-buffer

    std::sort(_triangles.begin(), _triangles.end(), [](Triangle &t1, Triangle &t2){
        double z1 = t1[0].z() +t1[10].z()+t1[2].z();
        double z2 = t2[0].z() +t2[10].z()+t2[2].z();

        return z1 > z2;
    });

    return _triangles;
}

void Camera::clear() {
    // Cleaning all _tris and recalculation of View matrix
    _triangles.clear();
}
