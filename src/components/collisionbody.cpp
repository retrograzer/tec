#include "components/collisionbody.hpp"

namespace tec {
	CollisionBody::CollisionBody() : mass(0.0),
		disable_deactivation(false), disable_rotation(false) {
		motion_state.transform_updated = true;
	}

	
	CollisionBody::CollisionBody(CollisionBody&& other) : mass(other.mass),
	disable_deactivation(other.disable_deactivation), disable_rotation(other.disable_rotation),
	motion_state(std::move(other.motion_state)), shape(std::move(other.shape)),
	entity_id(other.entity_id) {

	}

	CollisionBody::~CollisionBody() { }

	CollisionBody& CollisionBody::operator=(CollisionBody&& other) {
		mass = other.mass;
		disable_deactivation = other.disable_deactivation;
		disable_rotation = other.disable_rotation;
		motion_state = std::move(other.motion_state);
		shape = std::move(other.shape);
		entity_id = other.entity_id;
		return *this;
	}

	void CollisionBody::Out(proto::Component* target) {
		proto::CollisionBody* comp = target->mutable_collision_body();
		comp->set_disable_deactivation(this->disable_deactivation);
		comp->set_disable_rotation(this->disable_rotation);
		comp->set_mass(static_cast<float>(this->mass));
		switch (this->shape->getShapeType()) {
			case BOX_SHAPE_PROXYTYPE:
				{
					proto::CollisionBody::Box* box = comp->mutable_box();
					btVector3 half_extents = std::static_pointer_cast<btBoxShape>(this->shape)->getHalfExtentsWithMargin();
					box->set_x_extent(static_cast<float>(half_extents.getX()));
					box->set_y_extent(static_cast<float>(half_extents.getY()));
					box->set_z_extent(static_cast<float>(half_extents.getZ()));
				}
				break;
			case SPHERE_SHAPE_PROXYTYPE:
				{
					proto::CollisionBody::Sphere* sphere = comp->mutable_sphere();
					sphere->set_radius(static_cast<float>(std::static_pointer_cast<btSphereShape>(this->shape)->getRadius()));
				}
				break;
			case CAPSULE_SHAPE_PROXYTYPE:
				{
					proto::CollisionBody::Capsule* capsule = comp->mutable_capsule();
					auto capsule_shape = std::static_pointer_cast<btCapsuleShape>(this->shape);
					capsule->set_radius(static_cast<float>(capsule_shape->getRadius()));
					capsule->set_height(static_cast<float>(capsule_shape->getHalfHeight() * 2.0f));
				}
				break;
		}
	}

	void CollisionBody::In(const proto::Component& source) {
		const proto::CollisionBody& comp = source.collision_body();
		switch (comp.shape_case()) {
			case proto::CollisionBody::ShapeCase::kBox:
				{
					btVector3 half_extents = btVector3(comp.box().x_extent(), comp.box().y_extent(), comp.box().z_extent());
					this->shape = std::make_shared<btBoxShape>(half_extents);
				}
				break;
			case proto::CollisionBody::ShapeCase::kSphere:
				{
					float radius = comp.sphere().radius();
					this->shape = std::make_shared<btSphereShape>(radius);
				}
				break;
			case proto::CollisionBody::ShapeCase::kCapsule:
				{
					float radius = comp.capsule().radius();
					float height = comp.capsule().height();
					this->shape = std::make_shared<btCapsuleShape>(radius, height);
				}
				break;
		}

		if (comp.has_disable_deactivation()) {
			this->disable_deactivation = comp.disable_deactivation();
		}
		if (comp.has_disable_rotation()) {
			this->disable_rotation = comp.disable_rotation();
		}
		if (comp.has_mass()) {
			this->mass = comp.mass();
		}
	}
}
