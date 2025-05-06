#include "gravity.cpp"
#include "canvas.cpp"

int main() {

    canvas graphic("gravity");
    graphic.startDoc();
    graphic.drawFrame();

    compare U;
    int choice;
    while (true) {
        std::cout << "[1] Add Object" << std::endl;
        std::cout << "[2] Add Rocket" << std::endl;
        std::cout << "[0] Start The Simulation" << std::endl;
        std::cout << "Choose: ";
        std::cin >> choice;

        if (choice == 0) break;

        if (choice == 1) {
            double mass, x, y, vx, vy;
            std::cout << "Object mass: "; std::cin >> mass;
            std::cout << "Position x y: "; std::cin >> x >> y;
            std::cout << "Velocity vx vy: "; std::cin >> vx >> vy;

            objects* new_ = new objects(mass, vector2D(x, y), vector2D(vx, vy));
            U.addObject(new_);
        }
        else if (choice == 2) {
            double mass, fuel, exhaustSpeed, fuelRate, x, y, vx, vy;
            std::cout << "Rocket mass: "; std::cin >> mass;
            std::cout << "Fuel amount: "; std::cin >> fuel;
            std::cout << "Exhaust velocity vP: "; std::cin >> exhaustSpeed;
            std::cout << "Fuel consumption rate wP: "; std::cin >> fuelRate;
            std::cout << "Position x y: "; std::cin >> x >> y;
            std::cout << "Velocity vx vy: "; std::cin >> vx >> vy;

            rocket* new_ = new rocket(mass, fuel, exhaustSpeed, fuelRate, vector2D(x, y), vector2D(vx, vy));
            U.addObject(new_);
        }
    }

    int timeStep = 10000;
    for (int t = 0; t < timeStep; ++t) {
        U.update();

        int index = 1;
        node* ptr = U.objectList.head;
        while (ptr) {
            std::string color;
            switch (index) {
                case 1: color = "red"; break;
                case 2: color = "green"; break;
                case 3: color = "blue"; break;
                case 4: color = "black"; break;
                default: color = "gray"; break;
            }
            vector2D pos = ptr->object->position;
            graphic.drawPoint(pos.x, pos.y, color);
            ptr = ptr->next;
            index++;
        }

        std::cout << "Energy: " << U.totalEnergy() << std::endl;
    }

    graphic.finishDoc();
    return 0;
}
