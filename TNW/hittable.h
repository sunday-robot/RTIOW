#ifndef HITTABLE_H
#define HITTABLE_H
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

#include "hit_record.h"
#include "aabb.h"

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const = 0;
	
	// レンダリング前に一度呼ばれるだけのものなので、inline化などの考慮は不要
	virtual aabb bounding_box(double exposureTime) const = 0;
};
#endif
