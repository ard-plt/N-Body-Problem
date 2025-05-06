#include <iostream>
#include <cmath>

const double G = 1.0;
const double timestep = 0.2;

class vector2D {
public:
    double x, y;

    vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    vector2D operator+(const vector2D& v) const {
        return vector2D(x + v.x, y + v.y);
    }
    vector2D operator-(const vector2D& v) const {
        return vector2D(x - v.x, y - v.y);
    }
    vector2D operator*(double scalar) const {
        return vector2D(x * scalar, y * scalar);
    }
    vector2D operator/(double scalar) const {
        return scalar != 0 ? vector2D(x / scalar, y / scalar) : vector2D();
    }

    vector2D normalize() const {
        double mag = sqrt(x * x + y * y);
        if (mag == 0) return vector2D(0, 0);
        return *this / mag;
    }
};

class object {
public:
    double mass;
    vector2D position, velocity, acceleration;

    object(double mass_, vector2D position_, vector2D velocity_) : mass(mass_), position(position_), velocity(velocity_) {}

    virtual ~object() {}

    virtual void update(vector2D force) {
        acceleration = force / mass;
        velocity = velocity + acceleration * timestep;
        position = position + velocity * timestep;
    }
};

class rocket : public object {
public:
    double exhaustVelocity, exhaustRate, fuelAmount;
    double massChange;

    rocket(double rocketMass, double fuel, double exVelocity, double rate, vector2D pos, vector2D vel)
        : object(rocketMass + fuel, pos, vel),
          exhaustVelocity(exVelocity),
          exhaustRate(rate),
          fuelAmount(fuel) {}

    void update(vector2D force) override {
        if (fuelAmount > 0) {
            massChange = exhaustRate * timestep;
            if (massChange > fuelAmount) {
                massChange = fuelAmount;
            }
            fuelAmount -= massChange;
            mass = std::max(mass - massChange, 0.0);
            vector2D thrust(0, exhaustVelocity * massChange / timestep);
            force = force + thrust;
        }
        object::update(force);
    }
};

class node {
public:
    object* obj;
    node* next;
    node(object* o) : obj(o), next(nullptr) {}
};

class objectList {
public:
    node* head;
    objectList() : head(nullptr) {}

    void add(object* o) {
        node* newNode = new node(o);
        newNode->next = head;
        head = newNode;
    }

    ~objectList() {
        while (head) {
            node* temp = head;
            head = head->next;
            delete temp->obj;
            delete temp;
        }
    }
};

class simulation {
public:
    objectList objects;

    void addObject(object* o) {
        objects.add(o);
    }

    void update() {
        node* i = objects.head;
        while (i) {
            vector2D netForce(0, 0);
            node* j = objects.head;
            while (j) {
                if (i != j) {
                    vector2D diff = j->obj->position - i->obj->position;
                    double distance = sqrt(diff.x * diff.x + diff.y * diff.y);
                    if (distance > 0) {
                        double forceMagnitude = G * i->obj->mass * j->obj->mass / (distance * distance);
                        netForce = netForce + diff.normalize() * forceMagnitude;
                    }
                }
                j = j->next;
            }
            i->obj->update(netForce);
            i = i->next;
        }
    }

    double totalEnergy() {
        double energy = 0;
        node* i = objects.head;
        while (i) {
            energy += 0.5 * i->obj->mass * (i->obj->velocity.x * i->obj->velocity.x + i->obj->velocity.y * i->obj->velocity.y);
            node* j = i->next;
            while (j) {
                vector2D diff = j->obj->position - i->obj->position;
                double distance = sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance > 0) {
                    energy -= (G * i->obj->mass * j->obj->mass) / distance;
                }
                j = j->next;
            }
            i = i->next;
        }
        return energy;
    }

    vector2D getPosition(int index) {
        node* temp = objects.head;
        int count = 1;
        while (temp) {
            if (count == index) {
                return temp->obj->position;
            }
            temp = temp->next;
            count++;
        }
        return vector2D();
    }
};
