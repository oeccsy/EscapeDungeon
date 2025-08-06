#include "BoxCollider.h"
#include "Actor/Actor.h"

BoxCollider::BoxCollider(Vector2 center, Vector2 extends) : center(center), extends(extends) { }
BoxCollider::BoxCollider(Vector2 center, Vector2 extends, Actor* owner) : center(center), extends(extends), owner(owner) { }

Vector2 BoxCollider::GetMaxBound()
{
    Vector2 maxBound = center + extends;
    if (owner != nullptr) maxBound = maxBound + owner->GetPosition();

    return maxBound;
}

Vector2 BoxCollider::GetMinBound()
{
    Vector2 minBound = center - extends;
    if (owner != nullptr) minBound = minBound + owner->GetPosition();

    return minBound;
}


bool BoxCollider::Intersects(BoxCollider* other)
{
    Vector2 ownMaxBound = GetMaxBound();
    Vector2 ownMinBound = GetMinBound();

    Vector2 otherMaxBound = other->GetMaxBound();
    Vector2 otherMinBound = other->GetMinBound();
    
    if (ownMaxBound.x < otherMinBound.x || ownMinBound.x > otherMaxBound.x) return false;
    if (ownMaxBound.y < otherMinBound.y || ownMinBound.y > otherMaxBound.y) return false;

    return true;
}

