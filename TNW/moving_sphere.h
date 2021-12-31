#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"


class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            point3 c, vec3 v, double r, shared_ptr<material> m)
            : center(c), velocity(v), radius(r), mat_ptr(m)
        {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double exposureTime, aabb& output_box) const override;

        point3 centerAt(double time) const;

    public:
        point3 center;
        vec3 velocity;
        double radius;
        shared_ptr<material> mat_ptr;
};


point3 moving_sphere::centerAt(double time) const{
    return center + velocity * time;
}


bool moving_sphere::bounding_box(double exposureTime, aabb& output_box) const {
    auto c0 = centerAt(-exposureTime / 2);
    auto c1 = centerAt(exposureTime / 2);
    aabb box0(
        c0 - vec3(radius, radius, radius),
        c0 + vec3(radius, radius, radius));
    aabb box1(
        c1 - vec3(radius, radius, radius),
        c1 + vec3(radius, radius, radius));
    output_box = surrounding_box(box0, box1);
    return true;
}


bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - centerAt(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - centerAt(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
