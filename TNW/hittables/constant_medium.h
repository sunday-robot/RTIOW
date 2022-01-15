#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H
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

#include "../hittable.h"
#include "../texture.h"
#include "../materials/isotropic.h"

class constant_medium : public hittable {
public:
	constant_medium(std::shared_ptr<hittable> b, double d, std::shared_ptr<texture> a)
		: boundary(b),
		neg_inv_density(-1 / d),
		phase_function(std::make_shared<isotropic>(a))
	{}

	constant_medium(std::shared_ptr<hittable> b, double d, color c)
		: boundary(b),
		neg_inv_density(-1 / d),
		phase_function(std::make_shared<isotropic>(c))
	{}

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override {
		return boundary->bounding_box(exposureTime);
	}

	virtual void print(std::string indent = "") const override;

public:
	std::shared_ptr<hittable> boundary;
	std::shared_ptr<material> phase_function;
	double neg_inv_density;
};
#endif
