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

class objects {
public:
    double mass;
    vector2D position, velocity, acceleration;

    objects(double mass_, vector2D position_, vector2D velocity_) : mass(mass_), position(position_), velocity(velocity_) {}

    virtual ~objects() {}

    virtual void update(vector2D force_) {
        acceleration = force_ / mass;
        velocity = velocity + acceleration * timestep;
        position = position + velocity * timestep;
    }
};

class rocket : public objects {
public:
    double exhaustVelocity, exhaustRate, fuelAmount;
    double massChange;

    rocket(double rocketMass, double fuel, double exVelocity, double rate, vector2D pos, vector2D vel)
        : objects(rocketMass + fuel, pos, vel),
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
        objects::update(force);
    }
};

class node {
public:
    objects* object;
    node* next;
    node(objects* c) : object(c), next(nullptr) {}
};

class list {
public:
    node* head;
    list() : head(nullptr) {}

    void add(objects* c) {
        node* new_ = new node(c);
        new_->next = head;
        head = new_;
    }
    ~list() {
        while (head) {
            node* temp = head;
                head = head->next;
            delete temp->object;
            delete temp;
        }
    }
};
class compare {
public:
    list objectList;

    void addObject(objects* c) { objectList.add(c); }

    void update(){
        node* i = objectList.head;
        while (i) {
            vector2D netForce(0, 0);
            node* j = objectList.head;
            while (j) {
                if (i != j) {
                    vector2D difference = j->object->position - i->object->position;
                    double distance = sqrt(difference.x * difference.x + difference.y * difference.y);
                    if (distance > 0) {
                        double forceMagnitude = G * i->object->mass * j->object->mass / (distance * distance);
                        netForce = netForce + difference.normalize() * forceMagnitude;
                    }
                }
                j = j->next;
            }
            i->object->update(netForce);
            i = i->next;
        }
    }
    double totalEnergy() {
        double energy = 0;
        node* i = objectList.head;
        while (i) {
            energy += 0.5 * i->object->mass * (i->object->velocity.x * i->object->velocity.x + i->object->velocity.y * i->object->velocity.y);
            node* j = i->next;
            while (j) {
                vector2D minus = j->object->position - i->object->position;
                double distance = sqrt(minus.x * minus.x + minus.y * minus.y);
                if (distance > 0) {
                    energy -= (G * i->object->mass * j->object->mass) / distance;
                }
                j = j->next;
            }
            i = i->next;
        }
        return energy;
    }

    vector2D getPosition(int begin) {
        node* temp = objectList.head;
        int count=1;
        while (temp) {
            if (count==begin) {
                return temp->object->position;
            }
            temp = temp->next;
            count++;
        }
        return vector2D();
    }
};
