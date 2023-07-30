#include "physics/contact_solver.h"

#include "physics/physics.h"

namespace neko
{


void Contact::Resolve(Scalar dt) const
{
    ResolveVelocity(dt);
    ResolveInterpenetration(dt);
}

Scalar Contact::CalculateSeparateVelocity() const
{
    const auto relativeVelocity = bodies[0].body->velocity - bodies[1].body->velocity;
    return Vec2f::Dot(relativeVelocity, contactNormal);
}

void Contact::ResolveVelocity(Scalar dt) const
{
    const auto separatingVelocity = CalculateSeparateVelocity();
    if (separatingVelocity > Scalar{0})
    {
        return;
    }
    const auto newSepVelocity = -separatingVelocity * restitution;
    const auto deltaVelocity = newSepVelocity - separatingVelocity;
    const auto inverseMass1 = Scalar{ 1 } / bodies[0].body->mass;
    const auto inverseMass2 = Scalar{ 1 } / bodies[1].body->mass;
    const auto totalInverseMass = inverseMass1 + inverseMass2;
    const auto impulse = deltaVelocity / totalInverseMass;
    const auto impulsePerIMass = contactNormal * impulse;
    bodies[0].body->velocity = bodies[0].body->velocity + impulsePerIMass * inverseMass1;
    bodies[1].body->velocity = bodies[1].body->velocity - impulsePerIMass * inverseMass2;
}

void Contact::ResolveInterpenetration(Scalar dt) const
{
    // If we don�t have any penetration, skip this step.
    if (penetration <= Scalar{0}) return;
    const auto inverseMass1 = Scalar{ 1 } / bodies[0].body->mass;
    const auto inverseMass2 = Scalar{ 1 } / bodies[1].body->mass;
    const auto totalInverseMass = inverseMass1 + inverseMass2;
    // If all particles have infinite mass, then we do nothing.
    if (totalInverseMass <= Scalar{0}) return;
    // Find the amount of penetration resolution per unit of inverse mass.
    const auto movePerIMass = contactNormal *
        (-penetration / totalInverseMass);
    // Apply the penetration resolution.
    bodies[0].body->position = bodies[0].body->position + movePerIMass * inverseMass1;
    bodies[1].body->position = bodies[1].body->position + movePerIMass * inverseMass2;
} // namespace neko
}